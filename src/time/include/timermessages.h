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

#ifndef __CTIMERMESSAGES_H__
#define __CTIMERMESSAGES_H__

#include <boost/shared_ptr.hpp>

#include "../../core/include/message.h"

using namespace boost;
using namespace Gnoll::Core;

namespace Gnoll
{
	namespace Time
	{
		/**
		 * This defines a one-time event
		 */
		struct TimerEvent 
		{
			/**
			 * How long before this event occurs
			 */
			unsigned long int delay;

			/**
			 * Message to be sent when the event occurs
			 */
			shared_ptr<Message> message;
		};

		/**
		 * This defines a periodic event
		 */
		struct TimerPeriodicEvent 
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
			shared_ptr<Message> message;
		};
	}
}

#endif
