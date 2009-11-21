/***************************************************************************
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


/*---------------------------OISJoystickListener----------------------------*\
|   Joystick events listener for OIS                                         |
|                                                                            |
|                                                                            |
\*--------------------------------------------------------------------------*/

#include <OISJoyStick.h>
#include <OISInputManager.h>

#include <OgreRenderWindow.h>

#include <boost/any.hpp>

using boost::any_cast;


#ifndef __OISJOYSTICKLISTENER_H__
#define __OISJOYSTICKLISTENER_H__

namespace Gnoll
{
	namespace Input
	{

		class OISJoystickListener : public OIS::JoyStickListener
		{
			public:

				/**
				 * Returns Message type when a joystick's button is pressed
				 * @return Message type constant for joystick's button event
				 */
				inline static const char * MESSAGE_TYPE_JOYSTICK_BUTTON_PRESSED() {return "JOYSTICK_BUTTON_PRESSED";}


				/**
				 * Returns Message type when a joystick's button is pressed
				 * @return Message type constant for joystick's button event
				 */
				inline static const char * MESSAGE_TYPE_JOYSTICK_BUTTON_RELEASED() {return "JOYSTICK_BUTTON_RELEASED";}


				/**
				 * Returns Message type when a joystick's axis is moved
				 * @return Message type constant for joystick's axis event
				 */
				inline static const char * MESSAGE_TYPE_JOYSTICK_AXIS_MOVED() {return "JOYSTICK_AXIS_MOVED";}


				/**
				 * Default constructor
				 */
				OISJoystickListener();


				/**
				 * Default destructor
				 */
				virtual ~OISJoystickListener(void);


				/**
				 * Listener called whenever a button of a joystick is pressed
				 * @param arg data structure describing the state of the joystick
				 * @param button ID of the button being pressed
				 * @return Status
				 */
				bool buttonPressed( const OIS::JoyStickEvent &arg, int button );


				/**
				 * Listener called whenever a button of a joystick is released
				 * @param arg data structure describing the state of the joystick
				 * @param button ID of the button being released
				 * @return Status
				 */
				bool buttonReleased( const OIS::JoyStickEvent &arg, int button );


				/**
				 * Listener called whenever an axis of a joystick is moved
				 * @param arg data structure describing the state of the joystick
				 * @param axis ID of the axis being moved
				 * @return Status
				 */
				bool axisMoved( const OIS::JoyStickEvent &arg, int axis );

		};
	}
}

#endif // __OISJOYSTICKLISTENER_H__
