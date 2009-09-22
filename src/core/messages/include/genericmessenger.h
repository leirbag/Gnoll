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

#ifndef __GENERICMESSENGER_H__
#define __GENERICMESSENGER_H__

#include "../include/messenger.h"
#include <boost/thread/recursive_mutex.hpp>
#include <set>
#include <map>
#include <list>

namespace Gnoll
{
	namespace Core
	{
		namespace Messages
		{
			class ListenerContainer;

			class GenericMessenger: public Messenger
			{
				public :
					GenericMessenger();
					virtual ~GenericMessenger();

					virtual void addListener(ListenerPtr listener, const MessageType & messagetype);
					virtual void delListener(ListenerPtr listener, const MessageType & messagetype);

					virtual void triggerMessage(MessagePtr message);
					virtual void queueMessage(MessagePtr message );

					virtual void abortFirstMessage(const MessageType & messageType);
					virtual void abortAllMessages(const MessageType & messageType);

					virtual void processQueue();

				private :
					void throwIfMessageNotValid(MessagePtr message);
					void throwIfTypeNotValid(const MessageType & type);
					void throwIfNoListenerForMessage(MessagePtr message);

					void addMessageToCurrentQueue(MessagePtr message);

					static const unsigned int MAX_NUMBER_OF_QUEUES = 2;

					boost::recursive_mutex m_listenersMutex;
                                        ListenerContainer * m_listeners;

					/**
					* Message queues are double buffered to avoid messaging loops.
					*/
					std::list< MessagePtr > m_messages[MAX_NUMBER_OF_QUEUES];
					boost::recursive_mutex m_messagesMutex[MAX_NUMBER_OF_QUEUES];

					unsigned int m_activeQueue;
					boost::recursive_mutex m_activeQueueMutex;
			};
		}
	}
}

#endif

