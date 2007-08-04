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


/*--------------------------csdlinputmodule.cpp----------------------------*\
|   The game input module that use SDL                                      |
|                                                                           |
|   Changelog :                                                             |
|               06/23/2006 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../include/csdlinputmodule.h"
#include <iostream>


CSDLInputModule::CSDLInputModule()
{


}


void CSDLInputModule::init()
{
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) 
	{
        printf("N'a pas pu initialise la SDL: %s\n", SDL_GetError());
        return;
   }
/*	if(SDL_SetVideoMode(640, 480, 32, 0) == NULL)
	{
		printf("\n SDL_SetVideoMode(): %s \n", SDL_GetError());
		return ;
	}*/
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
}


void CSDLInputModule::process()
{	

	CMessageType keydown("KEYBOARD_KEYDOWN");
	CMessageType keyup("KEYBOARD_UP");


	while( SDL_PollEvent( &m_event ) )
	{
		switch( m_event.type )
		{cout << "Je suis quand meme passe par la" << endl;
			case SDL_QUIT:
				{
				  shared_ptr<void> texte (new string("Q")) ;

				  shared_ptr<CMessage>  mymessage (new CMessage(keydown, texte ));

				  if (CGenericMessageManager::getInstance().queueMessage(mymessage) == true)
				  	  cout << "Message de quittage ajoute" << endl;
				}
				break;
			
			case SDL_KEYDOWN:
				{
				shared_ptr<void> texte (new string( SDL_GetKeyName(m_event.key.keysym.sym) )) ;

				shared_ptr<CMessage>  mymessage (new CMessage(keydown, texte ));

				if (CGenericMessageManager::getInstance().queueMessage(mymessage) == true)
					cout << "Message ajoute" << endl;

				cout << "Je suis quand meme passe par la" << endl;
				}
				break;

			case SDL_KEYUP:
				break;

			default:
				break;
		}

	}
}


void CSDLInputModule::exit()
{

}


CSDLInputModule::~CSDLInputModule()
{

}
