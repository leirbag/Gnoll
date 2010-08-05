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

#ifndef __KEYBOARDEVENTSTRIGGER_H__
#define __KEYBOARDEVENTSTRIGGER_H__

#include <boost/shared_ptr.hpp>

#include "../../core/messages/include/listener.h"
#include "../../core/include/message.h"

using namespace boost;

namespace Gnoll
{
	namespace Input
	{
		/// This is translate keyboard events to action events
		class KeyboardEventsTrigger : public Messages::Listener
		{
			public:
				/**
				* This is a constructor
				*/
				KeyboardEventsTrigger(shared_ptr<KeyboardEventsTranslator> _cKeyboardEventsTranslator);

				/**
				* This is a destructor
				*/
				virtual ~KeyboardEventsTrigger();

				/**
				* This method is called in order to process a message
				* @param message The message this method will have to process
				*/
				virtual void handle(MessagePtr message);

			private:
				shared_ptr<KeyboardEventsTranslator> m_cKeyboardEventsTranslator;
		};
	}
}

#endif
