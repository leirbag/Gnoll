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

#ifndef __INPUTMODULE_H__
#define __INPUTMODULE_H__

#include "../../core/include/module.h"

using namespace Gnoll::Core;

/**
 *	Interface of all game modules. 
 */ 
class InputModule: public Module
{
	public:
		/**
		 * @copydoc Module::init
		 */
		virtual void init() = 0;

		/**
		 * @copydoc Module::process
		 */
		virtual void process() = 0;

		/**
		 * @copydoc Module::exit 
		 */
		virtual void exit() = 0;

		/**
		 * @copydoc Module::~Module
		 */
		virtual ~InputModule() {};
};

#endif
