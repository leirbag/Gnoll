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

#ifndef __LISTENER_H__
#define __LISTENER_H__

#include <string>
#include <boost/shared_ptr.hpp>
#include "../../include/cmessage.h"

namespace Gnoll
{
	namespace Core
	{
		namespace Messages
		{
			/// Listener to messages interface.
			class Listener
			{
				public:
					typedef boost::shared_ptr<CMessage> MessagePtr;

					// Listener();
					virtual ~Listener() {}

					// TODO : I don't like "handle". Change to process later
					virtual void handle(MessagePtr receivedMessage) = 0;
			};
		}
	}
}

#endif

