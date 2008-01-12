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


/*---------------------CMouseMotionEventsTranslator------------------------*\
|   This is translate mouse motion events to action events                  |
|                                                                           |
|   Changelog :                                                             |
|               01/11/2008 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include "../include/cmousemotioneventstranslator.h"
#include "../include/cinputmouseevents.h"
#include "../include/ctranslationevents.h"
#include "../../core/include/persistentobjectmanager.h"
#include "../../core/include/cmessagemodule.h"
#include "../include/cinputmouseevents.h"
#include <OIS/OISMouse.h>
#include <iostream>

#include <OgreRenderWindow.h>

#include "../../graphic/include/cgraphicmodule.h"


using namespace boost;
using namespace Gnoll::Core;


namespace Gnoll
{
	namespace Input 
	{

		CMouseMotionEventsTranslator::CMouseMotionEventsTranslator(): mouseMoved("MOUSE_MOVED")
		{

			PersistentObjectManager *pom = PersistentObjectManager::getInstancePtr();

			/**
			 * Loading translation map : Keycode -> Action
			 */
			this->mouseMotionEventTranslationMap = pom->load("mouseMotionEventTranslationMap");


			Ogre::RenderWindow* renderWindow = CGraphicModule::getInstancePtr()->getRenderWindow();

			if (renderWindow)
			{

				maxX = renderWindow->getWidth();
				maxY = renderWindow->getHeight();

			} else
			{

				/**
				 * Default values if we cannot get the renderWindow.
				 * This shouldn't happen
				 */
				maxX = 800;
				maxY = 600;

			}

		}

		CMouseMotionEventsTranslator::~CMouseMotionEventsTranslator()
		{
		}


		void CMouseMotionEventsTranslator::sendActionEventForEventAndIntensity(string _event, float _intensity)
		{
			CMessageType actionEventType(ACTION_EVENT_TYPE);

			/**
			 * If an action is associated to this key code, an action message is sent
			 */
			if ( mouseMotionEventTranslationMap->hasAttribute(_event) )
			{
				shared_ptr<String> actionString = mouseMotionEventTranslationMap->getAttribute<String>( _event );
				string actionName ( *actionString );

				ActionEvent actionEvent(actionName, _intensity);

				shared_ptr<boost::any> data (new boost::any(actionEvent) ) ;
				shared_ptr<CMessage>  actionMessage (new CMessage( actionEventType, data ));

				if (CMessageModule::getInstancePtr()->getMessageManager()->queueMessage(actionMessage) == true)
					cout << "Message ajoute ["<< actionName << "]" << endl;
				else
					cout << "Message NON ajoute ["<< actionName << "]" << endl;

			}	

		}

		void CMouseMotionEventsTranslator::sendXMotionEvents(MouseEvent _mouseEvent)
		{
			string event("");
			float intensity = 0.0f;

			if (_mouseEvent.relX != 0)	
			{
				if (_mouseEvent.relX > 0)
				{
					event = "+X";

				} else
				{
					event = "-X";
				}

				intensity = _mouseEvent.relX / maxX;
			}

			if (event != "")
			{
				sendActionEventForEventAndIntensity(event, intensity);
			}

		}
		void CMouseMotionEventsTranslator::sendYMotionEvents(MouseEvent _mouseEvent)
		{
			string event("");
			float intensity = 0.0f;

			if (_mouseEvent.relY != 0)	
			{
				if (_mouseEvent.relY > 0)
				{
					event = "+Y";

				} else
				{
					event = "-Y";
				}

				intensity = _mouseEvent.relY / maxY;
			}

			if (event != "")
			{
				sendActionEventForEventAndIntensity(event, intensity);
			}

		}

		void CMouseMotionEventsTranslator::handle ( shared_ptr<CMessage> message )
		{


			/**
			 * Get the motion information
			 */
			MouseEvent mouseMotionEvent = message->getData<MouseEvent>();

			sendXMotionEvents( mouseMotionEvent );
			sendYMotionEvents( mouseMotionEvent );

		}
	};
};



