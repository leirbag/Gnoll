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

#ifndef __MESSENGER_H__
#define __MESSENGER_H__

namespace boost
{
    template<class T>
    class shared_ptr;
}

namespace Gnoll
{
    namespace Core
    {
        class CMessage;

        namespace Messages
        {
            class MessageType;
            class Listener;

            /** Interface for message managers
            *
            * A message manager is in charge distributing messages to corresponding listeners.
            */
            class Messenger
            {
                public:
                    typedef boost::shared_ptr<Listener> ListenerPtr;
                    typedef boost::shared_ptr<CMessage> MessagePtr;

                    // Messenger();
                    virtual ~Messenger() {};

                    virtual void addListener(ListenerPtr listener, const MessageType & messagetype) = 0;
                    virtual void delListener(ListenerPtr listener, const MessageType & messagetype) = 0;

                    virtual void triggerMessage(MessagePtr message) = 0;
                    virtual void queueMessage(MessagePtr message ) = 0;
                    virtual void abortFirstMessage(const MessageType & messagetype) = 0;
                    virtual void abortAllMessages(const MessageType & messagetype) = 0;

                    virtual bool isMessageTypeValid(const MessageType & messagetype) const = 0;

                    virtual void processQueue() = 0;
            };
        }
    }
}

#endif

