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


/*------------------------CMouseStateTranslator----------------------------*\
|   This is translate mouse events to action events                         |
|                                                                           |
|   Changelog :                                                             |
|               01/11/2008 - Paf - Initial release                          |
|                                                                           |
|		16/05/2008 - WT  - Send a list of action messages instead   |
|				     of a unique message per input	    |
\*-------------------------------------------------------------------------*/


#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include "../include/cmousebuttonstatetranslator.h"
#include "../../dynamicobject/include/dynamicobjectmanager.h"
#include "../../core/include/cmessagemodule.h"
#include "../../log/include/clogmodule.h"
#include "../include/cinputmouseevents.h"
#include "../include/ctranslationevents.h"
#include <OIS/OISMouse.h>

#include "../../config.h"

using namespace boost;
using namespace Gnoll::Core;
using namespace Gnoll::DynamicObject;


namespace Gnoll
{
	namespace Input
	{

		CMouseButtonStateTranslator::CMouseButtonStateTranslator():
			mouseButtonPressedEvent("MOUSE_PRESSED"),
			mouseButtonReleasedEvent("MOUSE_RELEASED")
		{

			DynamicObjectManager *pom = DynamicObjectManager::getInstancePtr();

			/**
			 * Loading translation map : Keycode -> Action
			 */
			this->mouseButtonEventTranslationMap = pom->load("mouseButtonEventTranslationMap");

		}

		CMouseButtonStateTranslator::~CMouseButtonStateTranslator()
		{
		}

		void CMouseButtonStateTranslator::handle ( shared_ptr<CMessage> message )
		{
			/**
			 * Get the key code
			 */
			MouseButton button = message->getData<MouseButton>();


			/**
			 * Get the value and convert it to a string
			 */
			string buttonValue = lexical_cast<string> (button);

			Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "Looking for KeyCode [" + buttonValue + "]" );


			/**
			 * If an action is associated to this key code, an action message is sent
			 */
			if ( mouseButtonEventTranslationMap->hasAttribute(buttonValue) )
			{
				CMessageType actionEventType(ACTION_EVENT_STATE_TYPE);


				shared_ptr<List> actionList = mouseButtonEventTranslationMap->getAttribute<List>( buttonValue );
				typedef list< shared_ptr<IAttribute> >::iterator ListIterator;


				CMessageType messageType = message->getType();
				float intensity = 1.0;

				if (messageType == mouseButtonPressedEvent)
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
						shared_ptr<CMessage>  actionMessage (new CMessage( actionEventType, data ));

						std::ostringstream tmpString;
						if (CMessageModule::getInstancePtr()->getMessageManager()->queueMessage(actionMessage) == true)
						{
							tmpString << "Message ajoute ["<< *actionName << "]";
						}
						else
						{
							tmpString << "Message NON ajoute ["<< *actionName << "]";
						}
						Gnoll::Log::CLogModule::getInstancePtr()->logMessage( tmpString.str() );
					}
				}

			}

		}
	};
};
