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

#include "../include/cinputeventstranslator.h"
#include "../include/ckeyboardeventstranslator.h"
#include "../include/ckeyboardeventstrigger.h"
#include "../include/ckeyboardstatetranslator.h"
#include "../include/cmousemotioneventstranslator.h"
#include "../include/cmousemotionstatetranslator.h"
#include "../include/cmousebuttoneventstranslator.h"
#include "../include/cmousebuttoneventstrigger.h"
#include "../include/cmousebuttonstatetranslator.h"
#include "../include/joystickaxiseventstranslator.h"
#include "../include/joystickaxisstatetranslator.h"
#include "../include/crulemanager.h"
#include "../include/coisinputmanager.h"
#include "../../core/messages/include/listener.h"
#include "../../core/include/messagemodule.h"
#include "../../dynamicobject/include/dynamicobject.h"
#include "../../dynamicobject/include/dynamicobjectmanager.h"
#include "../../dynamicobject/include/integer.h"
#include "../../time/include/ctimemodule.h"
#include "../../log/include/logmodule.h"
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
		CInputEventsTranslator::CInputEventsTranslator()
		{
		}

		void CInputEventsTranslator::init()
		{
			activateKeyboardTranslation();
			activateMouseMotionTranslation();
			activateMouseButtonTranslation();
			activateJoystickAxisTranslation();
			activateRuleManager();
		}

		void CInputEventsTranslator::process()
		{
		}

		void CInputEventsTranslator::exit()
		{
			deactivateKeyboardTranslation();
			deactivateMouseMotionTranslation();
			deactivateMouseButtonTranslation();
			deactivateJoystickAxisTranslation();
		}


		CInputEventsTranslator::~CInputEventsTranslator()
		{
		}


		void CInputEventsTranslator::activateKeyboardTranslation()
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
			shared_ptr< Gnoll::DynamicObject::Integer > periodFromConfig = keyboardConfig->getAttribute< Gnoll::DynamicObject::Integer >("period");
			period = *periodFromConfig;

			MessageModule* messageModule = MessageModule::getInstancePtr();
			Messages::Messenger* messageManager = messageModule->getMessageManager();


			keyboardEventsTranslator = shared_ptr<Messages::Listener> ( new CKeyboardEventsTranslator() );
			keyboardEventsTrigger = shared_ptr<Messages::Listener> ( new CKeyboardEventsTrigger(static_pointer_cast<CKeyboardEventsTranslator>(keyboardEventsTranslator) ));
			keyboardStateTranslator = shared_ptr<Messages::Listener> ( new CKeyboardStateTranslator() );

			try
			{
				/**
				 * Continuous keyboard messages
				 */
				messageManager->addListener ( keyboardEventsTranslator, keyDown );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "KeyboardEventsTranslator listener installed" );

				messageManager->addListener ( keyboardEventsTranslator, keyUp );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "KeyboardEventsTranslator listener installed" );

				messageManager->addListener ( keyboardEventsTrigger, updateKeyboard );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "KeyboardEventsTrigger listener installed" );

				/**
				 * State based keyboard messages
				 */
				messageManager->addListener ( keyboardStateTranslator, keyDown );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "KeyboardStateTranslator listener installed" );

				messageManager->addListener ( keyboardStateTranslator, keyUp );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "KeyboardStateTranslator listener installed" );
			}
			catch(...)
			{
				throw;
			}

			CTimeModule* timeModule = CTimeModule::getInstancePtr();
			m_periodData = shared_ptr<boost::any> (new boost::any(period)) ;
			shared_ptr<Message>  message (new Message(updateKeyboard, m_periodData ));

			timeModule->addPeriodicEvent(0, message, period);
		}

		void CInputEventsTranslator::deactivateKeyboardTranslation()
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
				messageManager->delListener ( keyboardEventsTranslator, keyDown );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "KeyboardEventsTranslator listener removed" );

				messageManager->delListener ( keyboardEventsTranslator, keyUp );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "KeyboardEventsTranslator listener removed" );

				messageManager->delListener ( keyboardEventsTrigger, updateKeyboard );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "KeyboardEventsTrigger listener removed" );

				/**
				 * State based keyboard messages
				 */
				messageManager->delListener ( keyboardStateTranslator, keyDown );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "KeyboardStateTranslator listener removed" );

				messageManager->delListener ( keyboardStateTranslator, keyUp );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "KeyboardStateTranslator listener removed" );
			}
			catch(...)
			{
				throw;
			}


			CTimeModule* timeModule = CTimeModule::getInstancePtr();
			shared_ptr<Message>  message (new Message(updateKeyboard, m_periodData ));
			timeModule->delPeriodicEvent(0, message, boost::any_cast<unsigned long int> (*m_periodData) );
		}


		void CInputEventsTranslator::activateMouseMotionTranslation()
		{

			Messages::MessageType mouseMoved("MOUSE_MOVED");

			MessageModule* messageModule = MessageModule::getInstancePtr();
			Messages::Messenger* messageManager = messageModule->getMessageManager();

			mouseMotionEventsTranslator = shared_ptr<Messages::Listener> ( new CMouseMotionEventsTranslator() );
			mouseMotionStateTranslator  = shared_ptr<Messages::Listener> ( new CMouseMotionStateTranslator() );

			try
			{
				messageManager->addListener ( mouseMotionEventsTranslator, mouseMoved );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "mouseMotionEventsTranslator listener installed" );

				messageManager->addListener ( mouseMotionStateTranslator, mouseMoved );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "mouseMotionStateTranslator listener installed" );
			}
			catch(...)
			{
				throw;
			}

		}


		void CInputEventsTranslator::deactivateMouseMotionTranslation()
		{

			Messages::MessageType mouseMoved("MOUSE_MOVED");

			MessageModule* messageModule = MessageModule::getInstancePtr();
			Messages::Messenger* messageManager = messageModule->getMessageManager();

			try
			{
				messageManager->delListener ( mouseMotionEventsTranslator, mouseMoved );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "mouseMotionEventsTranslator listener removed" );

				messageManager->delListener ( mouseMotionStateTranslator, mouseMoved );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "mouseMotionStateTranslator listener removed" );
			}
			catch(...)
			{
				throw;
			}
		}

		void CInputEventsTranslator::activateMouseButtonTranslation()
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


			mouseButtonEventsTranslator = shared_ptr<Messages::Listener> ( new CMouseButtonEventsTranslator() );
			mouseButtonEventsTrigger = shared_ptr<Messages::Listener> ( new CMouseButtonEventsTrigger(static_pointer_cast<CMouseButtonEventsTranslator>(mouseButtonEventsTranslator) ) );

			mouseButtonStateTranslator = shared_ptr<Messages::Listener> ( new CMouseButtonStateTranslator() );

			try
			{
				/**
				 * Continuous mouse messages
				 */
				messageManager->addListener ( mouseButtonEventsTranslator, mousePressed );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "mouseButtonEventsTranslator listener installed for MOUSE_PRESSED" );

				messageManager->addListener ( mouseButtonEventsTranslator, mouseReleased );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "mouseButtonEventsTranslator listener installed for MOUSE_RELEASED" );

				messageManager->addListener ( mouseButtonEventsTrigger, updateMouse );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage ( "mouseButtonEventsTrigger listener installed" );

				/**
				 * State based mouse messages
				 */
				messageManager->addListener ( mouseButtonStateTranslator, mousePressed );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage ( "mouseButtonStateTranslator listener installed for MOUSE_PRESSED" );

				messageManager->addListener ( mouseButtonStateTranslator, mouseReleased );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage ( "mouseButtonStateTranslator listener installed for MOUSE_RELEASED" );
			}
			catch(...)
			{
				throw;
			}


			CTimeModule* timeModule = CTimeModule::getInstancePtr();
			m_periodData = shared_ptr<boost::any> (new boost::any(period)) ;
			shared_ptr<Message>  message (new Message(updateMouse, m_periodData ));

			timeModule->addPeriodicEvent(0, message, period);
		}


		void CInputEventsTranslator::deactivateMouseButtonTranslation()
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
				messageManager->delListener ( mouseButtonEventsTranslator, mousePressed );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage ( "mouseButtonEventsTranslator listener removed for MOUSE_PRESSED" );

				messageManager->delListener ( mouseButtonEventsTranslator, mouseReleased );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage ( "mouseButtonEventsTranslator listener removed for MOUSE_RELEASED" );

				messageManager->delListener ( mouseButtonEventsTrigger, updateMouse );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage ( "mouseButtonEventsTrigger listener removed" );

				/**
				 * State based mouse messages
				 */
				messageManager->delListener ( mouseButtonStateTranslator, mousePressed );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage ( "mouseButtonStateTranslator listener removed for MOUSE_PRESSED" );

				messageManager->delListener ( mouseButtonStateTranslator, mouseReleased );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage ( "mouseButtonStateTranslator listener removed for MOUSE_RELEASED" );
			}
			catch(...)
			{
				throw;
			}

			CTimeModule* timeModule = CTimeModule::getInstancePtr();
			shared_ptr<Message>  message (new Message(updateMouse, m_periodData ));
			timeModule->delPeriodicEvent(0, message, boost::any_cast<unsigned long int> (*m_periodData) );

		}



		void CInputEventsTranslator::activateJoystickAxisTranslation()
		{

			Messages::MessageType axisEventType(OISJoystickListener::MESSAGE_TYPE_JOYSTICK_AXIS_MOVED());

			MessageModule* messageModule = MessageModule::getInstancePtr();
			Messages::Messenger* messageManager = messageModule->getMessageManager();

			joystickAxisEventsTranslator = shared_ptr<Messages::Listener> ( new JoystickAxisEventsTranslator() );
			joystickAxisStateTranslator  = shared_ptr<Messages::Listener> ( new JoystickAxisStateTranslator() );

			try
			{
				messageManager->addListener ( joystickAxisEventsTranslator, axisEventType );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "joystickAxisEventsTranslator listener installed" );

				messageManager->addListener ( joystickAxisStateTranslator, axisEventType );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "joystickAxisStateTranslator listener installed" );
			}
			catch(...)
			{
				throw;
			}

		}


		void CInputEventsTranslator::deactivateJoystickAxisTranslation()
		{

			Messages::MessageType axisEventType(OISJoystickListener::MESSAGE_TYPE_JOYSTICK_AXIS_MOVED());

			MessageModule* messageModule = MessageModule::getInstancePtr();
			Messages::Messenger* messageManager = messageModule->getMessageManager();

			try
			{
				messageManager->delListener ( joystickAxisEventsTranslator, axisEventType );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "joystickAxisEventsTranslator listener removed" );

				messageManager->delListener ( joystickAxisStateTranslator, axisEventType );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "joystickAxisStateTranslator listener removed" );
			}
			catch(...)
			{
				throw;
			}
		}


		void CInputEventsTranslator::activateRuleManager()
		{
			ruleManager = shared_ptr<Messages::Listener>( new CRuleManager("ruleManager") );


			MessageModule* messageModule = MessageModule::getInstancePtr();
			Messages::Messenger* messageManager = messageModule->getMessageManager();

			try
			{
				messageManager->addListener ( ruleManager, ACTION_EVENT_TYPE );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "ruleManager installed for ACTION_EVENT_TYPE" );

				messageManager->addListener ( ruleManager, ACTION_EVENT_STATE_TYPE );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "ruleManager installed for ACTION_EVENT_STATE_TYPE" );
			}
			catch(...)
			{
				throw;
			}
		}


		void CInputEventsTranslator::deactivateRuleManager()
		{

			MessageModule* messageModule = MessageModule::getInstancePtr();
			Messages::Messenger* messageManager = messageModule->getMessageManager();

			try
			{
				messageManager->delListener ( ruleManager, ACTION_EVENT_TYPE );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "ruleManager removed for ACTION_EVENT_TYPE" );

				messageManager->delListener ( ruleManager, ACTION_EVENT_STATE_TYPE );
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "ruleManager removed for ACTION_EVENT_STATE_TYPE" );
			}
			catch(...)
			{
				throw;
			}
		}
	}
}


