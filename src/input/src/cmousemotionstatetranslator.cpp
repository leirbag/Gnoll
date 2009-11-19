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

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include "../include/cmousemotionstatetranslator.h"
#include "../include/cinputmouseevents.h"
#include "../include/ctranslationevents.h"
#include "../../dynamicobject/include/dynamicobjectmanager.h"
#include "../../dynamicobject/include/float.h"
#include "../../core/include/cmessagemodule.h"
#include "../../log/include/clogmodule.h"
#include "../../graphic/include/cgraphicmodule.h"
#include "../../log/include/clogmacros.h"
#include <OIS/OISMouse.h>

#include "../../config.h"

using namespace boost;
using namespace Gnoll::Core;
using namespace Gnoll::Graphic;
using namespace Gnoll::DynamicObject;


namespace Gnoll
{
	namespace Input
	{

		CMouseMotionStateTranslator::CMouseMotionStateTranslator():
			mouseMovedEvent("MOUSE_MOVED")
		{
			/**
			 * Mouse config
			 */
			DynamicObjectManager *pom = DynamicObjectManager::getInstancePtr();
			this->mouseConfig = pom->load("mouseConfig");

			/**
			 * Get window size
			 */
			Ogre::RenderWindow* renderWindow = CGraphicModule::getInstancePtr()->getRenderWindow();
			if (renderWindow)
			{

				maxX = renderWindow->getWidth();
				maxY = renderWindow->getHeight();

			}
            else
			{

				/**
				 * Default values if we cannot get the renderWindow.
				 * This shouldn't happen
				 */
				maxX = 800;
				maxY = 600;
			}

		}


		CMouseMotionStateTranslator::~CMouseMotionStateTranslator()
		{
		}


		void CMouseMotionStateTranslator::sendXBorderState(MouseEvent mouseEvent)
		{
			Messages::MessageType actionEventType(ACTION_EVENT_STATE_TYPE);
			ActionEvent actionEvent ("BORDER_X", mouseEvent.abX / maxX);
			shared_ptr<boost::any> data (new boost::any(actionEvent) ) ;
			shared_ptr<CMessage>  actionMessage (new CMessage( actionEventType, data ));

			try
			{
				CMessageModule::getInstancePtr()->getMessageManager()->queueMessage(actionMessage);
				GNOLL_LOG() << "Message ajoute [BORDER_X]\n";
			}
			catch(...)
			{
				GNOLL_LOG() << "Message NON ajoute [BORDER_X]\n";
			}
		}

		void CMouseMotionStateTranslator::sendYBorderState(MouseEvent mouseEvent)
		{
			Messages::MessageType actionEventType(ACTION_EVENT_STATE_TYPE);
			ActionEvent actionEvent ("BORDER_Y", mouseEvent.abY / maxY);
			shared_ptr<boost::any> data (new boost::any(actionEvent) ) ;
			shared_ptr<CMessage>  actionMessage (new CMessage( actionEventType, data ));

			try
			{
				CMessageModule::getInstancePtr()->getMessageManager()->queueMessage(actionMessage);
				GNOLL_LOG() << "Message ajoute [BORDER_Y]\n";
			}
			catch(...)
			{
				GNOLL_LOG() << "Message NON ajoute [BORDER_Y]\n";
			}
		}

		void CMouseMotionStateTranslator::handle ( shared_ptr<CMessage> message )
		{
			/**
			 * Get the motion information
			 */
			MouseEvent mouseMotionEvent = message->getData<MouseEvent>();

			sendXBorderState(mouseMotionEvent);
			sendYBorderState(mouseMotionEvent);
		}
	};
};
