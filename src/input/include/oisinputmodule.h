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

#ifndef __OISINPUTMODULE_H__
#define __OISINPUTMODULE_H__

#include <string>

#include <boost/shared_ptr.hpp>
#include <OgreRenderWindow.h>

#include "inputmodule.h"
#include "oisinputmanager.h"
#include "../../core/include/messagemodule.h"

using namespace std;
using namespace boost;

/**
 *	The game module that use OIS. 
 */ 
class OISInputModule
{
	public:
		/**
		 * A constructor
		 */
		OISInputModule();

		/**
		 * @copydoc Module::init
		 */
		virtual void init();

		/**
		 * @copydoc Module::process
		 */
		virtual void process();

		/**
		 * @copydoc Module::exit 
		 */
		virtual void exit();

		/**
		 * @copydoc Module::~Module
		 */
		virtual ~OISInputModule();

	private:
		/**
		 * The events which will be processed
		 */
		OISInputManager* m_inputmanager;
};

#endif
