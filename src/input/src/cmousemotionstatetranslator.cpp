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
             * Border data
             */
            shared_ptr<Float> defaultBorder = shared_ptr<Float> (new Float(0.0f));
			border = *(mouseConfig->getAttributeOrDefault<Float>("border", defaultBorder));

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


		void CMouseMotionStateTranslator::sendLeftBorderState(MouseEvent mouseEvent)
		{
			Messages::MessageType actionEventType(ACTION_EVENT_STATE_TYPE);

            /*
             * We send a BORDER message with intensity of (-)1 if the cursor is in the
             * border, else intensity is set up to 0
             */
            float value = 0.0f;
            if(mouseEvent.abX <= border)
                value = 1.0f;

			ActionEvent actionEvent ("BORDER_LEFT", value);
			shared_ptr<boost::any> data (new boost::any(actionEvent) ) ;
			shared_ptr<CMessage>  actionMessage (new CMessage( actionEventType, data ));

			try
			{
				CMessageModule::getInstancePtr()->getMessageManager()->queueMessage(actionMessage);
				GNOLL_LOG() << "Message ajoute [BORDER_LEFT]\n";
			}
			catch(...)
			{
				GNOLL_LOG() << "Message NON ajoute [BORDER_LEFT]\n";
			}
		}


		void CMouseMotionStateTranslator::sendRightBorderState(MouseEvent mouseEvent)
		{
			Messages::MessageType actionEventType(ACTION_EVENT_STATE_TYPE);

            /*
             * We send a BORDER message with intensity of 1 if the cursor is in the
             * border, else intensity is set up to 0
             */
            float value = 0.0f;
            if(mouseEvent.abX >= maxX - border)
                value = 1.0f;

			ActionEvent actionEvent ("BORDER_RIGHT", value);
			shared_ptr<boost::any> data (new boost::any(actionEvent) ) ;
			shared_ptr<CMessage>  actionMessage (new CMessage( actionEventType, data ));

			try
			{
				CMessageModule::getInstancePtr()->getMessageManager()->queueMessage(actionMessage);
				GNOLL_LOG() << "Message ajoute [BORDER_RIGHT]\n";
			}
			catch(...)
			{
				GNOLL_LOG() << "Message NON ajoute [BORDER_RIGHT]\n";
			}
		}

        void CMouseMotionStateTranslator::sendBottomBorderState(MouseEvent mouseEvent)
		{
			Messages::MessageType actionEventType(ACTION_EVENT_STATE_TYPE);

            /*
             * We send a BORDER message with intensity of 1 if the cursor is in the
             * border, else intensity is set up to 0
             */
            float value = 0.0f;
            if(mouseEvent.abY <= border)
                value = 1.0f;

			ActionEvent actionEvent ("BORDER_BOTTOM", value);
			shared_ptr<boost::any> data (new boost::any(actionEvent) ) ;
			shared_ptr<CMessage>  actionMessage (new CMessage( actionEventType, data ));

			try
			{
				CMessageModule::getInstancePtr()->getMessageManager()->queueMessage(actionMessage);
				GNOLL_LOG() << "Message ajoute [BORDER_BOTTOM]\n";
			}
			catch(...)
			{
				GNOLL_LOG() << "Message NON ajoute [BORDER_BOTTOM]\n";
			}
		}


        void CMouseMotionStateTranslator::sendTopBorderState(MouseEvent mouseEvent)
		{
			Messages::MessageType actionEventType(ACTION_EVENT_STATE_TYPE);

            /*
             * We send a BORDER message with intensity of 1 if the cursor is in the
             * border, else intensity is set up to 0
             */
            float value = 0.0f;
            if(mouseEvent.abY >= maxY - border)
                value = 1.0f;

			ActionEvent actionEvent ("BORDER_TOP", value);
			shared_ptr<boost::any> data (new boost::any(actionEvent) ) ;
			shared_ptr<CMessage>  actionMessage (new CMessage( actionEventType, data ));

			try
			{
				CMessageModule::getInstancePtr()->getMessageManager()->queueMessage(actionMessage);
				GNOLL_LOG() << "Message ajoute [BORDER_TOP]\n";
			}
			catch(...)
			{
				GNOLL_LOG() << "Message NON ajoute [BORDER_TOP]\n";
			}
		}

		void CMouseMotionStateTranslator::handle ( shared_ptr<CMessage> message )
		{
			/**
			 * Get the motion information
			 */
			MouseEvent mouseMotionEvent = message->getData<MouseEvent>();

			sendLeftBorderState(mouseMotionEvent);
			sendRightBorderState(mouseMotionEvent);
            sendTopBorderState(mouseMotionEvent);
			sendBottomBorderState(mouseMotionEvent);
		}
	};
};
