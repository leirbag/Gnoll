/**************************************************************************
*   Copyright (C) 2008 by Bruno Mahe                                      *
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


/*-----------------------CMouseButtonEventsTrigger-------------------------*\
|   This is translate mouse button events to action events                  |
|                                                                           |
|   Changelog :                                                             |
|               16/05/2008 - WT - Initial release     		            |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include "../include/cmousebuttoneventstranslator.h"
#include "../include/cmousebuttoneventstrigger.h"
#include "../include/ctranslationevents.h"



using namespace boost;
using namespace Gnoll::Core;


namespace Gnoll
{
	namespace Input
	{

		CMouseButtonEventsTrigger::CMouseButtonEventsTrigger( shared_ptr<CMouseButtonEventsTranslator> _cMouseButtonEventsTranslator):
			m_cMouseButtonEventsTranslator(_cMouseButtonEventsTranslator)
		{

		}

		CMouseButtonEventsTrigger::~CMouseButtonEventsTrigger()
		{
		}

		void CMouseButtonEventsTrigger::handle ( shared_ptr<CMessage> message )
		{
			m_cMouseButtonEventsTranslator->trigger( message );
		}
	};
};
