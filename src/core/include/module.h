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

#ifndef __MODULE_H__
#define __MODULE_H__

namespace Gnoll
{
	namespace Core
	{
		/**
		 *	Interface of all game modules.
		 */
		class Module
		{
			public:
				/**
				 * This inits a module
				 */
				virtual void init() = 0;

				/**
				 * This lets a module to do its own business
				 */
				virtual void process() = 0;

				/**
				 * This is called on exiting
				 */
				virtual void exit() = 0;

				/**
				 * This is a virtual destructor
				 */
				virtual ~Module() {};
		};
	};
};

#endif 
