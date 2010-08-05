/***************************************************************************
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

#include "../include/inputeventstranslator.h"

#include "../include/keyboardeventstranslator.h"
#include "../include/keyboardeventstrigger.h"
#include "../include/keyboardstatetranslator.h"
#include "../include/mousemotioneventstranslator.h"
#include "../include/mousemotionstatetranslator.h"
#include "../include/mousebuttoneventstranslator.h"
#include "../include/mousebuttoneventstrigger.h"
#include "../include/mousebuttonstatetranslator.h"
#include "../include/joystickaxiseventstranslator.h"
#include "../include/joystickaxisstatetranslator.h"
#include "../include/rulemanager.h"
#include "../include/oisinputmanager.h"
#include "../../core/messages/include/listener.h"
#include "../../core/include/messagemodule.h"
#include "../../dynamicobject/include/dynamicobject.h"
#include "../../dynamicobject/include/dynamicobjectmanager.h"
#include "../../dynamicobject/include/integer.h"
#include "../../time/include/timemodule.h"
#include "../../log/include/logmacros.h"
#include "../include/oisjoysticklistener.h"

using namespace Gnoll::Input;
using namespace Gnoll::Core;
using namespace Gnoll::DynamicObject;
using namespace Gnoll::Time;
using namespace boost;

namespace Gnoll
{
	namespace Input
	{
		InputEventsTranslator::InputEventsTranslator()
		{
		}

		void InputEventsTranslator::init()
		{
			activateKeyboardTranslation();
			activateMouseMotionTranslation();
			activateMouseButtonTranslation();
			activateJoystickAxisTranslation();
			activateRuleManager();
		}

		void InputEventsTranslator::process()
		{
		}

		void InputEventsTranslator::exit()
		{
			deactivateKeyboardTranslation();
			deactivateMouseMotionTranslation();
			deactivateMouseButtonTranslation();
			deactivateJoystickAxisTranslation();
		}

		InputEventsTranslator::~InputEventsTranslator()
		{
		}

		void InputEventsTranslator::activateKeyboardTranslation()
		{
			Messages::MessageType keyDown("KEYBOARD_KEYDOWN");
			Messages::MessageType keyUp("KEYBOARD_KEYUP");
			Messages::MessageType updateKeyboard("UPDATE_KEYBOARD");

			DynamicObjectManager *dom = DynamicObjectManager::getInstancePtr();
			shared_ptr< Gnoll::DynamicObject::DynamicObject > keyboardConfig = dom->load("keyboardConfig");

			/**
			 * How often will the keyboard module get updated (millisecond)
			 */
			unsigned long int period = 300;
			shared_ptr<Gnoll::DynamicObject::Integer> periodFromConfig = keyboardConfig->getAttribute<Gnoll::DynamicObject::Integer>("period");
			period = *periodFromConfig;

			MessageModule* messageModule = MessageModule::getInstancePtr();
			Messages::Messenger* messageManager = messageModule->getMessageManager();

			keyboardEventsTranslator = shared_ptr<Messages::Listener>(new KeyboardEventsTranslator() );
			keyboardEventsTrigger = shared_ptr<Messages::Listener>(new KeyboardEventsTrigger(static_pointer_cast<KeyboardEventsTranslator>(keyboardEventsTranslator) ));
			keyboardStateTranslator = shared_ptr<Messages::Listener>(new KeyboardStateTranslator() );

			try
			{
				/**
				 * Continuous keyboard messages
				 */
				messageManager->addListener(keyboardEventsTranslator, keyDown);
				GNOLL_LOG() << "KeyboardEventsTranslator listener installed";

				messageManager->addListener(keyboardEventsTranslator, keyUp);
				GNOLL_LOG() << "KeyboardEventsTranslator listener installed";

				messageManager->addListener(keyboardEventsTrigger, updateKeyboard);
				GNOLL_LOG() << "KeyboardEventsTrigger listener installed";

				/**
				 * State based keyboard messages
				 */
				messageManager->addListener(keyboardStateTranslator, keyDown);
				GNOLL_LOG() << "KeyboardStateTranslator listener installed";

				messageManager->addListener(keyboardStateTranslator, keyUp);
				GNOLL_LOG() << "KeyboardStateTranslator listener installed";
			}
			catch(...)
			{
				throw;
			}

			TimeModule* timeModule = TimeModule::getInstancePtr();
			m_periodData = shared_ptr<boost::any> (new boost::any(period)) ;
			shared_ptr<Message>  message (new Message(updateKeyboard, m_periodData));

			timeModule->addPeriodicEvent(0, message, period);
		}

		void InputEventsTranslator::deactivateKeyboardTranslation()
		{
			Messages::MessageType keyDown("KEYBOARD_KEYDOWN");
			Messages::MessageType keyUp("KEYBOARD_KEYUP");
			Messages::MessageType updateKeyboard("UPDATE_KEYBOARD");

			MessageModule* messageModule = MessageModule::getInstancePtr();
			Messages::Messenger* messageManager = messageModule->getMessageManager();

			try
			{
				/**
				 * Continuous keybord messages
				 */
				messageManager->delListener(keyboardEventsTranslator, keyDown);
				GNOLL_LOG() << "KeyboardEventsTranslator listener removed";

				messageManager->delListener(keyboardEventsTranslator, keyUp);
				GNOLL_LOG() << "KeyboardEventsTranslator listener removed";

				messageManager->delListener(keyboardEventsTrigger, updateKeyboard);
				GNOLL_LOG() << "KeyboardEventsTrigger listener removed";

				/**
				 * State based keyboard messages
				 */
				messageManager->delListener(keyboardStateTranslator, keyDown);
				GNOLL_LOG() << "KeyboardStateTranslator listener removed";

				messageManager->delListener(keyboardStateTranslator, keyUp);
				GNOLL_LOG() << "KeyboardStateTranslator listener removed";
			}
			catch(...)
			{
				throw;
			}

			TimeModule* timeModule = TimeModule::getInstancePtr();
			shared_ptr<Message>  message (new Message(updateKeyboard, m_periodData));
			timeModule->delPeriodicEvent(0, message, boost::any_cast<unsigned long int> (*m_periodData));
		}


		void InputEventsTranslator::activateMouseMotionTranslation()
		{
			Messages::MessageType mouseMoved("MOUSE_MOVED");

			MessageModule* messageModule = MessageModule::getInstancePtr();
			Messages::Messenger* messageManager = messageModule->getMessageManager();

			mouseMotionEventsTranslator = shared_ptr<Messages::Listener>(new MouseMotionEventsTranslator());
			mouseMotionStateTranslator  = shared_ptr<Messages::Listener>(new MouseMotionStateTranslator());

			try
			{
				messageManager->addListener(mouseMotionEventsTranslator, mouseMoved);
				GNOLL_LOG() << "mouseMotionEventsTranslator listener installed";

				messageManager->addListener(mouseMotionStateTranslator, mouseMoved);
				GNOLL_LOG() << "mouseMotionStateTranslator listener installed";
			}
			catch(...)
			{
				throw;
			}
		}

		void InputEventsTranslator::deactivateMouseMotionTranslation()
		{
			Messages::MessageType mouseMoved("MOUSE_MOVED");
			MessageModule* messageModule = MessageModule::getInstancePtr();
			Messages::Messenger* messageManager = messageModule->getMessageManager();

			try
			{
				messageManager->delListener(mouseMotionEventsTranslator, mouseMoved);
				GNOLL_LOG() << "mouseMotionEventsTranslator listener removed";

				messageManager->delListener(mouseMotionStateTranslator, mouseMoved);
				GNOLL_LOG() << "mouseMotionStateTranslator listener removed";
			}
			catch(...)
			{
				throw;
			}
		}

		void InputEventsTranslator::activateMouseButtonTranslation()
		{
			Messages::MessageType mousePressed("MOUSE_PRESSED");
			Messages::MessageType mouseReleased("MOUSE_RELEASED");
			Messages::MessageType updateMouse("UPDATE_MOUSE");

			DynamicObjectManager *dom = DynamicObjectManager::getInstancePtr();
			shared_ptr< Gnoll::DynamicObject::DynamicObject > mouseConfig = dom->load("mouseConfig");

			/**
			 * How often will the mouse button module get updated (millisecond)
			 */
			unsigned long int period = 300;
			shared_ptr< Gnoll::DynamicObject::Integer > periodFromConfig = mouseConfig->getAttribute< Gnoll::DynamicObject::Integer >("period");
			period = *periodFromConfig;


			MessageModule* messageModule = MessageModule::getInstancePtr();
			Messages::Messenger* messageManager = messageModule->getMessageManager();


			mouseButtonEventsTranslator = shared_ptr<Messages::Listener>(new MouseButtonEventsTranslator() );
			mouseButtonEventsTrigger = shared_ptr<Messages::Listener>(new MouseButtonEventsTrigger(static_pointer_cast<MouseButtonEventsTranslator>(mouseButtonEventsTranslator)));

			mouseButtonStateTranslator = shared_ptr<Messages::Listener>(new MouseButtonStateTranslator() );

			try
			{
				/**
				 * Continuous mouse messages
				 */
				messageManager->addListener(mouseButtonEventsTranslator, mousePressed );
				GNOLL_LOG() << "mouseButtonEventsTranslator listener installed for MOUSE_PRESSED";

				messageManager->addListener(mouseButtonEventsTranslator, mouseReleased);
				GNOLL_LOG() << "mouseButtonEventsTranslator listener installed for MOUSE_RELEASED";

				messageManager->addListener(mouseButtonEventsTrigger, updateMouse);
				Gnoll::Log::LogModule::getInstancePtr()->logMessage("mouseButtonEventsTrigger listener installed");

				/**
				 * State based mouse messages
				 */
				messageManager->addListener(mouseButtonStateTranslator, mousePressed);
				Gnoll::Log::LogModule::getInstancePtr()->logMessage("mouseButtonStateTranslator listener installed for MOUSE_PRESSED");

				messageManager->addListener(mouseButtonStateTranslator, mouseReleased);
				Gnoll::Log::LogModule::getInstancePtr()->logMessage("mouseButtonStateTranslator listener installed for MOUSE_RELEASED");
			}
			catch(...)
			{
				throw;
			}


			TimeModule* timeModule = TimeModule::getInstancePtr();
			m_periodData = shared_ptr<boost::any> (new boost::any(period)) ;
			shared_ptr<Message>  message (new Message(updateMouse, m_periodData));

			timeModule->addPeriodicEvent(0, message, period);
		}

		void InputEventsTranslator::deactivateMouseButtonTranslation()
		{
			Messages::MessageType mousePressed("MOUSE_PRESSED");
			Messages::MessageType mouseReleased("MOUSE_RELEASED");
			Messages::MessageType updateMouse("UPDATE_MOUSE");

			MessageModule* messageModule = MessageModule::getInstancePtr();
			Messages::Messenger* messageManager = messageModule->getMessageManager();

			try
			{
				/**
				 * Continuous mouse messages
				 */
				messageManager->delListener(mouseButtonEventsTranslator, mousePressed);
				Gnoll::Log::LogModule::getInstancePtr()->logMessage("mouseButtonEventsTranslator listener removed for MOUSE_PRESSED");

				messageManager->delListener(mouseButtonEventsTranslator, mouseReleased);
				Gnoll::Log::LogModule::getInstancePtr()->logMessage("mouseButtonEventsTranslator listener removed for MOUSE_RELEASED");

				messageManager->delListener(mouseButtonEventsTrigger, updateMouse);
				Gnoll::Log::LogModule::getInstancePtr()->logMessage("mouseButtonEventsTrigger listener removed");

				/**
				 * State based mouse messages
				 */
				messageManager->delListener(mouseButtonStateTranslator, mousePressed);
				Gnoll::Log::LogModule::getInstancePtr()->logMessage("mouseButtonStateTranslator listener removed for MOUSE_PRESSED");

				messageManager->delListener(mouseButtonStateTranslator, mouseReleased);
				Gnoll::Log::LogModule::getInstancePtr()->logMessage("mouseButtonStateTranslator listener removed for MOUSE_RELEASED");
			}
			catch(...)
			{
				throw;
			}

			TimeModule* timeModule = TimeModule::getInstancePtr();
			shared_ptr<Message>  message (new Message(updateMouse, m_periodData));
			timeModule->delPeriodicEvent(0, message, boost::any_cast<unsigned long int> (*m_periodData));
		}

		void InputEventsTranslator::activateJoystickAxisTranslation()
		{
			Messages::MessageType axisEventType(OISJoystickListener::MESSAGE_TYPE_JOYSTICK_AXIS_MOVED());

			MessageModule* messageModule = MessageModule::getInstancePtr();
			Messages::Messenger* messageManager = messageModule->getMessageManager();

			joystickAxisEventsTranslator = shared_ptr<Messages::Listener>(new JoystickAxisEventsTranslator());
			joystickAxisStateTranslator  = shared_ptr<Messages::Listener>(new JoystickAxisStateTranslator());

			try
			{
				messageManager->addListener(joystickAxisEventsTranslator, axisEventType);
				GNOLL_LOG() << "joystickAxisEventsTranslator listener installed";

				messageManager->addListener(joystickAxisStateTranslator, axisEventType);
				GNOLL_LOG() << "joystickAxisStateTranslator listener installed";
			}
			catch(...)
			{
				throw;
			}

		}

		void InputEventsTranslator::deactivateJoystickAxisTranslation()
		{
			Messages::MessageType axisEventType(OISJoystickListener::MESSAGE_TYPE_JOYSTICK_AXIS_MOVED());

			MessageModule* messageModule = MessageModule::getInstancePtr();
			Messages::Messenger* messageManager = messageModule->getMessageManager();

			try
			{
				messageManager->delListener(joystickAxisEventsTranslator, axisEventType);
				GNOLL_LOG() << "joystickAxisEventsTranslator listener removed";

				messageManager->delListener(joystickAxisStateTranslator, axisEventType);
				GNOLL_LOG() << "joystickAxisStateTranslator listener removed";
			}
			catch(...)
			{
				throw;
			}
		}

		void InputEventsTranslator::activateRuleManager()
		{
			ruleManager = shared_ptr<Messages::Listener>( new RuleManager("ruleManager"));
			MessageModule* messageModule = MessageModule::getInstancePtr();
			Messages::Messenger* messageManager = messageModule->getMessageManager();

			try
			{
				messageManager->addListener(ruleManager, ACTION_EVENT_TYPE);
				GNOLL_LOG() << "ruleManager installed for ACTION_EVENT_TYPE";

				messageManager->addListener(ruleManager, ACTION_EVENT_STATE_TYPE);
				GNOLL_LOG() << "ruleManager installed for ACTION_EVENT_STATE_TYPE";
			}
			catch(...)
			{
				throw;
			}
		}


		void InputEventsTranslator::deactivateRuleManager()
		{
			MessageModule* messageModule = MessageModule::getInstancePtr();
			Messages::Messenger* messageManager = messageModule->getMessageManager();

			try
			{
				messageManager->delListener(ruleManager, ACTION_EVENT_TYPE);
				GNOLL_LOG() << "ruleManager removed for ACTION_EVENT_TYPE";

				messageManager->delListener(ruleManager, ACTION_EVENT_STATE_TYPE);
				GNOLL_LOG() << "ruleManager removed for ACTION_EVENT_STATE_TYPE";
			}
			catch(...)
			{
				throw;
			}
		}
	}
}
