/**************************************************************************
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


/*-----------------------CKeyboardEventsTranslator-------------------------*\
|   This is translate keyboard events to action events                      |
|                                                                           |
|   Changelog :                                                             |
|               01/08/2008 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include "../include/ckeyboardeventstranslator.h"
#include "../include/ckeyboardtranslationevents.h"
#include "../../core/include/persistentobjectmanager.h"
#include "../../core/include/cmessagemodule.h"
#include "../include/cinputmouseevents.h"
#include <OIS/OISKeyboard.h>
#include <iostream>


using namespace boost;
using namespace Gnoll::Core;


namespace Gnoll
{
	namespace Input 
	{

		CKeyboardEventsTranslator::CKeyboardEventsTranslator(): keyUp("KEYBOARD_KEYUP"), keyDown("KEYBOARD_KEYDOWN")
		{

			PersistentObjectManager *pom = PersistentObjectManager::getInstancePtr();

			/**
			 * Loading translation map : Keycode -> Action
			 */
			this->keyboardEventTranslationMap = pom->load("keyboardEventTranslationMap");

		}

		CKeyboardEventsTranslator::~CKeyboardEventsTranslator()
		{
		}

		void CKeyboardEventsTranslator::handle ( shared_ptr<CMessage> message )
		{
			/**
			 * Get the key code
			 */
			OIS::KeyCode keyCode = message->getData<OIS::KeyCode>();


			/**
			 * Get the value and convert it to a string
			 */ 
			string keyCodeValue = lexical_cast<string> (keyCode);
			cout << "Looking for KeyCode [" << keyCodeValue << "]"<< endl;


			/**
			 * If an action is associated to this key code, an action message is sent
			 */
			if ( keyboardEventTranslationMap->hasAttribute(keyCodeValue) )
			{
				CMessageType actionEventType(ACTION_EVENT_TYPE);


				shared_ptr<String> actionString = keyboardEventTranslationMap->getAttribute<String>( keyCodeValue );
				string actionName ( *actionString );


				CMessageType messageType = message->getType();
				float intensity = 1.0;

				if (messageType == keyDown)
				{
					intensity = 0.0f;
				}

				ActionEvent actionEvent(actionName, intensity);



				shared_ptr<boost::any> data (new boost::any(actionEvent) ) ;
				shared_ptr<CMessage>  actionMessage (new CMessage( actionEventType, data ));

				if (CMessageModule::getInstancePtr()->getMessageManager()->queueMessage(actionMessage) == true)
					cout << "Message ajoute ["<< actionName << "]" << endl;
				else
					cout << "Message NON ajoute ["<< actionName << "]" << endl;

			}

		}
	};
};



