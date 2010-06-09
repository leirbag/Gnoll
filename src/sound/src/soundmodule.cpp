/**************************************************************************
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

#include "../include/soundmodule.h"

#include <sstream>

#include "../../config.h"
#include "../../log/include/logmacros.h"
#include "../include/soundmanager.h"

namespace Gnoll 
{
	namespace Sound 
	{
		SoundModule::SoundModule()
		{
			/**
			 * Initialize sound queue
			 */
			m_sound_queue = new vector<string>;
		}

		SoundModule::~SoundModule()
		{
			/**
			 * Delete sound queue
			 */
			delete m_sound_queue;
		}
		
		void SoundModule::init()
		{
			/**
			 * SoudPlayListener is in charge of handling messages asking to play a sound
			 */
			m_play_listener = shared_ptr<Messages::Listener>(new SoundPlayListener(m_sound_queue));
			MessageModule* messageModule = MessageModule::getInstancePtr();
			messageModule->getMessageManager()->addListener(m_play_listener, Messages::MessageType("PLAY_SOUND"));

			/**
			 * Default OpenAL m_device
			 * XXX Might need to be updated
			 */
			m_device = alcOpenDevice(NULL);
			
			if(m_device)
			{
				m_context = alcCreateContext(m_device, NULL);
				alcMakeContextCurrent(m_context);
			}
		
			//Listener properties -- To be redefined
			ALfloat listenerPos[] = { 0.0, 0.0, 0.0 };
			ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
			ALfloat listenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };
			alListenerfv(AL_POSITION,    listenerPos);
			alListenerfv(AL_VELOCITY,    listenerVel);
			alListenerfv(AL_ORIENTATION, listenerOri);
			//-----------------------

			/**
			 * Initialize the sound manager
			 */	
			m_soundMgr = SoundManager::getInstancePtr();
		}
		
		void SoundModule::process()
		{
			shared_ptr<Sound> currentSound;
			
			/**
			 * Go through the list of sound to play
			 */
			for(unsigned int i = 0; i < m_sound_queue->size(); i++)
			{
				/**
				 * Load that sound
				 * Since the SoundManager cache them, they should be loaded only once (except if
				 * m_sound_queue->size is greater than the number of sound the SoundManager is caching)
				 */
				currentSound = m_soundMgr->load((*m_sound_queue)[i]);

				if(!currentSound)
				{
					std::ostringstream tmpString;
					tmpString << "Impossible de trouver " << (*m_sound_queue)[i] << " dans les paths existants !";
					GNOLL_LOG().logMessage( tmpString.str() );
				}
				else
					currentSound->play();
			}

			m_sound_queue->clear();
		}
		
		void SoundModule::exit()
		{
			/**
			 * Destroy SoundManager
			 */
			SoundManager::destroy();

			/**
			 * Delete listeners
			 */
			MessageModule* messageModule = MessageModule::getInstancePtr();
			messageModule->getMessageManager()->delListener(m_play_listener, Messages::MessageType("PLAY_SOUND"));

			/**
			 * Uninitialize OpenAL
			 */
			if(m_device)
			{
				alcMakeContextCurrent(NULL);
				alcDestroyContext(m_context);
				alcCloseDevice(m_device);
			}
		}
			
		void SoundModule::enqueueSound(const string instance)
		{
			m_sound_queue->push_back(instance);	
		}
	}
}
