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

#include "../include/mousemotioneventstranslator.h"

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <OIS/OISMouse.h>
#include <OgreRenderWindow.h>

#include "../include/inputmouseevents.h"
#include "../include/translationevents.h"
#include "../../dynamicobject/include/dynamicobjectmanager.h"
#include "../../dynamicobject/include/float.h"
#include "../../core/include/messagemodule.h"
#include "../../log/include/logmodule.h"
#include "../../graphic/include/graphicmodule.h"
#include "../../config.h"

using namespace boost;
using namespace Gnoll::Core;
using namespace Gnoll::DynamicObject;
using namespace Gnoll::Graphic;

namespace Gnoll
{
	namespace Input 
	{
		MouseMotionEventsTranslator::MouseMotionEventsTranslator(): m_mouseMoved("MOUSE_MOVED")
		{
			DynamicObjectManager *pom = DynamicObjectManager::getInstancePtr();

			/**
			 * Loading translation map : Keycode -> Action
			 */
			this->m_mouseMotionEventTranslationMap = pom->load("m_mouseMotionEventTranslationMap");

			/**
			 * Mouse config
			 */
			this->m_mouseConfig = pom->load("m_mouseConfig");
			Ogre::RenderWindow* renderWindow = GraphicModule::getInstancePtr()->getRenderWindow();

			if (renderWindow)
			{
				m_maxX = renderWindow->getWidth();
				m_maxY = renderWindow->getHeight();

			} else
			{
				/**
				 * Default values if we cannot get the renderWindow.
				 * This shouldn't happen
				 */
				m_maxX = 800;
				m_maxY = 600;

			}
		}

		MouseMotionEventsTranslator::~MouseMotionEventsTranslator()
		{
		}

		void MouseMotionEventsTranslator::sendActionEventForEventAndIntensity(string _event, float _intensity)
		{
			Messages::MessageType actionEventType(ACTION_EVENT_TYPE);

			shared_ptr<Float> defaultMouseSensibility = shared_ptr<Float> (new Float(1.0f));
			shared_ptr< Gnoll::DynamicObject::Float > mouseSensibility = m_mouseConfig->getAttributeOrDefault<Float>("sensibility", defaultMouseSensibility);

			float intensity = _intensity * (*mouseSensibility);

			/**
			 * If an action is associated to this key code, a list of action messages is sent
			 */
			if ( m_mouseMotionEventTranslationMap->hasAttribute(_event) )
			{
				shared_ptr<List> actionList = m_mouseMotionEventTranslationMap->getAttribute<List>( _event );
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
							tmpString << "Message ajoute ["<< *actionName << "]";
						}
						catch(...)
						{
							tmpString << "Message NON ajoute ["<< *actionName << "]" << endl;
						}
						Gnoll::Log::LogModule::getInstancePtr()->logMessage( tmpString.str() );
					}
				}
			}	
		}

		void MouseMotionEventsTranslator::sendXMotionEvents(MouseEvent _mouseEvent)
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

				intensity = _mouseEvent.relX / m_maxX;
			}

			if (event != "")
			{
				sendActionEventForEventAndIntensity(event, intensity);
			}
		}

		void MouseMotionEventsTranslator::sendYMotionEvents(MouseEvent _mouseEvent)
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

				intensity = _mouseEvent.relY / m_maxY;
			}

			if (event != "")
			{
				sendActionEventForEventAndIntensity(event, intensity);
			}
		}

		void MouseMotionEventsTranslator::sendZMotionEvents(MouseEvent _mouseEvent)
		{
			string event("");
			float intensity = 0.0f;

			if (_mouseEvent.relZ != 0)	
			{
				if (_mouseEvent.relZ > 0)
				{
					event = "+Z";

				} else
				{
					event = "-Z";
				}

				intensity = _mouseEvent.relZ;
			}

			if (event != "")
			{
				sendActionEventForEventAndIntensity(event, intensity);
			}
		}

		void MouseMotionEventsTranslator::handle ( shared_ptr<Message> message )
		{
			/**
			 * Get the motion information
			 */
			MouseEvent mouseMotionEvent = message->getData<MouseEvent>();

			sendXMotionEvents( mouseMotionEvent );
			sendYMotionEvents( mouseMotionEvent );
			sendZMotionEvents( mouseMotionEvent );
		}
	};
};
