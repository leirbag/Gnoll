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


/*------------------------CMouseEventsTranslator---------------------------*\
|   This is translate mouse events to action events                         |
|                                                                           |
|   Changelog :                                                             |
|               16/05/2008 - WT - Initial release                           |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include "../include/cmousebuttoneventstranslator.h"
#include "../../dynamicobject/include/dynamicobjectmanager.h"
#include "../../core/include/cmessagemodule.h"
#include "../include/cinputmouseevents.h"
#include "../include/ctranslationevents.h"
#include <OIS/OISMouse.h>
#include <iostream>
#include "../../time/include/ctimemodule.h"

#include "../../config.h"

using namespace boost;
using namespace Gnoll::Core;
using namespace Gnoll::DynamicObject;
using namespace Gnoll::Time;


namespace Gnoll
{
	namespace Input 
	{

		CMouseButtonEventsTranslator::CMouseButtonEventsTranslator():
			mouseButtonPressedEvent("MOUSE_PRESSED"),
			mouseButtonReleasedEvent("MOUSE_RELEASED")
		{

			DynamicObjectManager *pom = DynamicObjectManager::getInstancePtr();

			/**
			 * Loading translation map : Keycode -> Action
			 */
			this->mouseButtonEventTranslationMap = pom->load("mouseButtonEventTranslationMap");

		}

		CMouseButtonEventsTranslator::~CMouseButtonEventsTranslator()
		{
		}

		void CMouseButtonEventsTranslator::handle ( shared_ptr<CMessage> message )
		{
			/**
			 * Get the key code
			 */
			MouseButton button = message->getData<MouseButton>();


			/**
			 * Get the value and convert it to a string
			 */ 
			string buttonValue = lexical_cast<string> (button);

#if DEBUG
			cout << "Looking for KeyCode [" << buttonValue << "]"<< endl;
#endif


			/**
			 * If an action is associated to this key code, an action message is sent
			 */
			if ( mouseButtonEventTranslationMap->hasAttribute(buttonValue) )
			{
				CMessageType actionEventType(ACTION_EVENT_TYPE);


				CTimeModule* timeModule = CTimeModule::getInstancePtr();

				CMessageType messageType = message->getType();

				if (messageType == mouseButtonPressedEvent)
				{
					m_buttonPressed[buttonValue] = timeModule->getMsecs();

					if (m_durationButtonPressed.find(buttonValue) == m_durationButtonPressed.end())
					{
						m_durationButtonPressed[buttonValue] = 0;
					}
				}
				else
				{
					m_durationButtonPressed[buttonValue] += timeModule->getMsecs() - m_buttonPressed[buttonValue];
					m_buttonPressed[buttonValue] = 0;
				}

			}

		}


		void CMouseButtonEventsTranslator::trigger ( shared_ptr<CMessage> _msg )
		{

			CMessageType actionEventType(ACTION_EVENT_TYPE);
			CTimeModule* timeModule = CTimeModule::getInstancePtr();

			unsigned long int now = timeModule->getMsecs();
			unsigned long int period = now - m_lastTimeTriggerCalled;


			for( map<string, unsigned long int>::iterator it = m_durationButtonPressed.begin(); it != m_durationButtonPressed.end(); it++)
			{
				unsigned long int timePressed = it->second;

				if (m_buttonPressed[it->first] != 0)
				{
					timePressed += now - m_buttonPressed[it->first];

					m_buttonPressed[it->first] = now;
				}


				if (timePressed > 0)
				{

					float intensity = (float) timePressed / (float) period;

					shared_ptr<List> actionList = mouseButtonEventTranslationMap->getAttribute<List>( it->first );
					typedef list< shared_ptr<IAttribute> >::iterator ListIterator;

					/**
					 * Send all action messages in the list
					 */
					for (ListIterator itAttrs = actionList->begin(); itAttrs != actionList->end(); itAttrs++)
					{
						if (shared_ptr<String> actionName = dynamic_pointer_cast<String>(*itAttrs))
						{
							ActionEvent actionEvent(*actionName, intensity);

							shared_ptr<boost::any> data (new boost::any(actionEvent) ) ;
							shared_ptr<CMessage>  actionMessage (new CMessage( actionEventType, data ));

							if (CMessageModule::getInstancePtr()->getMessageManager()->queueMessage(actionMessage) == true)
							{
#if DEBUG
								cout << "Message ajoute ["<< *actionName << "]" << endl;
#endif
							}
							else
							{
#if DEBUG
								cout << "Message NON ajoute ["<< *actionName << "]" << " of intensity ";
								cout <<  intensity << " => " << timePressed << " / " << period << endl;
#endif
							}

							it->second = 0;
						}
					}
				}
			}

			m_lastTimeTriggerCalled = now;
		}
	};
};



