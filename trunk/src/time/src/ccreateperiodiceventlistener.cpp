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


/*----------------------CCreatePeriodicEventListener-----------------------*\
|   This is defines listeners used by the time module                       |
|                                                                           |
|   Changelog :                                                             |
|               09/20/2007 - Paf - Initial release                          |
|               09/23/2007 - Paf - Renamed to CCreatePeriodicEventListener  |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../include/ccreateperiodiceventlistener.h"



namespace Gnoll {

	namespace Core {

				
		CCreatePeriodicEventListener::CCreatePeriodicEventListener() 
		{
		}

		CCreatePeriodicEventListener::~CCreatePeriodicEventListener() 
		{
		}

		void CCreatePeriodicEventListener::handle ( shared_ptr<CMessage> message )
		{
		
			TimerPeriodicEvent timerEvent ( message->getData<TimerPeriodicEvent>());

			CTimeModule* timeModule = CTimeModule::getInstancePtr();

			timeModule->addPeriodicEvent(timerEvent.delay, timerEvent.message, timerEvent.period);
		}

	}
}



