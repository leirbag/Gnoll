/**************************************************************************
*   Copyright (C) 2009 by Bruno Mahe                                      *
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

#include "../include/joystickaxiseventstranslator.h"

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <OIS/OISJoyStick.h>

#include "../include/joystickaxiseventstranslator.h"
#include "../include/inputjoystickevents.h"
#include "../include/translationevents.h"
#include "../../dynamicobject/include/dynamicobjectmanager.h"
#include "../../dynamicobject/include/float.h"
#include "../../core/include/messagemodule.h"
#include "../../log/include/logmacros.h"
#include "../../config.h"
#include "../include/oisjoysticklistener.h"

using namespace Gnoll::Input;
using namespace boost;
using namespace Gnoll::Core;
using namespace Gnoll::DynamicObject;

namespace Gnoll
{
	namespace Input
	{
		JoystickAxisEventsTranslator::JoystickAxisEventsTranslator(): m_axisMoved(OISJoystickListener::MESSAGE_TYPE_JOYSTICK_AXIS_MOVED())
		{
			DynamicObjectManager *pom = DynamicObjectManager::getInstancePtr();

			/**
			 * Loading translation map : Keycode -> Action
			 */
			this->m_joystickAxisEventTranslationMap = pom->load("m_joystickAxisEventTranslationMap");

			/**
			 * Joystick config
			 */
			this->m_joystickConfig = pom->load("m_joystickConfig");
		}

		JoystickAxisEventsTranslator::~JoystickAxisEventsTranslator()
		{
		}

		void JoystickAxisEventsTranslator::initPreviousAxisValues(unsigned int _numAxis)
		{
			this->m_previousAxisValues = shared_ptr < std::vector<int> > ( new std::vector<int>(_numAxis, 0));
		}

		void JoystickAxisEventsTranslator::sendActionEventForEventAndIntensity(string _event, float _intensity)
		{
			Messages::MessageType actionEventType(ACTION_EVENT_TYPE);

			shared_ptr<Float> defaultJoystickSensibility = shared_ptr<Float> (new Float(1.0f));
			shared_ptr< Gnoll::DynamicObject::Float > joystickSensibility = m_joystickConfig->getAttributeOrDefault<Float>("sensibility", defaultJoystickSensibility);

			float intensity = _intensity * (*joystickSensibility);

			/**
			 * If an action is associated to this key code, a list of action messages is sent
			 */
			if ( m_joystickAxisEventTranslationMap->hasAttribute(_event) )
			{
				shared_ptr<List> actionList = m_joystickAxisEventTranslationMap->getAttribute<List>( _event );
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
							tmpString << "Message ajoute ["<< *actionName << "] d'intensite " << intensity;
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

		void JoystickAxisEventsTranslator::handle ( shared_ptr<Message> message )
		{
			string event("");
			float intensity = 0.0f;

			Gnoll::Input::JoystickEvent::JoystickEvent joystickAxisEvent = message->getData<Gnoll::Input::JoystickEvent::JoystickEvent>();

			/**
			 * There is no good way yet to get the number of available joystick's axis
			 */
			if (this->m_previousAxisValues.get() == NULL)
			{
				this->initPreviousAxisValues(joystickAxisEvent.event.state.mAxes.size());
			}

			int valueAxis = joystickAxisEvent.event.state.mAxes.at(joystickAxisEvent.which).abs;
			int previousAxisValue = (this->m_previousAxisValues)->at(joystickAxisEvent.which);
			float newValueAxis = (previousAxisValue - valueAxis);

			(this->m_previousAxisValues)->at(joystickAxisEvent.which) = valueAxis;

			if (newValueAxis > 0)
			{
				event = "+" + boost::lexical_cast<std::string>(joystickAxisEvent.which);
				intensity = newValueAxis /  (float)OIS::JoyStick::MAX_AXIS;

			} else
			{
				event = "-" + boost::lexical_cast<std::string>(joystickAxisEvent.which);
				intensity = newValueAxis / (float)OIS::JoyStick::MIN_AXIS;
			}

			if (event != "")
			{
				sendActionEventForEventAndIntensity(event, intensity);
			}
		}
	};
};
