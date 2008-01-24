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


/*---------------------------cmessagelistener------------------------------*\
|   This is a message's listener                                            |
|                                                                           |
|   Changelog :                                                             |
|               05/15/2006 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include <string>
#include <boost/shared_ptr.hpp>

#include "../include/cmessage.h"

#ifndef __CMESSAGELISTENER_H__
#define __CMESSAGELISTENER_H__

using namespace std;
using namespace boost;


/**
 *	A message listener.
 *	This is a base implementation for all of the message's handler
 */ 
class CMessageListener
{

	public:
		
		/**
		 * This is a constructor
		 */
		CMessageListener() {}

		/**
		 * This is a destructor
		 */
		virtual ~CMessageListener() {}

		/**
		 * This method is called in order to process a message
		 * @param message The message this method will have to process
		 */
		virtual void handle ( shared_ptr<CMessage> message ) = 0;


};

#endif // __CMESSAGELISTENER_H__
