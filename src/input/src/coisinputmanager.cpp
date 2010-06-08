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

#include "../include/cinputmouseevents.h"
#include "../include/cinputjoystickevents.h"
#include "../include/coisinputmanager.h"
#include "../../core/include/messagemodule.h"
#include "../../core/include/message.h"
#include "../../log/include/clogmacros.h"
#include <boost/shared_ptr.hpp>
#include <string>

#include "../../config.h"
#include "../include/oisjoysticklistener.h"

using namespace Gnoll::Input;
using namespace Gnoll::Graphic;
using namespace Gnoll::Core;

COISInputManager::COISInputManager( ) :
    mInputSystem( 0 ) , mKeyboard( 0 ), mMouse( 0 ), mJoystick( 0 ), mMouseButtonsState( 0 )
{
}

COISInputManager::~COISInputManager( void )
{
	if( mInputSystem )
	{

		if( mKeyboard )
		{
			mInputSystem->destroyInputObject( mKeyboard );
			mKeyboard = 0;
		}

		if( mMouse )
		{
			mInputSystem->destroyInputObject( mMouse );
			mMouse = 0;
		}

		if( mJoystick )
		{
			mInputSystem->destroyInputObject( mJoystick );
			mJoystick = 0;
			delete this->joystickListener;
		}

		Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "Destroying OIS" );
		OIS::InputManager::destroyInputSystem( mInputSystem );
		mInputSystem = 0;
	}
}

void COISInputManager::initialise(  )
{
	if( !mInputSystem )
	{

		// Setup basic variables
		OIS::ParamList paramList;
		size_t windowHnd = 0;
		std::ostringstream windowHndStr;


		windowHnd = CGraphicModule::getInstancePtr()->getWindowHandle();

		// Fill parameter list
		windowHndStr << (unsigned int) windowHnd;
		paramList.insert( std::make_pair( std::string( "WINDOW" ), windowHndStr.str() ) );

		// Create inputsystem
		mInputSystem = OIS::InputManager::createInputSystem( paramList );


		/**
		 * Get number of available keyboard
		 *
		 * OIS::InputManager::numKeyBoards has been renamed to
		 * OIS::InputManager::numKeyboards from OIS 1.0
		 * But the old one is still used in OIS 1.0rc which is shiped
		 * with some GNU/Linux distribution
		 */
#ifdef HAVE_OIS_1RC
		int numKeyboards = mInputSystem->numKeyBoards();
#elif defined HAVE_OIS_1_2
		int numKeyboards = mInputSystem->getNumberOfDevices( OIS::OISKeyboard );
#else
		int numKeyboards = mInputSystem->numKeyboards();
#endif

		// If possible create a buffered keyboard
		if( numKeyboards > 0 )
		{
			mKeyboard = static_cast<OIS::Keyboard*>( mInputSystem->createInputObject( OIS::OISKeyboard, true ) );
			mKeyboard->setEventCallback( this );
		}


		// If possible create a buffered mouse
#ifdef HAVE_OIS_1_2
		if( mInputSystem->getNumberOfDevices( OIS::OISMouse ) > 0 )
		{
#else
		if( mInputSystem->numMice() > 0 )
		{
#endif
			mMouse = static_cast<OIS::Mouse*>( mInputSystem->createInputObject( OIS::OISMouse, true ) );
			mMouse->setEventCallback( this );

			Ogre::RenderWindow* renderWindow = CGraphicModule::getInstancePtr()->getRenderWindow();

			if (renderWindow)
			{
				const OIS::MouseState& mstate = mMouse->getMouseState();
				mstate.width = renderWindow->getWidth();
				mstate.height = renderWindow->getHeight();
			}
		}



		// If possible create a buffered joystick
#ifdef HAVE_OIS_1_2
		GNOLL_LOG() << "JOYSTICK: " << mInputSystem->getNumberOfDevices( OIS::OISJoyStick ) << " detected\n";
		if( mInputSystem->getNumberOfDevices( OIS::OISJoyStick ) > 0 )
		{
#else
		GNOLL_LOG() << "JOYSTICK: " << mInputSystem->numJoySticks() << " detected\n";
		if( mInputSystem->numJoySticks() > 0 )
		{
#endif
			mJoystick = static_cast<OIS::JoyStick*>( mInputSystem->createInputObject( OIS::OISJoyStick, true ) );

			GNOLL_LOG() << "JOYSTICK: Joystick found\n";

#ifdef HAVE_OIS_1_2
			GNOLL_LOG() << "JOYSTICK: " << mJoystick->getNumberOfComponents(OIS::OIS_Button)  << " buttons detected\n";
			GNOLL_LOG() << "JOYSTICK: " << mJoystick->getNumberOfComponents(OIS::OIS_Axis) << " axes detected\n";
			GNOLL_LOG() << "JOYSTICK: " << mJoystick->getNumberOfComponents(OIS::OIS_Slider) << " sliders detected\n";
			GNOLL_LOG() << "JOYSTICK: " << mJoystick->getNumberOfComponents(OIS::OIS_POV) << " pov detected\n";
#else
			GNOLL_LOG() << "JOYSTICK: " << mJoystick->axes() << " axes detected\n";
			GNOLL_LOG() << "JOYSTICK: " << mJoystick->buttons() << " buttons detected\n";
			GNOLL_LOG() << "JOYSTICK: " << mJoystick->hats() << " hats detected\n";
#endif

			this->joystickListener = new Gnoll::Input::OISJoystickListener();
			mJoystick->setEventCallback( this->joystickListener );

		} else {

			Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "JOYSTICK: no joystick found" );
		}

	}
}

void COISInputManager::capture( void ) {
	// Need to capture / update each device every frame

	if( mKeyboard ) {
		mKeyboard->capture();
	}

	if( mMouse ) {
		mMouse->capture();
	}

	if( mJoystick ) {
		mJoystick->capture();
	}

}



OIS::Keyboard* COISInputManager::getKeyboard( void ) {
	return mKeyboard;
}

bool COISInputManager::keyPressed( const OIS::KeyEvent &e )
{

	Messages::MessageType keydown("KEYBOARD_KEYDOWN");

	shared_ptr<boost::any> kc (new boost::any(OIS::KeyCode(e.key)) ) ;
	shared_ptr<Message>  mymessage (new Message(keydown, kc ));

	MessageModule::getInstancePtr()->getMessageManager()->queueMessage(mymessage);

	return true;
}

bool COISInputManager::keyReleased( const OIS::KeyEvent &e )
{
	Messages::MessageType keyup("KEYBOARD_KEYUP");

	shared_ptr<boost::any> kc (new boost::any(OIS::KeyCode(e.key)) ) ;
	shared_ptr<Message>  mymessage (new Message(keyup, kc ));

	MessageModule::getInstancePtr()->getMessageManager()->queueMessage(mymessage);

	return true;
}


bool COISInputManager::mouseMoved( const OIS::MouseEvent &arg )
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


bool COISInputManager::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	Messages::MessageType mousePressed("MOUSE_PRESSED");

	OIS::MouseState state = arg.state;
	const unsigned int curState = static_cast<unsigned int> (state.buttons);

	for (unsigned int i = 0; i < MAX_MOUSE_BUTTONS; i++)
	{
		unsigned int buttonCurState = curState >> i;
		buttonCurState = buttonCurState & 1;

		unsigned int buttonOldState = this->mMouseButtonsState >> i;
		buttonOldState = buttonOldState & 1;


		if ( (buttonCurState == 1) && (buttonOldState == 0) )
		{
			shared_ptr<boost::any> button (new boost::any(MouseButton(static_cast<MouseButton>(i)))  )  ;
			shared_ptr<Message>  mymessage (new Message(mousePressed, button ));

			CEGUI::System::getSingleton().injectMouseButtonDown( CEGUI::MouseButton(i) );

			MessageModule::getInstancePtr()->getMessageManager()->queueMessage(mymessage);
		}

	}


	this->mMouseButtonsState = curState;

	return true;
}


bool COISInputManager::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	Messages::MessageType mouseReleased("MOUSE_RELEASED");

	OIS::MouseState state = arg.state;
	const unsigned int curState = static_cast<unsigned int> (state.buttons);

	for (unsigned int i = 0; i < MAX_MOUSE_BUTTONS; i++)
	{
		unsigned int buttonCurState = curState >> i;
		buttonCurState = buttonCurState & 1;

		unsigned int buttonOldState = this->mMouseButtonsState >> i;
		buttonOldState = buttonOldState & 1;


		if ( (buttonCurState == 0) && (buttonOldState == 1) )
		{
			shared_ptr<boost::any> button (new boost::any(MouseButton(static_cast<MouseButton>(i)) ) )  ;
			shared_ptr<Message>  mymessage (new Message(mouseReleased, button ));

			CEGUI::System::getSingleton().injectMouseButtonUp( CEGUI::MouseButton(i) );

			MessageModule::getInstancePtr()->getMessageManager()->queueMessage(mymessage);
		}

	}

	this->mMouseButtonsState = curState;

	return true;
}

