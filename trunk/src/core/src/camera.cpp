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


/*---------------------------------integer---------------------------------*\
|   This is the interface of all camera                                     |
|                                                                           |
|   Changelog :                                                             |
|               08/30/2007 - Gabriel - Initial release                      |
|               10/30/2007 - Gabriel - add time to update()                 |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include "../include/camera.h"

namespace Gnoll
{
	namespace Core
	{
		Camera::Camera(const Glib::ustring& instanceName, Ogre::SceneManager* pSM) : PersistentObject(instanceName)
		{
			m_ogreCamera = pSM->createCamera(instanceName);
			m_pSM = pSM;
			m_name = instanceName;
		}

		Ogre::Vector3 Camera::getLookAt()
		{
			return m_ogreCamera->getRealDirection();
		}

		void Camera::setLookAt(const Ogre::Vector3& vLookAt)
		{
			m_ogreCamera->lookAt(vLookAt);
		}

		Ogre::Vector3 Camera::getUp()
		{
			return m_ogreCamera->getRealUp();
		}

		Ogre::Vector3 Camera::getEye()
		{
			return m_ogreCamera->getRealPosition();
		}

		void Camera::setEye(const Ogre::Vector3& vEye)
		{
			m_ogreCamera->setPosition(vEye);
		}

		void Camera::setNearDistance(float distance)
		{
			m_ogreCamera->setNearClipDistance(distance);
		}

		void Camera::setFarDistance(float distance)
		{
			m_ogreCamera->setFarClipDistance(distance);
		}

		void Camera::setFov(float angle)
		{
			m_ogreCamera->setAspectRatio(angle);
		}

		Ogre::Camera& Camera::getOgreCamera()
		{
			return *m_ogreCamera;
		}
	}
}