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


/*--------------------------csoundcmodule.cpp------------------------------*\
|   The sound module                                                        |
|                                                                           |
|   Changelog :                                                             |
|          11/06/2007 - Soax - Initial release                              |
|                                                                           |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../include/csoundmodule.h"
#include "config.h"


namespace Gnoll {
	namespace Sound {
		vector <string> * CSoundModule::sound_queue = new vector <string>;

		CSoundModule::CSoundModule()
		{
			
		}
		
		CSoundModule::~CSoundModule()
		{
		}
		
		
		void CSoundModule::init()
		{
		        //Initialise les listeners
			// Play Listener--> Evenement Demande de lecture d'un son
			play_listener = shared_ptr<CMessageListener>(new SoundPlayListener(sound_queue));
			CMessageModule* messageModule = CMessageModule::getInstancePtr();
			messageModule->getMessageManager()->addListener(play_listener, CMessageType("PLAY_SOUND"));

			
			//Device par default ... à voir une eventuelle modif
			device = alcOpenDevice(NULL);
			
			if (device)
			{
				context = alcCreateContext(device, NULL);
				alcMakeContextCurrent(context);
			}
						
		
			//Définit les propriétés du listener -- A REDEFINIR
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
			sMgr = SoundManager::getInstancePtr();
		}
		
		void CSoundModule::process()
		{
			shared_ptr<Sound> currentSound;
			
			//Parcours la liste des sons dont la lecture a été demandée
			for (unsigned int i = 0; i < sound_queue->size(); i++)
			{
				//cout << "A lire: " << (*sound_queue)[i];
				//Charge ce son 
				currentSound = sMgr->load((*sound_queue)[i]);
				if (!currentSound)
				cout << "Impossible de trouver " << (*sound_queue)[i] << " dans les paths existants !" << endl;
				else
					//Le joue
					currentSound->play();
			}
			sound_queue->clear();
		
		}
		
		void CSoundModule::exit()
		{
			//Supprime le listener
			CMessageModule* messageModule = CMessageModule::getInstancePtr();
			messageModule->getMessageManager()->delListener(play_listener, CMessageType("PLAY_SOUND"));
		
			alcMakeContextCurrent(NULL);
			alcDestroyContext(context);
			alcCloseDevice(device);		
		}
			
		void CSoundModule::enqueueSound(const string instance)
		{
			sound_queue->push_back(instance);	
		}
	}
}
