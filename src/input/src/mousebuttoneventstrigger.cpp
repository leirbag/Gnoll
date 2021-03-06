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

#include "../include/mousebuttoneventstranslator.h"

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include "../include/mousebuttoneventstrigger.h"
#include "../include/translationevents.h"

using namespace boost;
using namespace Gnoll::Core;

namespace Gnoll
{
	namespace Input
	{
		MouseButtonEventsTrigger::MouseButtonEventsTrigger(shared_ptr<MouseButtonEventsTranslator> _cMouseButtonEventsTranslator):
			m_cMouseButtonEventsTranslator(_cMouseButtonEventsTranslator)
		{
		}

		MouseButtonEventsTrigger::~MouseButtonEventsTrigger()
		{
		}

		void MouseButtonEventsTrigger::handle(shared_ptr<Message> message)
		{
			m_cMouseButtonEventsTranslator->trigger(message);
		}
	};
};
