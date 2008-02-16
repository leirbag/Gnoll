/**************************************************************************
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
|               12/24/2007 - Gabriel - pass by GraphicModule to get         |
|                                      the scenemanager and delete          |
|                                      scenemanager from constructor        |
|               02/15/2008 - Bruno Mahe - Load/Save Camera's position       |
|                                      and direction when (un-)initialized  |
|               02/15/2008 - Bruno Mahe - Replace                           |
|                            CPersistentObjectProxy::getAttribute() by      |
|                            CPersistentOBjectPRoxy::getAttributeOrDefault()|
|                                                                           |
\*-------------------------------------------------------------------------*/

#include "../include/camera.h"
#include "../../graphic/include/cgraphicmodule.h"
#include "../include/float.h"
#include <OgreVector3.h>



namespace Gnoll
{
	namespace Core
	{
		Camera::Camera(const Glib::ustring& instanceName) : CPersistentObjectProxy(instanceName)
		{
			m_ogreCamera = CGraphicModule::getInstancePtr()->getSceneManager()->createCamera(instanceName);
			m_name = instanceName;

			/**
			 * Extract Camera's position
			 */
			shared_ptr<Float> pos_x;
			shared_ptr<Float> pos_y;
			shared_ptr<Float> pos_z;

			shared_ptr<Float> default_pos = shared_ptr<Float> (new Float(0.0f));

			pos_x = this->getAttributeOrDefault<Float>("pos_x", default_pos);
			pos_y = this->getAttributeOrDefault<Float>("pos_y", default_pos);
			pos_z = this->getAttributeOrDefault<Float>("pos_z", default_pos);


			m_ogreCamera->setPosition(*pos_x, *pos_y, *pos_z);


			/**
			 * Extract Camera's direction
			 */
			shared_ptr<Float> dir_x;
			shared_ptr<Float> dir_y;
			shared_ptr<Float> dir_z;

			shared_ptr<Float> default_dir = shared_ptr<Float> (new Float(0.0f));

			dir_x = this->getAttributeOrDefault<Float>("dir_x", default_dir);
			dir_y = this->getAttributeOrDefault<Float>("dir_y", default_dir);
			dir_z = this->getAttributeOrDefault<Float>("dir_z", default_dir);

			m_ogreCamera->setDirection(*dir_x, *dir_y, *dir_z);

		}

		Camera::~Camera()
		{

			Ogre::Vector3 pos = m_ogreCamera->getPosition();

			shared_ptr<Float> pos_x(new Float(pos.x));
			this->setAttribute("pos_x", pos_x);

			shared_ptr<Float> pos_y(new Float(pos.y));
			this->setAttribute("pos_y", pos_y);

			shared_ptr<Float> pos_z(new Float(pos.z));
			this->setAttribute("pos_z", pos_z);




			Ogre::Vector3 dir = m_ogreCamera->getDirection();

			shared_ptr<Float> dir_x(new Float(dir.x));
			this->setAttribute("dir_x", dir_x);

			shared_ptr<Float> dir_y(new Float(dir.y));
			this->setAttribute("dir_y", dir_y);

			shared_ptr<Float> dir_z(new Float(dir.z));
			this->setAttribute("dir_z", dir_z);
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


