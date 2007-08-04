/***************************************************************************
 *   Copyright (C) 2006 by Puzzle Team                                     *
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


/*-------------------------cinputmouseevents.h------------------------------*\
|   Mouse events                                                             |
|                                                                            |
|   Changelog :                                                              |
|               10/20/2006 - Paf - Initial release                           |
|                                                                            |
\*--------------------------------------------------------------------------*/


#ifndef __CINPUTMOUSEEVENTS_H__
#define __CINPUTMOUSEEVENTS_H__


const unsigned int MAX_MOUSE_BUTTONS = 8;

enum MouseButton
{
	Left = 0, Right, Middle,
	Button3, Button4,	Button5, Button6,	Button7
};


struct MouseEvent
{
	int abX, abY, abZ, relX, relY, relZ;

	// Indicates if this Axis only supports Absolute (ie JoyStick)
	bool absOnly;

	MouseEvent(int _abX = 0, int _abY = 0, int _abZ = 0,
				  int _relX = 0, int _relY = 0, int _relZ = 0,
				  bool _absOnly = 0) 
	{
		abX = _abX;
		abY = _abY;
		abZ = _abZ;
		
		relX = _relX;
		relY = _relY;
		relZ = _relZ;

		absOnly = _absOnly;
	}

};


#endif // __CINPUTMOUSEEVENTS_H__
