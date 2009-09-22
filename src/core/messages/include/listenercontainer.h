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

#ifndef __LISTENERCONTAINER_H__
#define __LISTENERCONTAINER_H__

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <map>

namespace Gnoll
{
	namespace Core
	{
		namespace Messages
		{
			class Listener;
			class MessageType;

			class ListenerContainer
			{
				public:
					typedef boost::shared_ptr<Listener> ListenerPtr;

					ListenerContainer();
					virtual ~ListenerContainer();

					void add(ListenerPtr listener, const MessageType & messageType);
					void del(ListenerPtr listener, const MessageType & messageType);

					bool isListenerAssociatedToType(ListenerPtr listener, const MessageType & messageType);

					void forEach(const MessageType & messageType, boost::function<void (ListenerPtr &)> function);

				private:
					typedef std::multimap<MessageType, ListenerPtr > ContainerType;

					ContainerType m_listeners;

					void throwIfAlreadyListeningToType(ListenerPtr listener, const MessageType & messageType);
					bool isAlreadyListeningToType(ListenerPtr listener, const MessageType & messageType);
					ContainerType::iterator getListenerIteratorForType(ListenerPtr listener, const MessageType & messageType);
			};
		}
  }
}

#endif

