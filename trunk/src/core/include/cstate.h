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


/*--------------------------------cstate.h---------------------------------*\
|   Interface of all the FSM's states                                       |
|                                                                           |
|   Changelog :                                                             |
|               04/27/2006 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/

#ifndef __CSTATE_H__
#define __CSTATE_H__

/**
 *	Interface of all the FSM's states. <br>A state is a description of an activity.
 */ 
class CState
{
	public:
		/**
		 * This is called after entering this state
		 */
		virtual void onInit() = 0;

		/**
		 * This is called during its activation
		 */
		virtual void onProcess() = 0;

		/**
		 * This is called before exiting this state
		 */
		virtual void onExit() = 0;

		/**
		 * This is a virtual destructor
		 */
		virtual ~CState() {};
};

#endif // __CSTATE_H__
