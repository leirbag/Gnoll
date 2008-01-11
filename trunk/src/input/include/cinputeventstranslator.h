/***************************************************************************
 *   Copyright (C) 2008 by Paf                                             *
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


/*-------------------------CInputEventsTranslator.h-------------------------*\
|   This translate input events to action events used by game logic          |
|                                                                            |
|   Changelog :                                                              |
|               01/08/2008 - Paf - Initial release                           |
|                                                                            |
\*--------------------------------------------------------------------------*/



#ifndef __CINPUTEVENTSTRANSLATOR_H__
#define __CUNPUTEVENTSTRANSLATOR_H__



#include <boost/shared_ptr.hpp>

#include "../../core/include/cmodule.h"
#include "../../core/include/singleton.h"
#include "../../core/include/persistentobject.h"

#include "../../core/include/cmessagelistener.h"



using namespace boost;
using namespace Gnoll::Core;


namespace Gnoll
{

	namespace Input
	{

		/**
		 *	Interface of all game modules. 
		 */ 
		class CInputEventsTranslator: public CModule, public Gnoll::Core::Singleton<CInputEventsTranslator>
		{

			private:

				/**
				 * Translator for keyboard generated messages
				 */
				shared_ptr<CMessageListener> keyboardEventsTranslator;


			public:

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
				virtual ~CInputEventsTranslator();
		
				/**
				 * @copydoc CModule::CModule
				 */
				CInputEventsTranslator();

				/**
				 * Activate keyboard events translation
				 */
				void activateKeyboardTranslation();

				/**
				 * Deactivate keyboard events translation
				 */
				void deactivateKeyboardTranslation();
		};

	}
}

#endif // __CINPUTEVENTSTRANSLATOR_H__


