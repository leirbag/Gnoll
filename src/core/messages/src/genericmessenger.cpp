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
#include "../include/listenercontainer.h"

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

				class Sender
				{
					public:
						Sender(GenericMessenger::MessagePtr & message) :
							m_message(message)
						{
							assert(message);
						}

						void operator()(ListenerContainer::ListenerPtr & listener)
						{
							listener->handle(m_message);
						}

					private:
						GenericMessenger::MessagePtr & m_message;
				};
			}

			GenericMessenger::GenericMessenger() :
				m_activeQueue(0)
			{
				m_listeners = new ListenerContainer();
			}

			GenericMessenger::~GenericMessenger()
			{
				delete m_listeners;
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
				if (!m_listeners->hasListenerForType(message->getType()))
				{
					throw Exceptions::NoOneIsListening();
				}
			}

			void GenericMessenger::addListener(ListenerPtr listener, const MessageType & messageType)
			{
				throwIfTypeNotValid(messageType);
				m_listeners->add(listener, messageType);
			}

			void GenericMessenger::delListener(ListenerPtr listener, const MessageType & messageType)
			{
				throwIfTypeNotValid(messageType);
				m_listeners->del(listener, messageType);
			}

			void GenericMessenger::triggerMessage(MessagePtr message)
			{
				throwIfMessageNotValid(message);
				throwIfNoListenerForMessage(message);

				boost::recursive_mutex::scoped_lock lock(m_listenersMutex);

				// TODO : MSG_ANYTYPE could be kept somewhere
				Details::Sender sendToListener(message);
				m_listeners->forEach(MessageType(MSG_ANYTYPE), sendToListener);
				m_listeners->forEach(message->getType(), sendToListener);
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

