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

#ifndef __CDESTROYPERIODICEVENTLISTENER_H__
#define __CDESTROYPERIODICEVENTLISTENER_H__ 

#include <string>
#include <boost/shared_ptr.hpp>

#include "../../core/messages/include/listener.h"
#include "ctimermessages.h"
#include "ctimemodule.h"

using namespace std;
using namespace boost;
using namespace Gnoll::Core;

namespace Gnoll
{
	namespace Time
	{
		/**
		*	A message listener for the timer module.
		*	This listener destroy registered periodic events
		*/ 
		class CDestroyPeriodicEventListener : public Messages::Listener
		{
			public:
				
				/**
				* This is a constructor
				*/
				CDestroyPeriodicEventListener(); 

				/**
				* This is a destructor
				*/
				virtual ~CDestroyPeriodicEventListener();

				/**
				* This method is called in order to process a message
				* @param message The message this method will have to process
				*/
				virtual void handle(MessagePtr message);
			};
	}
}

#endif // __CDESTROYPERIODICEVENTLISTENER_H__
