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

#include "../include/mousemotionstatetranslator.h"

#include <OIS/OISMouse.h>

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include "../include/inputmouseevents.h"
#include "../include/translationevents.h"
#include "../../dynamicobject/include/dynamicobjectmanager.h"
#include "../../dynamicobject/include/float.h"
#include "../../core/include/messagemodule.h"
#include "../../log/include/logmodule.h"
#include "../../graphic/include/graphicmodule.h"
#include "../../log/include/logmacros.h"
#include "../../config.h"

using namespace boost;
using namespace Gnoll::Core;
using namespace Gnoll::Graphic;
using namespace Gnoll::DynamicObject;


namespace Gnoll
{
	namespace Input
	{

		MouseMotionStateTranslator::MouseMotionStateTranslator():
			m_mouseMovedEvent("MOUSE_MOVED")
		{
			/**
			 * Mouse config
			 */
			DynamicObjectManager *pom = DynamicObjectManager::getInstancePtr();
			this->m_mouseConfig = pom->load("mouseConfig");

            /**
             * Border data
             */
            shared_ptr<Float> defaultBorder = shared_ptr<Float> (new Float(0.0f));
			m_border = *(m_mouseConfig->getAttributeOrDefault<Float>("border", defaultBorder));

            /**
			 * Get window size
			 */
			Ogre::RenderWindow* renderWindow = GraphicModule::getInstancePtr()->getRenderWindow();
			if (renderWindow)
			{
				m_maxX = renderWindow->getWidth();
				m_maxY = renderWindow->getHeight();
			}
            else
			{
				/**
				 * Default values if we cannot get the renderWindow.
				 * This shouldn't happen
				 */
				m_maxX = 800;
				m_maxY = 600;
			}

		}

		MouseMotionStateTranslator::~MouseMotionStateTranslator()
		{
		}

		void MouseMotionStateTranslator::sendLeftBorderState(MouseEvent mouseEvent)
		{
			Messages::MessageType actionEventType(ACTION_EVENT_STATE_TYPE);

            /*
             * We send a BORDER message with intensity of (-)1 if the cursor is in the
             * border, else intensity is set up to 0
             */
            float value = 0.0f;
            if(mouseEvent.abX <= m_border)
                value = 1.0f;

			ActionEvent actionEvent ("BORDER_LEFT", value);
			shared_ptr<boost::any> data (new boost::any(actionEvent) ) ;
			shared_ptr<Message>  actionMessage (new Message( actionEventType, data ));

			try
			{
				MessageModule::getInstancePtr()->getMessageManager()->queueMessage(actionMessage);
				GNOLL_LOG() << "Message ajoute [BORDER_LEFT]\n";
			}
			catch(...)
			{
				GNOLL_LOG() << "Message NON ajoute [BORDER_LEFT]\n";
			}
		}


		void MouseMotionStateTranslator::sendRightBorderState(MouseEvent mouseEvent)
		{
			Messages::MessageType actionEventType(ACTION_EVENT_STATE_TYPE);

            /*
             * We send a BORDER message with intensity of 1 if the cursor is in the
             * border, else intensity is set up to 0
             */
            float value = 0.0f;
            if(mouseEvent.abX >= m_maxX - m_border)
                value = 1.0f;

			ActionEvent actionEvent ("BORDER_RIGHT", value);
			shared_ptr<boost::any> data (new boost::any(actionEvent) ) ;
			shared_ptr<Message>  actionMessage (new Message( actionEventType, data ));

			try
			{
				MessageModule::getInstancePtr()->getMessageManager()->queueMessage(actionMessage);
				GNOLL_LOG() << "Message ajoute [BORDER_RIGHT]\n";
			}
			catch(...)
			{
				GNOLL_LOG() << "Message NON ajoute [BORDER_RIGHT]\n";
			}
		}

        void MouseMotionStateTranslator::sendBottomBorderState(MouseEvent mouseEvent)
		{
			Messages::MessageType actionEventType(ACTION_EVENT_STATE_TYPE);

            /*
             * We send a BORDER message with intensity of 1 if the cursor is in the
             * border, else intensity is set up to 0
             */
            float value = 0.0f;
            if(mouseEvent.abY <= m_border)
                value = 1.0f;

			ActionEvent actionEvent ("BORDER_BOTTOM", value);
			shared_ptr<boost::any> data (new boost::any(actionEvent) ) ;
			shared_ptr<Message>  actionMessage (new Message( actionEventType, data ));

			try
			{
				MessageModule::getInstancePtr()->getMessageManager()->queueMessage(actionMessage);
				GNOLL_LOG() << "Message ajoute [BORDER_BOTTOM]\n";
			}
			catch(...)
			{
				GNOLL_LOG() << "Message NON ajoute [BORDER_BOTTOM]\n";
			}
		}

        void MouseMotionStateTranslator::sendTopBorderState(MouseEvent mouseEvent)
		{
			Messages::MessageType actionEventType(ACTION_EVENT_STATE_TYPE);

            /*
             * We send a BORDER message with intensity of 1 if the cursor is in the
             * border, else intensity is set up to 0
             */
            float value = 0.0f;
            if(mouseEvent.abY >= m_maxY - m_border)
                value = 1.0f;

			ActionEvent actionEvent ("BORDER_TOP", value);
			shared_ptr<boost::any> data (new boost::any(actionEvent) ) ;
			shared_ptr<Message>  actionMessage (new Message( actionEventType, data ));

			try
			{
				MessageModule::getInstancePtr()->getMessageManager()->queueMessage(actionMessage);
				GNOLL_LOG() << "Message ajoute [BORDER_TOP]\n";
			}
			catch(...)
			{
				GNOLL_LOG() << "Message NON ajoute [BORDER_TOP]\n";
			}
		}

		void MouseMotionStateTranslator::handle ( shared_ptr<Message> message )
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
