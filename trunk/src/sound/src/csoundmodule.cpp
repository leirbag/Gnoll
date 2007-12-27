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
#include "../../config.h"


namespace Gnoll {
	namespace Sound {
		vector <string> * CSoundModule::sound_queue = new vector <string>;

		CSoundModule::CSoundModule()
		{
			//Initialise les écouteurs d'événements
			// --> Evenement Demande de lecture d'un son
			shared_ptr<CMessageListener> playListener (new SoundPlayListener(sound_queue));
			CMessageModule().getInstancePtr()->getMessageManager()->addListener(playListener, *(new CMessageType("PLAY_SOUND")));
		}
		
		CSoundModule::~CSoundModule()
		{
		}
		
		
		void CSoundModule::init()
		{
			std::cout << "Initialisation du module de Son " << endl;
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
			
			// A REDEFINIR, en utilisant un fichier de configuration
			//Récupère l'instance du SoundManager, et ajoute le répertoire par default
			sMgr = SoundManager::getInstancePtr();
			shared_ptr<ISource> defaultFolder (new SourceFile("./"));
			sMgr->addLoadSource(defaultFolder);
			shared_ptr<ISource> soundFolder (new SourceFile("./data/sound/"));
			sMgr->addLoadSource(soundFolder);
		}
		
		void CSoundModule::process()
		{
			shared_ptr<Sound> currentSound;
			
			//Parcours la liste des sons dont la lecture a été demandée
			for (int i = 0; i < sound_queue->size(); i++)
			{
				//cout << "A lire: " << (*sound_queue)[i];
				//Charge ce son 
				currentSound = sMgr->load((*sound_queue)[i]);
				//Le joue
				currentSound->play();
			}
			sound_queue->clear();
		
		}
		
		void CSoundModule::exit()
		{
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
