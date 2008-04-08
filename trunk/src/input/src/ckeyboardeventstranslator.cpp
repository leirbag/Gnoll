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
#include "../include/ctranslationevents.h"
#include "../../core/include/persistentobjectmanager.h"
#include "../../core/include/cmessagemodule.h"
#include "../include/cinputmouseevents.h"
#include <OIS/OISKeyboard.h>
#include <iostream>
#include "../../time/include/ctimemodule.h"



using namespace boost;
using namespace Gnoll::Core;
using namespace Gnoll::Time;


namespace Gnoll
{
	namespace Input 
	{

		CKeyboardEventsTranslator::CKeyboardEventsTranslator(): keyUp("KEYBOARD_KEYUP"), keyDown("KEYBOARD_KEYDOWN"), m_lastTimeTriggerCalled(0)
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


				shared_ptr<String> actionString = keyboardEventTranslationMap->getAttribute<String>( keyCodeValue );
				string actionName ( *actionString );

				CTimeModule* timeModule = CTimeModule::getInstancePtr();

				CMessageType messageType = message->getType();

				if (messageType == keyDown)
				{
					m_keyPressed[keyCodeValue]	= timeModule->getMsecs();

					if (m_durationKeyPressed.find(keyCodeValue) == m_durationKeyPressed.end())
					{
						m_durationKeyPressed[keyCodeValue] = 0;
					}
				}
				else
				{
					m_durationKeyPressed[keyCodeValue] += timeModule->getMsecs() - m_keyPressed[keyCodeValue];
					m_keyPressed[keyCodeValue]	= 0;
				}

			}

		}


		void CKeyboardEventsTranslator::trigger ( shared_ptr<CMessage> _msg )
		{

			CMessageType actionEventType(ACTION_EVENT_TYPE);
			CTimeModule* timeModule = CTimeModule::getInstancePtr();

			unsigned long int now = timeModule->getMsecs();
			unsigned long int period = now - m_lastTimeTriggerCalled;


			for( map<string, unsigned long int>::iterator it =	m_durationKeyPressed.begin(); it != m_durationKeyPressed.end(); it++)
			{
				unsigned long int timePressed = it->second;

				if (m_keyPressed[it->first] != 0)
				{
					timePressed += now - m_keyPressed[it->first];

					m_keyPressed[it->first] = now;
				}


				if (timePressed > 0)
				{

					float intensity = (float) timePressed / (float) period;

					shared_ptr<String> actionString = keyboardEventTranslationMap->getAttribute<String>( it->first );
					string actionName ( *actionString );
					ActionEvent actionEvent(actionName, intensity);

					shared_ptr<boost::any> data (new boost::any(actionEvent) ) ;
					shared_ptr<CMessage>  actionMessage (new CMessage( actionEventType, data ));

					if (CMessageModule::getInstancePtr()->getMessageManager()->queueMessage(actionMessage) == true)
						cout << "Message ajoute ["<< actionName << "]" << endl;
					else
						cout << "Message NON ajoute ["<< actionName << "]" << " of intensity " <<  intensity << " => " << timePressed << " / " << period << endl;

					it->second = 0;
				}
			}

			m_lastTimeTriggerCalled = now;
		}
	};
};



