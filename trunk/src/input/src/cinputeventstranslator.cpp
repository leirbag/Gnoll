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


/*------------------------CInputEventsTranslator.cpp-----------------------*\
|   The translates input events to action events used by game logic         |
|                                                                           |
|   Changelog :                                                             |
|               01/08/2008 - Bruno Mahe - Initial release                   |
|               04/13/2008 - Bruno Mahe - Read refreshing period from a     |
|                                           config file                     |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../include/cinputeventstranslator.h"
#include "../include/ckeyboardeventstranslator.h"
#include "../include/ckeyboardeventstrigger.h"
#include "../include/cmousemotioneventstranslator.h"
#include "../include/cmousebuttoneventstranslator.h"
#include "../../core/include/cmessagelistener.h"
#include "../../core/include/cmessagemodule.h"
#include "../../dynamicobject/include/dynamicobject.h"
#include "../../dynamicobject/include/dynamicobjectmanager.h"
#include "../../dynamicobject/include/integer.h"
#include "../../time/include/ctimemodule.h"
#include <iostream>


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

		}


		void CInputEventsTranslator::process()
		{	
		}


		void CInputEventsTranslator::exit()
		{
			deactivateKeyboardTranslation();
			deactivateMouseMotionTranslation();
			deactivateMouseButtonTranslation();
		}


		CInputEventsTranslator::~CInputEventsTranslator()
		{
		
		}


		void CInputEventsTranslator::activateKeyboardTranslation()
		{
		
			CMessageType keyDown("KEYBOARD_KEYDOWN");
			CMessageType keyUp("KEYBOARD_KEYUP");
			CMessageType updateKeyboard("UPDATE_KEYBOARD");


			DynamicObjectManager *dom = DynamicObjectManager::getInstancePtr();
			shared_ptr< Gnoll::DynamicObject::DynamicObject > keyboardConfig = dom->load("keyboardConfig");

			/**
			 * How often will the keyboard module get updated (millisecond)
			 */
			unsigned long int period = 300;
			shared_ptr< Integer > periodFromConfig = keyboardConfig->getAttribute<Integer>("period");
			period = *periodFromConfig;

			CMessageModule* messageModule = CMessageModule::getInstancePtr();
			CMessageManager* messageManager = messageModule->getMessageManager();


			keyboardEventsTranslator = shared_ptr<CMessageListener> ( new CKeyboardEventsTranslator() );
			keyboardEventsTrigger = shared_ptr<CMessageListener> ( new CKeyboardEventsTrigger(static_pointer_cast<CKeyboardEventsTranslator>(keyboardEventsTranslator) ));

			if (messageManager->addListener ( keyboardEventsTranslator, keyDown ) == true)
				cout << "KeyboardEventsTranslator listener installed" << endl;

			if (messageManager->addListener ( keyboardEventsTranslator, keyUp ) == true)
				cout << "KeyboardEventsTranslator listener installed" << endl;

			if (messageManager->addListener ( keyboardEventsTrigger, updateKeyboard ) == true)
				cout << "KeyboardEventsTrigger listener installed" << endl;



			CTimeModule* timeModule = CTimeModule::getInstancePtr();
			m_periodData = shared_ptr<boost::any> (new boost::any(period)) ;
			shared_ptr<CMessage>  message (new CMessage(updateKeyboard, m_periodData ));

			timeModule->addPeriodicEvent(0, message, period);


		}


		void CInputEventsTranslator::deactivateKeyboardTranslation()
		{
		
			CMessageType keyDown("KEYBOARD_KEYDOWN");
			CMessageType keyUp("KEYBOARD_KEYUP");
			CMessageType updateKeyboard("UPDATE_KEYBOARD");


			CMessageModule* messageModule = CMessageModule::getInstancePtr();
			CMessageManager* messageManager = messageModule->getMessageManager();

			if (messageManager->delListener ( keyboardEventsTranslator, keyDown ) == true)
				cout << "KeyboardEventsTranslator listener removed" << endl;

			if (messageManager->delListener ( keyboardEventsTranslator, keyUp ) == true)
				cout << "KeyboardEventsTranslator listener removed" << endl;

			if (messageManager->delListener ( keyboardEventsTrigger, updateKeyboard ) == true)
				cout << "KeyboardEventsTrigger listener removed" << endl;
		

			CTimeModule* timeModule = CTimeModule::getInstancePtr();
			shared_ptr<CMessage>  message (new CMessage(updateKeyboard, m_periodData ));
			timeModule->delPeriodicEvent(0, message, boost::any_cast<unsigned long int> (*m_periodData) );
		}


		void CInputEventsTranslator::activateMouseMotionTranslation()
		{
		
			CMessageType mouseMoved("MOUSE_MOVED");

			CMessageModule* messageModule = CMessageModule::getInstancePtr();
			CMessageManager* messageManager = messageModule->getMessageManager();


			mouseMotionEventsTranslator = shared_ptr<CMessageListener> ( new CMouseMotionEventsTranslator() );

			if (messageManager->addListener ( mouseMotionEventsTranslator, mouseMoved ) == true)
				cout << "mouseMotionEventsTranslator listener installed" << endl;

		}


		void CInputEventsTranslator::deactivateMouseMotionTranslation()
		{
		
			CMessageType mouseMoved("MOUSE_MOVED");

			CMessageModule* messageModule = CMessageModule::getInstancePtr();
			CMessageManager* messageManager = messageModule->getMessageManager();

			if (messageManager->delListener ( mouseMotionEventsTranslator, mouseMoved ) == true)
				cout << "mouseMotionEventsTranslator listener removed" << endl;

			if (messageManager->delListener ( mouseMotionEventsTranslator, mouseMoved ) == true)
				cout << "mouseMotionEventsTranslator listener removed" << endl;
		
		}


		void CInputEventsTranslator::activateMouseButtonTranslation()
		{
		
			CMessageType mousePressed("MOUSE_PRESSED");
			CMessageType mouseReleased("MOUSE_RELEASED");

			CMessageModule* messageModule = CMessageModule::getInstancePtr();
			CMessageManager* messageManager = messageModule->getMessageManager();


			mouseButtonEventsTranslator = shared_ptr<CMessageListener> ( new CMouseButtonEventsTranslator() );

			if (messageManager->addListener ( mouseButtonEventsTranslator, mousePressed ) == true)
				cout << "mouseButtonEventsTranslator listener installed for MOUSE_PRESSED" << endl;

			if (messageManager->addListener ( mouseButtonEventsTranslator, mouseReleased ) == true)
				cout << "mouseButtonEventsTranslator listener installed for MOUSE_RELEASED" << endl;
		}


		void CInputEventsTranslator::deactivateMouseButtonTranslation()
		{
		
			CMessageType mousePressed("MOUSE_PRESSED");
			CMessageType mouseReleased("MOUSE_RELEASED");

			CMessageModule* messageModule = CMessageModule::getInstancePtr();
			CMessageManager* messageManager = messageModule->getMessageManager();

			if (messageManager->delListener ( mouseButtonEventsTranslator, mousePressed ) == true)
				cout << "mouseButtonEventsTranslator listener removed for MOUSE_PRESSED" << endl;

			if (messageManager->delListener ( mouseButtonEventsTranslator, mouseReleased ) == true)
				cout << "mouseButtonEventsTranslator listener removed for MOUSE_RELEASED" << endl;
		
		}


	}
}


