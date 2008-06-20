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


/*-----------------------CKeyboardStateTranslator-------------------------*\
|   This is translate keyboard events to action events                      |
|                                                                           |
|   Changelog :                                                             |
|               01/08/2008 - WT - Initial release                           |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include "../include/ckeyboardstatetranslator.h"
#include "../include/ctranslationevents.h"
#include "../../dynamicobject/include/dynamicobjectmanager.h"
#include "../../core/include/cmessagemodule.h"
#include "../../log/include/clogmodule.h"
#include "../include/cinputmouseevents.h"
#include <OIS/OISKeyboard.h>
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

		CKeyboardStateTranslator::CKeyboardStateTranslator():
			keyUp("KEYBOARD_KEYUP"),
			keyDown("KEYBOARD_KEYDOWN")
		{

			DynamicObjectManager *pom = DynamicObjectManager::getInstancePtr();

			/**
			 * Loading translation map : Keycode -> Action
			 */
			this->keyboardEventTranslationMap = pom->load("keyboardEventTranslationMap");

		}

		CKeyboardStateTranslator::~CKeyboardStateTranslator()
		{
		}

		void CKeyboardStateTranslator::handle ( shared_ptr<CMessage> message )
		{
			CMessageType actionEventType(ACTION_EVENT_STATE_TYPE);

			/**
			 * Get the key code
			 */
			OIS::KeyCode keyCode = message->getData<OIS::KeyCode>();


			/**
			 * Get the value and convert it to a string
			 */
			string keyCodeValue = lexical_cast<string> (keyCode);

			Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "Looking for KeyCode [" + keyCodeValue + "]" );

			/**
			 * If an action is associated to this key code, an action message is sent
			 */
			if ( keyboardEventTranslationMap->hasAttribute(keyCodeValue) )
			{
				shared_ptr<List> actionList = keyboardEventTranslationMap->getAttribute<List>( keyCodeValue );
				typedef list< shared_ptr<IAttribute> >::iterator ListIterator;

				float intensity = 1.0f;
				if( message->getType() == keyUp )
					intensity = 0.0f;

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
