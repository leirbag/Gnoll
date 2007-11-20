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


/*------------------------CGenericMessageManager---------------------------*\
|   This is a generic message manager                                       |
|                                                                           |
|   Changelog :                                                             |
|               05/15/2006 - Paf - Initial release                          |
|               11/16/2007 - Paf - Remove Singleton<> inheritance           |
|               11/19/2007 - Paf - Add a mutex                              |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <string>
#include <set>
#include <map>
#include <list>

#include "../include/cmessagemanager.h"
#include "../include/cmessagetype.h"
#include "../include/cmessagelistener.h"
#include "../include/cmessage.h"
#include "../include/singleton.h"


#ifndef __CGENERICMESSAGEMANAGER_H__
#define __CGENERICMESSAGEMANAGER_H__

using namespace std;
using namespace boost;


/**
 *	A generic message manager.
 *	This is an implementation of a messages manager
 */ 
class CGenericMessageManager: public CMessageManager
{
	private :

		/**
		 * Mutex
		 */
		boost::mutex m_mutex;

		/**
		 * A set of message types
		 */
		set<CMessageType> m_messagetypes;  

		/**
		 * A map which holds the listners related to a message type
		 */
		multimap< CMessageType, shared_ptr<CMessageListener> > m_listeners;

		/**
		 * There is two lists of messages. Only one is active at a time.<br>
		 * There is two lists of message in order to blocks infinites messages
		 * (a message which create another message and so on...)
		 */
		list< shared_ptr<CMessage> > m_messages[2];

		/**
		 * The index of the active message queue
		 */
		unsigned int m_activequeue;

		/**
		 * Total number of message queues
 		*/
		const unsigned int NUMQUEUE;



	public :
		
		/**
		 * This is a constructor
		 */
		CGenericMessageManager(): m_activequeue(0), NUMQUEUE(2) {}

		/**
		 * This is a destructor
		 */
		virtual ~CGenericMessageManager() {}

		/**
		 * This adds a new listener
		 * @param handler The listener which handle messages
		 * @param messagetype The listener will care about every message of this message type
		 * @return True if ok, false otherwise 
		 */
		virtual bool addListener ( shared_ptr<CMessageListener> handler, CMessageType messagetype );

		/**
		 * This deletes a new listener
		 * @param handler The listener which handle messages
		 * @param messagetype The listener which have cared about every message of this message type
		 * @return True if ok, false otherwise 
		 */
		virtual bool delListener ( shared_ptr<CMessageListener> handler, CMessageType messagetype );

		/**
		 * This process a given message as parameter
		 * @param message The message to be processed
		 * @return True if ok, false otherwise 
		 */
		virtual bool trigger ( shared_ptr<CMessage> message );

		/**
		 * This enqueue a message
		 * @param message The message to be processed
		 * @return True if ok, false otherwise 
		 */
		virtual bool queueMessage ( shared_ptr<CMessage> message );

		/**
		 * This abort the first message or every message of type messagetype
		 * @param messagetype The message's or messages' type to be aborted
		 * @param alloftype Are every messages of type messagetype aborted ? (default : False)<br> True -> Yes <br> False -> No (only the first one is aborted)
		 * @return True if ok, false otherwise 
		 */
		virtual bool abortMessage ( CMessageType messagetype, bool alloftype = false );

		/**
		 * This validate a message type
		 * @param messagetype The message type to be validated
		 * @return True if ok, false otherwise 
		 */
		virtual bool validateType( CMessageType messagetype );

		/**
		 * This process the message queue
		 */
		virtual void process( );

};

#endif // __CGENERICMESSAGEMANAGER_H__
