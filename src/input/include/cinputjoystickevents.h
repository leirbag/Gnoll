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


/*-------------------------CInputJoystickEvents-----------------------------*\
|   Joystick events                                                          |
|                                                                            |
|                                                                            |
\*--------------------------------------------------------------------------*/



#include <OISJoyStick.h>


#ifndef __CINPUTJOYSTICKEVENTS_H__
#define __CINPUTJOYSTICKEVENTS_H__



namespace Gnoll
{
	namespace Input
	{
		enum JoystickEventType
		{
			Axis = 0,
			Button
		};


		struct JoystickEvent
		{

			/**
			 * Current state of the joystick
			 */
			const OIS::JoyStickEvent event;


			/**
			 * Which button or axis triggered this event
			 */
			int which;


			/**
			 * Type of event
			 */
			JoystickEventType type;


			/**
			 * Constructor
			 */
			JoystickEvent(const OIS::JoyStickEvent _event, int _which, JoystickEventType _type):
				event(_event), which(_which), type(_type)
			{
			}

		};

	};
};


#endif // __CINPUTJOYSTICKEVENTS_H__
