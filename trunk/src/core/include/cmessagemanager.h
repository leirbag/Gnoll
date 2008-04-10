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


/*----------------------------cmessagemanager------------------------------*\
|   This is a message manager                                               |
|                                                                           |
|   Changelog :                                                             |
|               05/15/2006 - Paf - Initial release                          |
|               04/10/2006 - Gabriel - Add namespace Gnoll and Core         |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include <string>

#include "cmessagetype.h"
#include "cmessagelistener.h"

#ifndef __CMESSAGEMANAGER_H__
#define __CMESSAGEMANAGER_H__

using namespace std;

namespace Gnoll
{
	namespace Core
	{
		/**
		 *	A message manager.
		 *	This is a base implementation for all of the messages manager
		 */
		class CMessageManager
		{

			public:

				/**
				 * This is a constructor
				 */
				CMessageManager() {}

				/**
				 * This is a destructor
				 */
				virtual ~CMessageManager() {}


				/**
				 * This adds a new listener
				 * @param handler The listener which handle messages
				 * @param messagetype The listener will care about every message of this message type
				 * @return True if ok, false otherwise
				 */
				virtual bool addListener ( shared_ptr<CMessageListener> handler, CMessageType messagetype ) = 0;

				/**
				 * This deletes a new listener
				 * @param handler The listener which handle messages
				 * @param messagetype The listener which have cared about every message of this message type
				 * @return True if ok, false otherwise
				 */
				virtual bool delListener ( shared_ptr<CMessageListener> handler, CMessageType messagetype ) = 0;

				/**
				 * This process a given message as parameter
				 * @param message The message to be processed
				 * @return True if ok, false otherwise
				 */
				virtual bool trigger ( shared_ptr<CMessage> message ) = 0;

				/**
				 * This enqueue a message
				 * @param message The message to be processed
				 * @return True if ok, false otherwise
				 */
				virtual bool queueMessage ( shared_ptr<CMessage> message ) = 0;

				/**
				 * This abort the first message or every message of type messagetype
				 * @param messagetype The message's or messages' type to be aborted
				 * @param alloftype Are every messages of type messagetype aborted ? (default : False)<br> True -> Yes <br> False -> No (only the first one is aborted)
				 * @return True if ok, false otherwise
				 */
				virtual bool abortMessage ( CMessageType messagetype, bool alloftype = false ) = 0;

				/**
				 * This validate a message type
				 * @param messagetype The message type to be validated
				 * @return True if ok, false otherwise
				 */
				virtual bool validateType( CMessageType messagetype ) = 0;

				/**
				 * This process the message queue
				 */
				virtual void process( ) = 0;


		};
	};
};

#endif // __CMESSAGEMANAGER_H__
