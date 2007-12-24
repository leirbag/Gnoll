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


/*-------------------------------cmessage----------------------------------*\
|   This is a free fly camera                                               |
|                                                                           |
|   Changelog :                                                             |
|               08/31/2007 - Gabriel - Initial release                      |
|               10/06/2007 - Gabriel - Add hack to enable key repeat        |
|                                    - Add management of mouse for the      |
|                                        rotation                           |
|               19/06/2007 - Gabriel - Change all variables for listener    |
|                                       by a map.                           |
|                                      Add time to the transformation       |
|               19/06/2007 - Gabriel - Change all variables for listener    |
|               11/16/2007 - Paf - Remove all references to                 |
|                                   CGenericMessageManager                  |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include "../include/camerafreefly.h"

namespace Gnoll
{

	namespace Core 
	{

		CameraFreeFly::CameraFreeFly(const Glib::ustring& instanceName, Ogre::SceneManager* pSM) : Gnoll::Core::Camera(instanceName, pSM)
		{
			CMessageModule* messageModule = CMessageModule::getInstancePtr(); 

			m_listenerKeyUp = shared_ptr<CMessageListener>(new KeyUp);
			messageModule->getMessageManager()->addListener ( m_listenerKeyUp, CMessageType("KEYBOARD_KEYUP") );

			m_listenerKeyDown = shared_ptr<CMessageListener>(new KeyDown);
			messageModule->getMessageManager()->addListener ( m_listenerKeyDown, CMessageType("KEYBOARD_KEYDOWN") );


			m_listenerMove = shared_ptr<CMessageListener>(new MoveCameraFreeFlyListener(static_cast<Gnoll::Core::CameraFreeFly*>(this)));
			messageModule->getMessageManager()->addListener ( m_listenerMove, CMessageType("GRAPHIC_FRAME_RENDERED") );

			m_listenerRotate = shared_ptr<CMessageListener>(new RotateCameraFreeFlyListener(static_cast<Gnoll::Core::CameraFreeFly*>(this)));
			messageModule->getMessageManager()->addListener ( m_listenerRotate, CMessageType("GRAPHIC_FRAME_RENDERED") );

			m_listenerMouseRotate = shared_ptr<CMessageListener>(new MouseRotateCameraFreeFlyListener(static_cast<Gnoll::Core::CameraFreeFly*>(this)));
			messageModule->getMessageManager()->addListener ( m_listenerMouseRotate, CMessageType("MOUSE_MOVED") );


			m_listenerStrafe = shared_ptr<CMessageListener>(new Gnoll::Core::StrafeCameraFreeFlyListener(static_cast<Gnoll::Core::CameraFreeFly*>(this)));
			messageModule->getMessageManager()->addListener ( m_listenerStrafe, CMessageType("GRAPHIC_FRAME_RENDERED") );

			// Add key listener
			g_mapCffKeys["MoveUp"] = false;
			g_mapCffKeys["MoveDown"] = false;

			g_mapCffKeys["StrafeUp"] = false;
			g_mapCffKeys["StrafeDown"] = false;
			g_mapCffKeys["StrafeLeft"] = false;
			g_mapCffKeys["StrafeRight"] = false;

			g_mapCffKeys["RotateUp"] = false;
			g_mapCffKeys["RotateDown"] = false;
			g_mapCffKeys["RotateLeft"] = false;
			g_mapCffKeys["RotateRight"] = false;
		}

		CameraFreeFly::~CameraFreeFly()
		{
			CMessageModule* messageModule = CMessageModule::getInstancePtr(); 
			messageModule->getMessageManager()->delListener ( m_listenerMove, CMessageType("GRAPHIC_FRAME_RENDERED") );
			messageModule->getMessageManager()->delListener ( m_listenerRotate, CMessageType("GRAPHIC_FRAME_RENDERED") );
			messageModule->getMessageManager()->delListener ( m_listenerStrafe, CMessageType("GRAPHIC_FRAME_RENDERED") );
			messageModule->getMessageManager()->delListener ( m_listenerKeyUp, CMessageType("KEYBOARD_KEYUP") );
			messageModule->getMessageManager()->delListener ( m_listenerKeyDown, CMessageType("KEYBOARD_KEYDOWN") );
			messageModule->getMessageManager()->delListener ( m_listenerMouseRotate, CMessageType("MOUSE_MOVED") );
		}

		void CameraFreeFly::move(const Ogre::Vector3& dir)
		{
			m_ogreCamera->moveRelative(dir);
		}

		void CameraFreeFly::rotateAxisX(float angle)
		{
			m_ogreCamera->rotate(m_ogreCamera->getRight(), Ogre::Radian(angle));
		}

		void CameraFreeFly::rotateAxisY(float angle)
		{
			m_ogreCamera->rotate(m_ogreCamera->getUp(), Ogre::Radian(angle));
		}

		void CameraFreeFly::rotateAxisZ(float angle)
		{
			m_ogreCamera->rotate(m_ogreCamera->getRealDirection(), Ogre::Radian(angle));
		}

		void CameraFreeFly::strafe(float distance)
		{
			m_ogreCamera->moveRelative(Ogre::Vector3(distance, 0.0f, 0.0f));
		}

		void CameraFreeFly::strafeUp(float distance)
		{
			m_ogreCamera->moveRelative(Ogre::Vector3(0.0f, distance, 0.0f));
		}

	};
};


