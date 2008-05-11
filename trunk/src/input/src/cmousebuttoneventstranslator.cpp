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
|               01/11/2008 - Paf - Initial release                          |
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

#include "../../config.h"

using namespace boost;
using namespace Gnoll::Core;
using namespace Gnoll::DynamicObject;


namespace Gnoll
{
	namespace Input 
	{

		CMouseButtonEventsTranslator::CMouseButtonEventsTranslator(): mouseButtonPressedEvent("MOUSE_PRESSED"), mouseButtonReleasedEvent("MOUSE_RELEASED")
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


				shared_ptr<String> actionString = mouseButtonEventTranslationMap->getAttribute<String>( buttonValue );
				string actionName ( *actionString );


				CMessageType messageType = message->getType();
				float intensity = 1.0;

				if (messageType == mouseButtonPressedEvent)
				{
					intensity = 0.0f;
				}

				ActionEvent actionEvent(actionName, intensity);



				shared_ptr<boost::any> data (new boost::any(actionEvent) ) ;
				shared_ptr<CMessage>  actionMessage (new CMessage( actionEventType, data ));

				if (CMessageModule::getInstancePtr()->getMessageManager()->queueMessage(actionMessage) == true)
				{
#if DEBUG
					cout << "Message ajoute ["<< actionName << "]" << endl;
#endif
				}
				else
				{
#if DEBUG
					cout << "Message NON ajoute ["<< actionName << "]" << endl;
#endif
				}

			}

		}
	};
};



