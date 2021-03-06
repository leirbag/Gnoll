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

#include "../include/mousebuttonstatetranslator.h"

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <OIS/OISMouse.h>

#include "../include/inputmouseevents.h"
#include "../include/translationevents.h"
#include "../../dynamicobject/include/dynamicobjectmanager.h"
#include "../../dynamicobject/include/float.h"
#include "../../core/include/messagemodule.h"
#include "../../log/include/logmodule.h"
#include "../../config.h"

using namespace boost;
using namespace Gnoll::Core;
using namespace Gnoll::DynamicObject;

namespace Gnoll
{
	namespace Input
	{

		MouseButtonStateTranslator::MouseButtonStateTranslator():
			m_mouseButtonPressedEvent("MOUSE_PRESSED"),
			m_mouseButtonReleasedEvent("MOUSE_RELEASED")
		{

			DynamicObjectManager *pom = DynamicObjectManager::getInstancePtr();

			/**
			 * Loading translation map : Keycode -> Action
			 */
			this->m_mouseButtonEventTranslationMap = pom->load("mouseButtonEventTranslationMap");


			/**
			 * Keyboard config
			 */
			this->m_mouseConfig = pom->load("mouseConfig");

		}

		MouseButtonStateTranslator::~MouseButtonStateTranslator()
		{
		}

		void MouseButtonStateTranslator::handle ( shared_ptr<Message> message )
		{
			/**
			 * Get the key code
			 */
			MouseButton button = message->getData<MouseButton>();

			/**
			 * Get the value and convert it to a string
			 */
			string buttonValue = lexical_cast<string> (button);
			Gnoll::Log::LogModule::getInstancePtr()->logMessage( "Looking for KeyCode [" + buttonValue + "]" );

			/**
			 * If an action is associated to this key code, an action message is sent
			 */
			if ( m_mouseButtonEventTranslationMap->hasAttribute(buttonValue) )
			{
				Messages::MessageType actionEventType(ACTION_EVENT_STATE_TYPE);
				shared_ptr<List> actionList = m_mouseButtonEventTranslationMap->getAttribute<List>( buttonValue );
				typedef list< shared_ptr<AbstractAttribute> >::iterator ListIterator;

				shared_ptr<Float> defaultMouseSensibility = shared_ptr<Float> (new Float(1.0f));
				shared_ptr< Gnoll::DynamicObject::Float > mouseSensibility = m_mouseConfig->getAttributeOrDefault<Float>("sensibility", defaultMouseSensibility);

				Messages::MessageType messageType = message->getType();
				float intensity = *mouseSensibility;

				if (messageType == m_mouseButtonPressedEvent)
				{
					intensity = 0.0f;
				}

				/**
				 * We send all action messages in the list
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
							tmpString << "Message ajoute ["<< *actionName << "]";
						}
						catch(...)
						{
							tmpString << "Message NON ajoute ["<< *actionName << "]";
						}
						Gnoll::Log::LogModule::getInstancePtr()->logMessage( tmpString.str() );
					}
				}
			}
		}
	}
}
