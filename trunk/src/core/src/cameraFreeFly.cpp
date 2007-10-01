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
|   This is a free fly camera                                                       |
|                                                                           |
|   Changelog :                                                             |
|               08/31/2007 - Gabriel - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include "../include/camerafreefly.h"

namespace Gnoll
{

	namespace Core 
	{

		CameraFreeFly::CameraFreeFly(const Glib::ustring& instanceName, Ogre::SceneManager* pSM) : Gnoll::Core::Camera(instanceName, pSM)
		{
			m_listenerMove = shared_ptr<CMessageListener>(new Gnoll::Core::MoveCameraFreeFlyListener(static_cast<Gnoll::Core::CameraFreeFly*>(this)));
			CGenericMessageManager::getInstance().addListener ( m_listenerMove, CMessageType("KEYBOARD_KEYDOWN") );
			m_listenerRotate = shared_ptr<CMessageListener>(new Gnoll::Core::RotateCameraFreeFlyListener(static_cast<Gnoll::Core::CameraFreeFly*>(this)));
			CGenericMessageManager::getInstance().addListener ( m_listenerRotate, CMessageType("KEYBOARD_KEYDOWN") );
			m_listenerStrafe = shared_ptr<CMessageListener>(new Gnoll::Core::StrafeCameraFreeFlyListener(static_cast<Gnoll::Core::CameraFreeFly*>(this)));
			CGenericMessageManager::getInstance().addListener ( m_listenerStrafe, CMessageType("KEYBOARD_KEYDOWN") );
		}

		void CameraFreeFly::move(const Ogre::Vector3& dir)
		{
			m_ogreCamera->moveRelative(dir);
		}

		void CameraFreeFly::rotateAxisX(float angle)
		{
			m_ogreCamera->rotate(m_ogreCamera->getRealRight(), Ogre::Radian(angle));
		}

		void CameraFreeFly::rotateAxisY(float angle)
		{
			m_ogreCamera->rotate(m_ogreCamera->getRealUp(), Ogre::Radian(angle));
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