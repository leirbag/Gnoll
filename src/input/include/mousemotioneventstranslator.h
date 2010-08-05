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

#ifndef __MOUSEMOTIONEVENTSTRANSLATOR_H__
#define __MOUSEMOTIONEVENTSTRANSLATOR_H__

#include <boost/shared_ptr.hpp>

#include "../../dynamicobject/include/dynamicobject.h"
#include "../../core/messages/include/listener.h"
#include "../../core/include/message.h"

#include "inputmouseevents.h"

using namespace boost;
using namespace Gnoll::Core;
using namespace Gnoll::DynamicObject;

namespace Gnoll
{
	namespace Input 
	{
		/// This is translate mouse motion events to action events
		class MouseMotionEventsTranslator : public Messages::Listener
		{
			public:
				/**
				 * This is a constructor
				 */
				MouseMotionEventsTranslator();

				/**
				 * This is a destructor
				 */
				virtual ~MouseMotionEventsTranslator(); 

				/**
				 * This method is called in order to process a message
				 * @param message The message this method will have to process
				 */
				virtual void handle(MessagePtr message);

			private:
				/**
				 * Create an INPUT_ACTION_EVENT message from an event type and its intensity
				 * @param _event event type
				 * @param _intensity Intensity of the event
				 */
				void sendActionEventForEventAndIntensity(string _event, float _intensity);

				/**
				 * Extract X mouse motion and convert it to an INPUT_ACTION_EVENT
				 * @param _mouseEvent Contains mouse motion information
				 */
				void sendXMotionEvents(MouseEvent _mouseEvent);

				/**
				 * Extract Y mouse motion and convert it to an INPUT_ACTION_EVENT
				 * @param _mouseEvent Contains mouse motion information
				 */
				void sendYMotionEvents(MouseEvent _mouseEvent);

				/**
				 * Extract Z mouse motion and convert it to an INPUT_ACTION_EVENT
				 * @param _mouseEvent Contains mouse motion information
				 */
				void sendZMotionEvents(MouseEvent _mouseEvent);

			private:
				/**
				 * DynamicObject which contains a translation map for events from mouse
				 */
				shared_ptr<Gnoll::DynamicObject::DynamicObject> m_mouseMotionEventTranslationMap;

				/**
				 * DynamicObject which contains mouse config
				 */
				shared_ptr<Gnoll::DynamicObject::DynamicObject> m_mouseConfig;

				/**
				 * Messages::MessageType for MOUSE_MOVED messages
				 */
				Messages::MessageType m_mouseMoved;

				/**
				 * Maximum X coordinate for the mouse
				 */
				float m_maxX;

				/**
				 * Maximum Y coordinate for the mouse
				 */
				float m_maxY;
		};
	};
};

#endif
