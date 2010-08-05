/**************************************************************************
*   Copyright (C) 2008 by Paf                                             *
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

#ifndef __KEYBOARDSTATETRANSLATOR_H__
#define __KEYBOARDSTATETRANSLATOR_H__

#include <boost/shared_ptr.hpp>

#include "../../dynamicobject/include/dynamicobject.h"
#include "../../core/messages/include/listener.h"
#include "../../core/include/message.h"

using namespace boost;
using namespace Gnoll::Core;
using namespace Gnoll::DynamicObject;

namespace Gnoll
{
	namespace Input
	{
		// This is translate keyboard events to action events
		class KeyboardStateTranslator : public Messages::Listener
		{
			public:
				/**
				 * This is a constructor
				 */
				KeyboardStateTranslator();

				/**
				 * This is a destructor
				 */
				virtual ~KeyboardStateTranslator();

				/**
				 * This method is called in order to process a message
				 * @param message The message this method will have to process
				 */
				virtual void handle(MessagePtr message);

			private:
				/**
				 * DynamicObject which contains a translation map for events from keyboard
				 */
				shared_ptr<Gnoll::DynamicObject::DynamicObject> m_keyboardEventTranslationMap;


				/**
				 * DynamicObject which contains keyboard config
				 */
				shared_ptr<Gnoll::DynamicObject::DynamicObject> m_keyboardConfig;


				/**
				 * Messages::MessageType for KeyUp messages
				 */
				Messages::MessageType m_keyUp;


				/**
				 * Messages::MessageType for KeyDown messages
				 */
				Messages::MessageType m_keyDown;
		};
	};
};

#endif
