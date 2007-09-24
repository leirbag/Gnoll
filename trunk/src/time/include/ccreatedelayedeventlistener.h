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


/*-----------------------CCreateDelayedEventListener-----------------------*\
|   This is a message listener for the timer module                         |
|                                                                           |
|   Changelog :                                                             |
|               09/12/2007 - Paf - Initial release                          |
|               09/23/2007 - Paf - Renamed to CCreateDelayedEventListener   |
|                                                                           |
\*-------------------------------------------------------------------------*/



#ifndef __CCREATEDELAYEDEVENTLISTENER_H__
#define __CCREATEDELAYEDEVENTLISTENER_H__ 



#include <string>
#include <boost/shared_ptr.hpp>

#include "../../core/include/cmessagelistener.h"
#include "ctimermessages.h"

#include "ctimemodule.h"


using namespace boost;


namespace Gnoll
{
	namespace Core
	{

		/**
		*	A message listener for the timer module.
		*/ 
		class CCreateDelayedEventListener : public CMessageListener
		{

			public:
				
				/**
				* This is a constructor
				*/
				CCreateDelayedEventListener(); 

				/**
				* This is a destructor
				*/
				virtual ~CCreateDelayedEventListener();

				/**
				* This method is called in order to process a message
				* @param message The message this method will have to process
				*/
				virtual void handle ( shared_ptr<CMessage> message );
			};
	}
}

#endif // __CCREATEDELAYEDEVENTLISTENER_H__
