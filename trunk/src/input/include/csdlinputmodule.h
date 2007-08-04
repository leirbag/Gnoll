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


/*---------------------------csdlinputmodule.h------------------------------*\
|   The game input module that use SDL                                       |
|                                                                            |
|   Changelog :                                                              |
|               06/23/2006 - Paf - Initial release                           |
|                                                                            |
\*--------------------------------------------------------------------------*/

#include "cinputmodule.h"
#include "../../core/include/cgenericmessagemanager.h"
#include <SDL/SDL.h>
#include <boost/shared_ptr.hpp>
#include <string>

#ifndef __CSDLINPUTMODULE_H__
#define __CSDLINPUTMODULE_H__


using namespace std;
using namespace boost;


/**
 *	The game module that use SDL. 
 */ 
class CSDLInputModule
{
	private:

		/**
		 * The events which will be processed
		 */
		SDL_Event m_event;

	public:
		
		/**
		 * A constructor
		 */
		CSDLInputModule();

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
		virtual ~CSDLInputModule();
};

#endif // __CSDLINPUTMODULE_H__
