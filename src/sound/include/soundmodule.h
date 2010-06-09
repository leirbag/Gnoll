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

#ifndef __SOUNDMODULE_H__
#define __SOUNDMODULE_H__

#include <iostream>
#include <vector>

#include "../../core/include/module.h"
#include "../../core/include/singleton.h"
#include "../../core/include/messagemodule.h"
#include "soundplaylistener.h"
#include "soundmanager.h"

using namespace Gnoll::Core;
using namespace Gnoll::Sound;
using namespace std;

namespace Gnoll 
{
	namespace Sound 
	{
		/**
		 *	The sound module
		 */
		class SoundModule : public Module, public Singleton<SoundModule>
		{
			public:
				/**
				 * A constructor
				 */
				SoundModule();

				/**
				 * @copydoc Module::~Module
				 */
				virtual ~SoundModule();

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
				 * Add a sound to the playlist
				 * @param _soundName Name of the sound to add to the playlist
				 */
				void enqueueSound(const string soundName);

			private:
				/**
				 * OpenAL device object
				 */
				ALCdevice* m_device;

				/**
				 * OpenAL context object
				 */
				ALCcontext* m_context;

				/**
				 * Sound manager
				 */
				SoundManager* m_soundMgr;

				/**
				 * Playlist
				 */
				vector<string>* m_sound_queue;

				/**
				 * Listener used to play sound
				 */
				shared_ptr<Messages::Listener> m_play_listener;
		};
	}		
}	

#endif		
