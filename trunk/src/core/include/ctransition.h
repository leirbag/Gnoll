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


/*-----------------------------ctransition.h-------------------------------*\
|   Interface of all the FSM's transitions                                  |
|                                                                           |
|   Changelog :                                                             |
|               04/27/2006 - Paf - Initial release                          |
|               07/10/2007 - Paf - Virtual destructor added                 |
|                                                                           |
\*-------------------------------------------------------------------------*/

#ifndef __CTRANSITION_H__
#define __CTRANSITION_H__

/**
 *	Interface of all the FSM's transitions
 */ 
class CTransition
{
	public:

		/**
		 * This is a destructor
		 */
		virtual ~CTransition() {};

		/**
		 * This will check if this transition is valid, or not.
		 * @return The validation test's result
		 */		
		virtual bool isValid() = 0;

};

#endif // __CTRANSITION_H__
