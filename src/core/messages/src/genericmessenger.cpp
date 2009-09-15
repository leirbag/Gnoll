/***************************************************************************
 *   Copyright (C) 2006 by Puzzle Team                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "../include/genericmessenger.h"

#include <iostream>
#include <string>
#include <exception>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include "../include/messagetype.h"
#include "../include/listener.h"
#include "../../include/cmessage.h"

// TODO : name queue a list is inconsistent
// TODO : implementation is too big. Usage of the set is redundant with multimap key. To do after unittesting

namespace Gnoll
{
    namespace Core
    {
        namespace Messages
        {
            namespace Exceptions
            {
                class InvalidMessage : std::exception
                {
                };

                class HandlerAlreadyRegistered : std::exception
                {
                };

                class CannotDeleteListener : std::exception
                {
                };
                
                class NoOneIsListening : std::exception
                {
                };
            }

            GenericMessenger::GenericMessenger() :
                m_activeQueue(0)
            {
            }

            GenericMessenger::~GenericMessenger()
            {
            }

            void GenericMessenger::checkMessageValidity(MessagePtr message)
            {
                checkMessageTypeValidity(message->getType());
            }

            void GenericMessenger::checkMessageTypeValidity(const MessageType & type)
            {
                if (!isMessageTypeValid(type))
                {
                    throw Exceptions::InvalidMessage();
                }
            }


            bool GenericMessenger::isMessageTypeRegistered(const MessageType & messageType) const
            {
                return m_messageTypes.find(messageType) != m_messageTypes.end();
            }

            void GenericMessenger::registerMessageType(const MessageType & messageType)
            {
                std::pair<std::set<MessageType>::const_iterator, bool> insertResult = m_messageTypes.insert(messageType);

                assert(insertResult.second == true);
                assert(insertResult.first != m_messageTypes.end());
            }

            void GenericMessenger::addListener(ListenerPtr listener, const MessageType & messageType)
            {
                checkMessageTypeValidity(messageType);

                boost::recursive_mutex::scoped_lock lock(m_messageTypesMutex);

                if (!isMessageTypeRegistered(messageType))
                {
                    registerMessageType(messageType);
                }

                if (hasListenerForMessageType(messageType))
                {
                    throw Exceptions::HandlerAlreadyRegistered();
                }

                m_listeners.insert(std::pair<MessageType, ListenerPtr >(messageType, listener));
            }

            void GenericMessenger::eraseListenerFromContainer(ListenerPtr listener, const MessageType & messageType)
            {
                ListenerContainer::iterator it = m_listeners.lower_bound(messageType);
                ListenerContainer::iterator itEnd = m_listeners.upper_bound(messageType);

                while (it != itEnd)
                {
                    if ( (*it).second == listener )
                    {
                        m_listeners.erase(it);
                        break;
                    }
                    
                    ++it;
                }

                if (it == itEnd)
                {
                    throw Exceptions::CannotDeleteListener();
                }
            }

            bool GenericMessenger::hasListenerForMessageType(const MessageType & messageType) const
            {
                ListenerContainer::const_iterator result = m_listeners.find(messageType);

                return result != m_listeners.end();
            }

            void GenericMessenger::eraseMessageTypeOrphan(const MessageType & messageType)
            {
                assert (!isMessageTypeRegistered(messageType));

                if (!hasListenerForMessageType(messageType))
                {
                    m_messageTypes.erase(messageType);
                }
            }

            void GenericMessenger::delListener(ListenerPtr listener, const MessageType & messageType)
            {
                checkMessageTypeValidity(messageType);

                boost::recursive_mutex::scoped_lock lock(m_listenersMutex);

                if (!isMessageTypeRegistered(messageType))
                {
                    throw Exceptions::CannotDeleteListener();
                }

                eraseListenerFromContainer(listener, messageType);
                eraseMessageTypeOrphan(messageType);
            }

            void GenericMessenger::sendToListenersOfType(MessagePtr message, const MessageType & type)
            {
                ListenerContainer::iterator itEnd = m_listeners.upper_bound(type);
                for (ListenerContainer::iterator it = m_listeners.lower_bound(type);
                it != m_listeners.upper_bound(message->getType())  ; ++it)
                {
                    ((*it).second)->handle(message);
                }
            }

            void GenericMessenger::triggerMessage(MessagePtr message)
            {
                checkMessageValidity(message);

                boost::recursive_mutex::scoped_lock lock(m_listenersMutex);

                sendToListenersOfType(message, MessageType(MSG_ANYTYPE)); // TODO : MSG_ANYTYPE ? Mmm
                sendToListenersOfType(message, message->getType());
            }

            void GenericMessenger::addMessageToCurrentQueue(MessagePtr message)
            {
                boost::recursive_mutex::scoped_lock lockAQ(m_activeQueueMutex);
                boost::recursive_mutex::scoped_lock lockMsg(m_messagesMutex[m_activeQueue]);

                m_messages[m_activeQueue].push_back(message);
            }

            void GenericMessenger::queueMessage(MessagePtr message)
            {
                checkMessageValidity(message);

                boost::recursive_mutex::scoped_lock lock(m_messageTypesMutex);

                if (!hasListenerForMessageType(message->getType()))
                {
                    throw Exceptions::NoOneIsListening();
                }

                addMessageToCurrentQueue(message);
            }

            namespace Details
            {
                static bool isMessageOfType(Messenger::MessagePtr message, const MessageType & type)
                {
                    return message->getType() == type;
                }
            }

            void GenericMessenger::abortFirstMessage(const MessageType & messageType)
            {
                checkMessageTypeValidity(messageType);
                boost::recursive_mutex::scoped_lock lockAQ(m_activeQueueMutex);
                boost::recursive_mutex::scoped_lock lockMsg(m_messagesMutex[m_activeQueue]);

                list<MessagePtr> & messages = m_messages[m_activeQueue];

                list<MessagePtr>::iterator foundMessage = std::find_if(messages.begin(), messages.end(), boost::bind(Details::isMessageOfType, _1, messageType));

                if (foundMessage != messages.end())
                {
                    messages.erase(foundMessage);
                }
            }

            void GenericMessenger::abortAllMessages(const MessageType & messageType)
            {
                checkMessageTypeValidity(messageType);
                boost::recursive_mutex::scoped_lock lockAQ(m_activeQueueMutex);
                boost::recursive_mutex::scoped_lock lockMsg(m_messagesMutex[m_activeQueue]);

                list<MessagePtr> & messages = m_messages[m_activeQueue];

                messages.remove_if(boost::bind(Details::isMessageOfType, _1, messageType));
            }

            bool GenericMessenger::isMessageTypeValid(const MessageType & messageType) const
            {
                return !messageType.getTypeStr().empty();
            }

            void GenericMessenger::processQueue()
            {
                boost::recursive_mutex::scoped_lock lockAQ(m_activeQueueMutex);
                boost::recursive_mutex::scoped_lock lockMsg(m_messagesMutex[m_activeQueue]);

                unsigned int queuetoprocess = m_activeQueue;
                m_activeQueue = (m_activeQueue + 1) % MAX_NUMBER_OF_QUEUES;

                for ( list<MessagePtr>::iterator itmsg = m_messages[queuetoprocess].begin(); itmsg != m_messages[queuetoprocess].end(); itmsg++ )
                {
                    triggerMessage(*itmsg);
                }

                m_messages[queuetoprocess].clear();
            }
        }
    }
}

