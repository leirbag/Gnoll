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

#include "../include/keyboardeventstranslator.h"

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <OIS/OISKeyboard.h>

#include "../include/translationevents.h"
#include "../include/inputmouseevents.h"
#include "../../dynamicobject/include/dynamicobjectmanager.h"
#include "../../dynamicobject/include/float.h"
#include "../../core/include/messagemodule.h"
#include "../../log/include/logmodule.h"
#include "../../time/include/timemodule.h"
#include "../../config.h"

using namespace boost;
using namespace Gnoll::Core;
using namespace Gnoll::DynamicObject;
using namespace Gnoll::Time;

namespace Gnoll
{
	namespace Input 
	{
		KeyboardEventsTranslator::KeyboardEventsTranslator() : 
			m_keyUp("KEYBOARD_KEYUP"), 
			m_keyDown("KEYBOARD_KEYDOWN"), 
			m_lastTimeTriggerCalled(0)
		{

			DynamicObjectManager *pom = DynamicObjectManager::getInstancePtr();

			/**
			 * Loading translation map : Keycode -> Action
			 */
			this->m_keyboardEventTranslationMap = pom->load("keyboardEventTranslationMap");

			/**
			 * Keyboard config
			 */
			this->m_keyboardConfig = pom->load("keyboardConfig");
		}

		KeyboardEventsTranslator::~KeyboardEventsTranslator()
		{
		}

		void KeyboardEventsTranslator::handle ( shared_ptr<Message> message )
		{
			/**
			 * Get the key code
			 */
			OIS::KeyCode keyCode = message->getData<OIS::KeyCode>();

			/**
			 * Get the value and convert it to a string
			 */ 
			string keyCodeValue = lexical_cast<string> (keyCode);
			Gnoll::Log::LogModule::getInstancePtr()->logMessage( "Looking for KeyCode [" + keyCodeValue + "]");

			/**
			 * If an action is associated to this key code, an action message is sent
			 */
			if ( m_keyboardEventTranslationMap->hasAttribute(keyCodeValue) )
			{
				TimeModule* timeModule = TimeModule::getInstancePtr();

				Messages::MessageType messageType = message->getType();

				if (messageType == m_keyDown)
				{
					m_keyPressed[keyCodeValue] = timeModule->getMsecs();

					if (m_durationKeyPressed.find(keyCodeValue) == m_durationKeyPressed.end())
					{
						m_durationKeyPressed[keyCodeValue] = 0;
					}
				}
				else
				{
					m_durationKeyPressed[keyCodeValue] += timeModule->getMsecs() - m_keyPressed[keyCodeValue];
					m_keyPressed[keyCodeValue] = 0;
				}
			}
		}

		void KeyboardEventsTranslator::trigger ( shared_ptr<Message> _msg )
		{
			Messages::MessageType actionEventType(ACTION_EVENT_TYPE);
			TimeModule* timeModule = TimeModule::getInstancePtr();

			shared_ptr<Float> defaultKeyboardSensibility = shared_ptr<Float> (new Float(1.0f));
			shared_ptr< Gnoll::DynamicObject::Float > keyboardSensibility = m_keyboardConfig->getAttributeOrDefault<Float>("sensibility", defaultKeyboardSensibility);

			unsigned long int now = timeModule->getMsecs();
			unsigned long int period = now - m_lastTimeTriggerCalled;

			for( map<string, unsigned long int>::iterator it = m_durationKeyPressed.begin(); it != m_durationKeyPressed.end(); it++)
			{
				unsigned long int timePressed = it->second;

				if (m_keyPressed[it->first] != 0)
				{
					timePressed += now - m_keyPressed[it->first];

					m_keyPressed[it->first] = now;
				}

				if (timePressed > 0)
				{
					float intensity = (float) timePressed / (float) period * (*keyboardSensibility);

					shared_ptr<List> actionList = m_keyboardEventTranslationMap->getAttribute<List>( it->first );
					typedef list< shared_ptr<AbstractAttribute> >::iterator ListIterator;

					/**
					 * Send all action messages in the list
					 */
					for (ListIterator itAttrs = actionList->begin(); itAttrs != actionList->end(); itAttrs++)
					{
						if (shared_ptr<String> actionName = dynamic_pointer_cast<String>(*itAttrs))
						{
							ActionEvent actionEvent(*actionName, intensity);

							shared_ptr<boost::any> data (new boost::any(actionEvent) ) ;
							shared_ptr<Message>  actionMessage (new Message( actionEventType, data ));

							std::ostringstream tmpString;

							try
							{
								MessageModule::getInstancePtr()->getMessageManager()->queueMessage(actionMessage);
								tmpString << "Message ajoute ["<< *actionName << "]" << endl;
							}
							catch(...)
							{
								tmpString << "Message NON ajoute ["<< *actionName << "]" << " of intensity ";
								tmpString <<  intensity << " => " << timePressed << " / " << period << endl;
							}

							Gnoll::Log::LogModule::getInstancePtr()->logMessage( tmpString.str() );

							it->second = 0;
						}
					}
				}
			}

			m_lastTimeTriggerCalled = now;
		}
	};
};
