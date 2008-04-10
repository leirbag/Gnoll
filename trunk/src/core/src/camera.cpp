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


/*-------------------------------Camera------------------------------------*\
|   This is the interface of camera                                         |
|                                                                           |
|   Changelog :                                                             |
|               04/08/2008 - Gabriel - Initial release                      |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include "../include/camera.h"
#include "../../graphic/include/cgraphicmodule.h"
#include "../include/cmessagemodule.h"
#include "../include/float.h"

namespace Gnoll
{
	namespace Core
	{
		struct camera_i
		{
			Glib::ustring name;
			Ogre::Camera* pOgreCamera;
			Ogre::SceneNode* pTarget;

			camera_i() :
				name(""),
				pOgreCamera(NULL),
				pTarget(NULL)
			{
			}
		};

		Camera::Camera(const Glib::ustring& instanceName) :
			CPersistentObjectProxy(instanceName),
			m_this(new camera_i)
		{
			m_this->name = instanceName;
			m_this->pOgreCamera = CGraphicModule::getInstancePtr()->getSceneManager()->createCamera(instanceName);

			setNearValue(1.0f);
			setFarValue(200.0f);
			setFovValue(3.14/4); // TODO : change this to the real value

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

			m_this->pOgreCamera->setPosition(*pos_x, *pos_y, *pos_z);

			/**
			 * Extract Camera's direction
			 */
			shared_ptr<Float> dir_x;
			shared_ptr<Float> dir_y;
			shared_ptr<Float> dir_z;
			shared_ptr<Float> default_dir_x = shared_ptr<Float> (new Float(0.0f));
			shared_ptr<Float> default_dir_y = shared_ptr<Float> (new Float(0.0f));
			shared_ptr<Float> default_dir_z = shared_ptr<Float> (new Float(1.0f));

			dir_x = this->getAttributeOrDefault<Float>("dir_x", default_dir_x);
			dir_y = this->getAttributeOrDefault<Float>("dir_y", default_dir_y);
			dir_z = this->getAttributeOrDefault<Float>("dir_z", default_dir_z);

			m_this->pOgreCamera->setDirection(*dir_x, *dir_y, *dir_z);
		}

		Camera::Camera(const Camera& copy) :
			CPersistentObjectProxy(copy)
		{
			// Copy attributs
			m_this = new camera_i;
			*m_this = *(copy.m_this);
		}

		Camera::~Camera()
		{
			/*
			 * Save the current configuration of the camera
			 */
			Ogre::Vector3 pos = m_this->pOgreCamera->getPosition();
			shared_ptr<Float> pos_x(new Float(pos.x));
			this->setAttribute("pos_x", pos_x);

			shared_ptr<Float> pos_y(new Float(pos.y));
			this->setAttribute("pos_y", pos_y);

			shared_ptr<Float> pos_z(new Float(pos.z));
			this->setAttribute("pos_z", pos_z);

			Ogre::Vector3 dir = m_this->pOgreCamera->getDirection();
			shared_ptr<Float> dir_x(new Float(dir.x));
			this->setAttribute("dir_x", dir_x);

			shared_ptr<Float> dir_y(new Float(dir.y));
			this->setAttribute("dir_y", dir_y);

			shared_ptr<Float> dir_z(new Float(dir.z));
			this->setAttribute("dir_z", dir_z);

			delete m_this;
		}

		Camera& Camera::operator=(const Camera& copy)
		{
			// Copy attributs
			*m_this = *(copy.m_this);
			return *this;
		}

		float Camera::getNearValue() const
		{
			return m_this->pOgreCamera->getNearClipDistance();
		}

		float Camera::getFarValue() const
		{
			return m_this->pOgreCamera->getFarClipDistance();
		}

		float Camera::getFovValue() const
		{
			return m_this->pOgreCamera->getAspectRatio();
		}

		const Ogre::Vector3& Camera::getPosition() const
		{
			return m_this->pOgreCamera->getPosition();
		}

		Ogre::Vector3 Camera::getDirection() const
		{
			return m_this->pOgreCamera->getDirection();
		}

		Ogre::Quaternion Camera::getOrientation() const
		{
			return m_this->pOgreCamera->getOrientation();
		}

		Ogre::SceneNode* Camera::getTarget() const
		{
			return m_this->pTarget;
		}

		Ogre::Camera* Camera::getOgreCamera()
		{
			return m_this->pOgreCamera;
		}

		Ogre::Vector3 Camera::getUp() const
		{
			return m_this->pOgreCamera->getUp();
		}

		Ogre::Matrix4 Camera::getView() const
		{
			return m_this->pOgreCamera->getViewMatrix();
		}

		void Camera::setNearValue(float near)
		{
			if(fabs(near) > getFarValue())
				return;

		    m_this->pOgreCamera->setNearClipDistance(near);
		}

		void Camera::setFarValue(float far)
		{
			if(fabs(far) <= getNearValue())
				return;

			m_this->pOgreCamera->setFarClipDistance(far);
		}

		void Camera::setFovValue(float fov)
		{
			m_this->pOgreCamera->setAspectRatio(fov);
		}

		void Camera::setPosition(const Ogre::Vector3& position)
		{
			m_this->pOgreCamera->setPosition(position);
		}

		void Camera::setTargetHelper(Ogre::SceneNode* target)
		{
			m_this->pTarget = target;
		}

		void Camera::setTarget(Ogre::SceneNode* target)
		{
			if(target != NULL)
			{
				m_this->pOgreCamera->setAutoTracking(true, target);
				setTargetHelper(target);
			}
			else
			{
				m_this->pOgreCamera->setAutoTracking(false);
				setTargetHelper(target);
			}

			update(0);
		}

		void Camera::setOrientation(const Ogre::Quaternion& orientation)
		{
			m_this->pOgreCamera->setOrientation(orientation);
		}

		void Camera::update(float time)
		{
			// Do nothing
		}
	};
};
