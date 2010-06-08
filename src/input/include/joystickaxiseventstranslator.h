/**************************************************************************
*   Copyright (C) 2009 by Bruno Mahe                                      *
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


#ifndef __JOYSTICKAXISEVENTSTRANSLATOR_H__
#define __JOYSTICKAXISEVENTSTRANSLATOR_H__

#include <boost/shared_ptr.hpp>

#include "../../dynamicobject/include/dynamicobject.h"
#include "../../core/messages/include/listener.h"
#include "../../core/include/message.h"

#include "cinputjoystickevents.h"

using namespace boost;
using namespace Gnoll::Core;
using namespace Gnoll::DynamicObject;

namespace Gnoll
{
	namespace Input
	{
		/// This is translate joystick axis events to action events
		class JoystickAxisEventsTranslator : public Messages::Listener
		{
			private:

				/**
				 * DynamicObject which contains a translation map for events from joystick
				 */
				shared_ptr<Gnoll::DynamicObject::DynamicObject> joystickAxisEventTranslationMap;


				/**
				 * DynamicObject which contains joystick config
				 */
				shared_ptr<Gnoll::DynamicObject::DynamicObject> joystickConfig;


				/**
				 * Messages::MessageType for MOUSE_MOVED messages
				 */
				Messages::MessageType axisMoved;


				/**
				 * Previous values for each axis
				 */
				shared_ptr< std::vector<int> > previousAxisValues;


				/**
				 * Create an INPUT_ACTION_EVENT message from an event type and its intensity
				 * @param _event event type
				 * @param _intensity Intensity of the event
				 */
				void sendActionEventForEventAndIntensity(string _event, float _intensity);


				/**
				 * Initialize previousAxisValue with the required number of axis
				 * @param _numAxis Number of required axis
				 */
				void initPreviousAxisValues(unsigned int _numAxis);


			public:

				/**
				* This is a constructor
				*/
				JoystickAxisEventsTranslator();

				/**
				* This is a destructor
				*/
				virtual ~JoystickAxisEventsTranslator();

				/**
				* This method is called in order to process a message
				* @param message The message this method will have to process
				*/
				virtual void handle(MessagePtr message);
		};
	};
};

#endif

