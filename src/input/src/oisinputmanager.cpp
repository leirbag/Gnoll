/***************************************************************************
 *   Copyright (C) 2006 by Puzzle Team                                     *
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

#include "../include/oisinputmanager.h"

#include <string>
#include <boost/shared_ptr.hpp>

#include "../include/inputmouseevents.h"
#include "../include/inputjoystickevents.h"
#include "../../core/include/messagemodule.h"
#include "../../core/include/message.h"
#include "../../log/include/logmacros.h"
#include "../include/oisjoysticklistener.h"
#include "../../config.h"

using namespace Gnoll::Input;
using namespace Gnoll::Graphic;
using namespace Gnoll::Core;

OISInputManager::OISInputManager( ) :
    m_inputSystem( 0 ) , m_keyboard( 0 ), m_mouse( 0 ), m_joystick( 0 ), m_mouseButtonsState( 0 )
{
}

OISInputManager::~OISInputManager( void )
{
	if( m_inputSystem )
	{
		if( m_keyboard )
		{
			m_inputSystem->destroyInputObject( m_keyboard );
			m_keyboard = 0;
		}

		if( m_mouse )
		{
			m_inputSystem->destroyInputObject( m_mouse );
			m_mouse = 0;
		}

		if( m_joystick )
		{
			m_inputSystem->destroyInputObject( m_joystick );
			m_joystick = 0;
			delete this->m_joystickListener;
		}

		Gnoll::Log::LogModule::getInstancePtr()->logMessage( "Destroying OIS" );
		OIS::InputManager::destroyInputSystem( m_inputSystem );
		m_inputSystem = 0;
	}
}

void OISInputManager::initialise(  )
{
	if( !m_inputSystem )
	{
		// Setup basic variables
		OIS::ParamList paramList;
		size_t windowHnd = 0;
		std::ostringstream windowHndStr;
		windowHnd = GraphicModule::getInstancePtr()->getWindowHandle();

		// Fill parameter list
		windowHndStr << (unsigned int) windowHnd;
		paramList.insert( std::make_pair( std::string( "WINDOW" ), windowHndStr.str() ) );

		// Create inputsystem
		m_inputSystem = OIS::InputManager::createInputSystem( paramList );

		/**
		 * Get number of available keyboard
		 *
		 * OIS::InputManager::numKeyBoards has been renamed to
		 * OIS::InputManager::num_keyboards from OIS 1.0
		 * But the old one is still used in OIS 1.0rc which is shiped
		 * with some GNU/Linux distribution
		 */
#ifdef HAVE_OIS_1RC
		int num_keyboards = m_inputSystem->numKeyBoards();
#elif defined HAVE_OIS_1_2
		int num_keyboards = m_inputSystem->getNumberOfDevices( OIS::OISKeyboard );
#else
		int num_keyboards = m_inputSystem->num_keyboards();
#endif

		// If possible create a buffered keyboard
		if( num_keyboards > 0 )
		{
			m_keyboard = static_cast<OIS::Keyboard*>( m_inputSystem->createInputObject( OIS::OISKeyboard, true ) );
			m_keyboard->setEventCallback( this );
		}

		// If possible create a buffered mouse
#ifdef HAVE_OIS_1_2
		if( m_inputSystem->getNumberOfDevices( OIS::OISMouse ) > 0 )
		{
#else
		if( m_inputSystem->numMice() > 0 )
		{
#endif
			m_mouse = static_cast<OIS::Mouse*>( m_inputSystem->createInputObject( OIS::OISMouse, true ) );
			m_mouse->setEventCallback( this );

			Ogre::RenderWindow* renderWindow = GraphicModule::getInstancePtr()->getRenderWindow();

			if (renderWindow)
			{
				const OIS::MouseState& mstate = m_mouse->getMouseState();
				mstate.width = renderWindow->getWidth();
				mstate.height = renderWindow->getHeight();
			}
		}

		// If possible create a buffered joystick
#ifdef HAVE_OIS_1_2
		GNOLL_LOG() << "JOYSTICK: " << m_inputSystem->getNumberOfDevices( OIS::OISJoyStick ) << " detected\n";
		if( m_inputSystem->getNumberOfDevices( OIS::OISJoyStick ) > 0 )
		{
#else
		GNOLL_LOG() << "JOYSTICK: " << m_inputSystem->numJoySticks() << " detected\n";
		if( m_inputSystem->numJoySticks() > 0 )
		{
#endif
			m_joystick = static_cast<OIS::JoyStick*>( m_inputSystem->createInputObject( OIS::OISJoyStick, true ) );

			GNOLL_LOG() << "JOYSTICK: Joystick found\n";

#ifdef HAVE_OIS_1_2
			GNOLL_LOG() << "JOYSTICK: " << m_joystick->getNumberOfComponents(OIS::OIS_Button)  << " buttons detected\n";
			GNOLL_LOG() << "JOYSTICK: " << m_joystick->getNumberOfComponents(OIS::OIS_Axis) << " axes detected\n";
			GNOLL_LOG() << "JOYSTICK: " << m_joystick->getNumberOfComponents(OIS::OIS_Slider) << " sliders detected\n";
			GNOLL_LOG() << "JOYSTICK: " << m_joystick->getNumberOfComponents(OIS::OIS_POV) << " pov detected\n";
#else
			GNOLL_LOG() << "JOYSTICK: " << m_joystick->axes() << " axes detected\n";
			GNOLL_LOG() << "JOYSTICK: " << m_joystick->buttons() << " buttons detected\n";
			GNOLL_LOG() << "JOYSTICK: " << m_joystick->hats() << " hats detected\n";
#endif

			this->m_joystickListener = new Gnoll::Input::OISJoystickListener();
			m_joystick->setEventCallback( this->m_joystickListener );

		} else {

			Gnoll::Log::LogModule::getInstancePtr()->logMessage( "JOYSTICK: no joystick found" );
		}
	}
}

void OISInputManager::capture( void ) {
	// Need to capture / update each device every frame

	if( m_keyboard ) {
		m_keyboard->capture();
	}

	if( m_mouse ) {
		m_mouse->capture();
	}

	if( m_joystick ) {
		m_joystick->capture();
	}
}

OIS::Keyboard* OISInputManager::getKeyboard( void ) {
	return m_keyboard;
}

bool OISInputManager::keyPressed( const OIS::KeyEvent &e )
{

	Messages::MessageType keydown("KEYBOARD_KEYDOWN");

	shared_ptr<boost::any> kc (new boost::any(OIS::KeyCode(e.key)) ) ;
	shared_ptr<Message>  mymessage (new Message(keydown, kc ));

	MessageModule::getInstancePtr()->getMessageManager()->queueMessage(mymessage);

	return true;
}

bool OISInputManager::keyReleased( const OIS::KeyEvent &e )
{
	Messages::MessageType keyup("KEYBOARD_KEYUP");

	shared_ptr<boost::any> kc (new boost::any(OIS::KeyCode(e.key)) ) ;
	shared_ptr<Message>  mymessage (new Message(keyup, kc ));

	MessageModule::getInstancePtr()->getMessageManager()->queueMessage(mymessage);

	return true;
}

bool OISInputManager::mouseMoved( const OIS::MouseEvent &arg )
{
	Messages::MessageType mouseMoved("MOUSE_MOVED");

	MouseEvent mouseE (arg.state.X.abs, arg.state.Y.abs, arg.state.Z.abs,
				arg.state.X.rel, arg.state.Y.rel, arg.state.Z.rel,
				arg.state.X.absOnly );

	shared_ptr<boost::any> data ( new boost::any(mouseE) )  ;
	shared_ptr<Message>  mymessage (new Message(mouseMoved, data ));

	CEGUI::System::getSingleton().injectMousePosition( arg.state.X.abs, arg.state.Y.abs );

	MessageModule::getInstancePtr()->getMessageManager()->queueMessage(mymessage);

	return true;
}

bool OISInputManager::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	Messages::MessageType mousePressed("MOUSE_PRESSED");

	OIS::MouseState state = arg.state;
	const unsigned int curState = static_cast<unsigned int> (state.buttons);

	for (unsigned int i = 0; i < MAX_MOUSE_BUTTONS; i++)
	{
		unsigned int buttonCurState = curState >> i;
		buttonCurState = buttonCurState & 1;

		unsigned int buttonOldState = this->m_mouseButtonsState >> i;
		buttonOldState = buttonOldState & 1;

		if ( (buttonCurState == 1) && (buttonOldState == 0) )
		{
			shared_ptr<boost::any> button (new boost::any(MouseButton(static_cast<MouseButton>(i)))  )  ;
			shared_ptr<Message>  mymessage (new Message(mousePressed, button ));

			CEGUI::System::getSingleton().injectMouseButtonDown( CEGUI::MouseButton(i) );

			MessageModule::getInstancePtr()->getMessageManager()->queueMessage(mymessage);
		}
	}

	this->m_mouseButtonsState = curState;

	return true;
}

bool OISInputManager::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	Messages::MessageType mouseReleased("MOUSE_RELEASED");

	OIS::MouseState state = arg.state;
	const unsigned int curState = static_cast<unsigned int> (state.buttons);

	for (unsigned int i = 0; i < MAX_MOUSE_BUTTONS; i++)
	{
		unsigned int buttonCurState = curState >> i;
		buttonCurState = buttonCurState & 1;

		unsigned int buttonOldState = this->m_mouseButtonsState >> i;
		buttonOldState = buttonOldState & 1;

		if ( (buttonCurState == 0) && (buttonOldState == 1) )
		{
			shared_ptr<boost::any> button (new boost::any(MouseButton(static_cast<MouseButton>(i)) ) )  ;
			shared_ptr<Message>  mymessage (new Message(mouseReleased, button ));

			CEGUI::System::getSingleton().injectMouseButtonUp( CEGUI::MouseButton(i) );

			MessageModule::getInstancePtr()->getMessageManager()->queueMessage(mymessage);
		}
	}

	this->m_mouseButtonsState = curState;
	return true;
}
