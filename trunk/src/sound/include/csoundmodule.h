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


/*----------------------------csoundcmodule.h-------------------------------*\
|   The sound module                                                         |
|                                                                            |
|   Changelog :                                                              |
|               11/06/2007 - Soax - Initial release                          |
|               02/02/2008 - Bruno Mahe - Add some doxygen comments          |
|                                                                            |
\*--------------------------------------------------------------------------*/

#include "../../core/include/cmodule.h"
#include "../../core/include/singleton.h"
#include "../../core/include/cgenericmessagemanager.h"
#include "../../core/include/cmessagemodule.h"
#include "soundplaylistener.h"
#include "soundmanager.h"

#include <iostream>
#include <vector>


#ifndef __CSOUNDMODULE_H__
#define __CSOUNDMODULE_H__

using namespace Gnoll::Core;
using namespace Gnoll::Sound;
using namespace std;

namespace Gnoll {

	namespace Sound {

		/**
		 *	The sound module
		 */
		class CSoundModule : public CModule, public Singleton<CSoundModule>
		{

			private :

				/**
				 * OpenAL device object
				 */
				ALCdevice * device;

				/**
				 * OpenAL context object
				 */
				ALCcontext * context;

				/**
				 * Sound manager
				 */
				SoundManager * sMgr;
				
				/**
				 * Playlist
				 */
				static vector< string > * sound_queue;

				/**
				 * Listener used to play sound
				 */
				shared_ptr<CMessageListener> play_listener;
				
			public:
		
				/**
				* A constructor
				*/
				CSoundModule();
		
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
				virtual ~CSoundModule();
				
				/**
				 * Add a sound to the playlist
				 * @param _soundName Name of the sound to add to the playlist
				 */
				static void enqueueSound(const string _soundName);

		};
	}		
}	
		
#endif		
