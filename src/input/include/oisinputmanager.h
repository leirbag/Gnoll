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

#ifndef __OISINPUTMANAGER_H__
#define __OISINPUTMANAGER_H__

#include <OISMouse.h>
#include <OISKeyboard.h>
#include <OISJoyStick.h>
#include <OISInputManager.h>

#include <OgreRenderWindow.h>

#include <boost/any.hpp>

#include "../../graphic/include/graphicmodule.h"

using boost::any_cast;

class OISInputManager : public OIS::KeyListener, public OIS::MouseListener
{
	public:
		OISInputManager();
		virtual ~OISInputManager();

		void initialise();
		void capture();

		void addKeyListener(OIS::KeyListener* keyListener, const std::string& instanceName);
		void removeKeyListener(const std::string& instanceName);
		void removeKeyListener(OIS::KeyListener* keyListener);

		void removeAllListeners();
		void removeAllKeyListeners();

		OIS::Keyboard* getKeyboard();

	private:
		bool keyPressed(const OIS::KeyEvent& e);
		bool keyReleased(const OIS::KeyEvent& e);

		bool mouseMoved(const OIS::MouseEvent& arg);
		bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
		bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);

		OIS::InputManager* m_inputSystem;
		OIS::Keyboard* m_keyboard;
		OIS::Mouse* m_mouse;
		OIS::JoyStick* m_joystick;
		OIS::JoyStickListener* m_joystickListener;

		unsigned int m_mouseButtonsState;
};

#endif // __COISINPUTMANAGER_H__
