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


/*---------------------------coisinputmodule.h------------------------------*\
|   The game input module that use SDL                                       |
|                                                                            |
|   Changelog :                                                              |
|               06/23/2006 - Paf - Initial release                           |
|                                                                            |
\*--------------------------------------------------------------------------*/

#include "cinputmodule.h"
#include "coisinputmanager.h"
#include "../../core/include/cgenericmessagemanager.h"
#include <boost/shared_ptr.hpp>
#include <string>
#include <OgreRenderWindow.h>


#ifndef __COISINPUTMODULE_H__
#define __COISINPUTMODULE_H__


using namespace std;
using namespace boost;


/**
 *	The game module that use OIS. 
 */ 
class COISInputModule
{
	private:

		/**
		 * The events which will be processed
		 */
		shared_ptr<COISInputManager> inputmanager;


	public:
		
		/**
		 * A constructor
		 */
		COISInputModule( );

		/**
		 * @copydoc CModule::init
		 */
		virtual void init();

		/**
		 * @copydoc CModule::process
		 */
		virtual void process();

		/**
		 * @copydoc CModule::exit 
		 */
		virtual void exit();

		/**
		 * @copydoc CModule::~CModule
		 */
		virtual ~COISInputModule();
};

#endif // __COISINPUTMODULE_H__
