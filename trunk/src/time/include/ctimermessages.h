/***************************************************************************
 *   Copyright (C) 2007 by Paf                                             *
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


/*------------------------------ctimermessages-----------------------------*\
|   This is a messages definition for the timer module                      |
|                                                                           |
|   Changelog :                                                             |
|               09/14/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../../core/include/cmessage.h"
#include <boost/shared_ptr.hpp>


#ifndef __CTIMERMESSAGES_H__
#define __CTIMERMESSAGES_H__

using namespace boost;


namespace Gnoll
{
	namespace Core
	{

		/**
		 * This defines a one-time event
		 */
		typedef struct _TimerEvent 
		{

			/**
			 * How long before this event occurs
			 */
			unsigned long int delay;

			/**
			 * Message to be sent when the event occurs
			 */
			shared_ptr<CMessage> message;

		} TimerEvent;


		/**
		 * This defines a periodic event
		 */
		typedef struct _TimerPeriodicEvent 
		{

			/**
			 * How long before this periodic event begins
			 */
			unsigned long int delay;

			/**
			 * How often this event occurs
			 */ 
			unsigned long int period;

			/**
			 * Message to be sent when the event occurs
			 */
			shared_ptr<CMessage> message;

		} TimerPeriodicEvent;

	}
}

#endif // __CTIMERMESSAGES_H__
