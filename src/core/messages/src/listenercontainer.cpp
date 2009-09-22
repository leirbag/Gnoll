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

#include "../include/listenercontainer.h"
#include "../include/messagetype.h"
#include "../include/exceptions.h"
#include <boost/bind.hpp>

#include <iostream>

namespace Gnoll
{
	namespace Core
	{
		namespace Messages
		{
			namespace Details
			{
				static bool isListenerTypePair(const std::pair<MessageType, ListenerContainer::ListenerPtr > & pair,
								ListenerContainer::ListenerPtr listener,
								const MessageType & messageType)
				{
					return pair.first == messageType && pair.second == listener;
				}
			}

                        // TODO : put concurrency back to container
			ListenerContainer::ListenerContainer()
			{
			}

			ListenerContainer::~ListenerContainer()
			{
			}

			ListenerContainer::ContainerType::iterator ListenerContainer::getListenerIteratorForType(ListenerPtr listener, const MessageType & messageType)
			{
				ContainerType::iterator it = m_listeners.lower_bound(messageType);
				ContainerType::iterator itEnd = m_listeners.upper_bound(messageType);

				ContainerType::iterator foundListener = std::find_if(it, itEnd,
								boost::bind(Details::isListenerTypePair, _1, listener, messageType));

				if (foundListener == itEnd)
				{
					return ContainerType::iterator();
				}

				return foundListener;
			}

			bool ListenerContainer::isAlreadyListeningToType(ListenerPtr listener, const MessageType & messageType)
			{
				ContainerType::iterator it(getListenerIteratorForType(listener, messageType));

				return it != ContainerType::iterator();
			}

			void ListenerContainer::throwIfAlreadyListeningToType(ListenerPtr listener, const MessageType & messageType)
			{
				if (isAlreadyListeningToType(listener, messageType))
				{
					throw Exceptions::HandlerAlreadyRegistered();
				}
			}

			void ListenerContainer::add(ListenerPtr listener, const MessageType & messageType)
			{
				throwIfAlreadyListeningToType(listener, messageType);

				m_listeners.insert(std::pair<MessageType, ListenerPtr >(messageType, listener));
			}

			void ListenerContainer::del(ListenerPtr listener, const MessageType & messageType)
			{
				ContainerType::iterator it(getListenerIteratorForType(listener, messageType));

				if (it == ContainerType::iterator())
				{
					throw Exceptions::CannotDeleteListener();
				}
				else
				{
					m_listeners.erase(it);
				}
			}

			bool ListenerContainer::isListenerAssociatedToType(ListenerPtr listener, const MessageType & messageType)
			{
				ContainerType::iterator it(getListenerIteratorForType(listener, messageType));

				return it != ContainerType::iterator();
			}

			bool ListenerContainer::hasListenerForType(const MessageType & messageType)
			{
				ContainerType::iterator it = m_listeners.lower_bound(messageType);

				return it != m_listeners.end();
			}

			void ListenerContainer::forEach(const MessageType & messageType, ForEachFunction function)
			{
				ContainerType::iterator itEnd = m_listeners.upper_bound(messageType);

				for (ContainerType::iterator it = m_listeners.lower_bound(messageType); it != itEnd; ++it)
				{
					function((*it).second);
				}
			}

		}
	}
}

