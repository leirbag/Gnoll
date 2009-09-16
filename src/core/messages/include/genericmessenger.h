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

#include "../include/messenger.h"
#include <boost/thread/recursive_mutex.hpp>
#include <set>
#include <map>
#include <list>

#ifndef __GENERICMESSENGER_H__
#define __GENERICMESSENGER_H__

namespace Gnoll
{
    namespace Core
    {
        namespace Messages
        {
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

                    // TODO : why is it public virtual ? Should be private.

                    virtual void processQueue();

                private :
                    typedef std::multimap<MessageType, ListenerPtr > ListenerContainer;
                    ListenerContainer m_listeners;

                    virtual bool isMessageTypeValid(const MessageType & messagetype) const;

                    bool isMessageTypeRegistered(const MessageType & messageType) const;
                    bool isAlreadyListeningToType(ListenerPtr listener, const MessageType & messageType);
                    ListenerContainer::iterator getListenerIteratorForType(ListenerPtr listener, const MessageType & messageType);

                    void registerMessageType(const MessageType & messageType);
                    void eraseListenerFromContainer(ListenerPtr listener, const MessageType & messageType);
                    void eraseMessageTypeOrphan(const MessageType & messageType);

                    bool hasListenerForMessageType(const MessageType & messageType) const;

                    void sendToListenersOfType(MessagePtr message, const MessageType & type);
                    void addMessageToCurrentQueue(MessagePtr message);

                    void checkMessageValidity(MessagePtr message);
                    void checkMessageTypeValidity(const MessageType & type);

                    static const unsigned int MAX_NUMBER_OF_QUEUES = 2;

                    // TODO : check necessity for this
                    std::set<MessageType> m_messageTypes;
                    boost::recursive_mutex m_messageTypesMutex;

                    boost::recursive_mutex m_listenersMutex;

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

