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
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include "../include/messagetype.h"
#include "../include/listener.h"
#include "../../include/cmessage.h"
#include "../include/exceptions.h"

// TODO : name queue a list is inconsistent
// TODO : implementation is too big. Usage of the set is redundant with multimap key. To do after unittesting

namespace Gnoll
{
	namespace Core
	{
		namespace Messages
		{
			namespace Details
			{
				static bool isMessageOfType(Messenger::MessagePtr message, const MessageType & type)
				{
					return message->getType() == type;
				}

				static bool isListenerTypePair(const std::pair<MessageType, Messenger::ListenerPtr > & pair,
								Messenger::ListenerPtr listener,
								const MessageType & messageType)
				{
					return pair.first == messageType && pair.second == listener;
				}
			}

			GenericMessenger::GenericMessenger() :
				m_activeQueue(0)
			{
			}

			GenericMessenger::~GenericMessenger()
			{
			}

			void GenericMessenger::throwIfTypeNotValid(const MessageType & type)
			{
				if (type.getTypeStr().empty())
				{
					throw Exceptions::InvalidMessage();
				}
			}

			void GenericMessenger::throwIfMessageNotValid(MessagePtr message)
			{
				throwIfTypeNotValid(message->getType());
			}

			void GenericMessenger::throwIfNoListenerForMessage(MessagePtr message)
			{
				if (!hasListenerForMessageType(message->getType()))
				{
					throw Exceptions::NoOneIsListening();
				}
			}

			GenericMessenger::ListenerContainer::iterator GenericMessenger::getListenerIteratorForType(ListenerPtr listener, const MessageType & messageType)
			{
				ListenerContainer::iterator it = m_listeners.lower_bound(messageType);
				ListenerContainer::iterator itEnd = m_listeners.upper_bound(messageType);

				ListenerContainer::iterator foundListener = std::find_if(it, itEnd,
								boost::bind(Details::isListenerTypePair, _1, listener, messageType));

				if (foundListener == itEnd)
				{
					return ListenerContainer::iterator();
				}

				return foundListener;
			}

			bool GenericMessenger::hasListenerForMessageType(const MessageType & messageType) const
			{
				ListenerContainer::const_iterator result = m_listeners.find(messageType);

				return result != m_listeners.end();
			}

			bool GenericMessenger::isAlreadyListeningToType(ListenerPtr listener, const MessageType & messageType)
			{
				ListenerContainer::iterator it(getListenerIteratorForType(listener, messageType));

				return it != ListenerContainer::iterator();
			}

			void GenericMessenger::throwIfAlreadyListeningToType(ListenerPtr listener, const MessageType & messageType)
			{
				if (isAlreadyListeningToType(listener, messageType))
				{
					throw Exceptions::HandlerAlreadyRegistered();
				}
			}

			void GenericMessenger::addListener(ListenerPtr listener, const MessageType & messageType)
			{
				throwIfTypeNotValid(messageType);
				throwIfAlreadyListeningToType(listener, messageType);

				m_listeners.insert(std::pair<MessageType, ListenerPtr >(messageType, listener));
			}

			void GenericMessenger::eraseListenerFromContainer(ListenerPtr listener, const MessageType & messageType)
			{
				boost::recursive_mutex::scoped_lock lock(m_listenersMutex);

				ListenerContainer::iterator it(getListenerIteratorForType(listener, messageType));

				if (it == ListenerContainer::iterator())
				{
					throw Exceptions::CannotDeleteListener();
				}
				else
				{
					m_listeners.erase(it);
				}
			}

			void GenericMessenger::delListener(ListenerPtr listener, const MessageType & messageType)
			{
				throwIfTypeNotValid(messageType);
				eraseListenerFromContainer(listener, messageType);
			}

			void GenericMessenger::sendToListenersOfType(MessagePtr message, const MessageType & type)
			{
				ListenerContainer::iterator itEnd = m_listeners.upper_bound(type);

				for (ListenerContainer::iterator it = m_listeners.lower_bound(type); it != itEnd; ++it)
				{
					((*it).second)->handle(message);
				}
			}

			void GenericMessenger::triggerMessage(MessagePtr message)
			{
				throwIfMessageNotValid(message);
				throwIfNoListenerForMessage(message);

				boost::recursive_mutex::scoped_lock lock(m_listenersMutex);

				sendToListenersOfType(message, MessageType(MSG_ANYTYPE));
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
				throwIfMessageNotValid(message);
				throwIfNoListenerForMessage(message);

				addMessageToCurrentQueue(message);
			}

			void GenericMessenger::abortFirstMessage(const MessageType & messageType)
			{
				throwIfTypeNotValid(messageType);
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
				throwIfTypeNotValid(messageType);
				boost::recursive_mutex::scoped_lock lockAQ(m_activeQueueMutex);
				boost::recursive_mutex::scoped_lock lockMsg(m_messagesMutex[m_activeQueue]);

				list<MessagePtr> & messages = m_messages[m_activeQueue];

				messages.remove_if(boost::bind(Details::isMessageOfType, _1, messageType));
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

