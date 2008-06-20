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
|               04/10/2008 - Gabriel - Add persistance of near, far and fov |
|									   value                                |
|               04/10/2008 - Gabriel - Add the enqueue of listener          |
\*-------------------------------------------------------------------------*/

#include "../include/camera.h"
#include "../include/cmessagelistenercamera.h"
#include "../../graphic/include/cgraphicmodule.h"
#include "../../core/include/cmessagemodule.h"
#include "../../core/include/cmessagetype.h"
#include "../../dynamicobject/include/float.h"
#include "../../log/include/clogmodule.h"
#include <sstream>
#include <queue>

#include "../../config.h"

using namespace Gnoll::Graphic;
using namespace Gnoll::DynamicObject;
using namespace Gnoll::Core;

namespace Gnoll
{
	namespace Scene
	{
		struct camera_i
		{
			typedef std::pair<shared_ptr<CMessageListenerCamera>, shared_ptr<CMessageType> > PairsListener;
			typedef std::queue<shared_ptr<PairsListener> > QueueListener;
			QueueListener queueListener;
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
			CDynamicObjectProxy(instanceName),
			m_this(new camera_i)
		{

			Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "Loading camera [" + instanceName + "]" );

			m_this->name = instanceName;
			m_this->pOgreCamera = CGraphicModule::getInstancePtr()->getSceneManager()->createCamera(instanceName);

			/**
			 * Extract Camera's near, far and fov value
			 */
			shared_ptr<Float> near_value;
			shared_ptr<Float> far_value;
			shared_ptr<Float> fov_value;
			shared_ptr<Float> default_near = shared_ptr<Float> (new Float(1.0f));
			shared_ptr<Float> default_far = shared_ptr<Float> (new Float(200.0f));
			shared_ptr<Float> default_fov = shared_ptr<Float> (new Float(3.14/4.0f)); // TODO : change this to the real value

			near_value = this->getAttributeOrDefault<Float>("near", default_near);
			far_value = this->getAttributeOrDefault<Float>("far", default_far);
			fov_value = this->getAttributeOrDefault<Float>("fov", default_fov);

			setNearValue(*near_value);
			setFarValue(*far_value);
			setFovValue(*fov_value);

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
			CDynamicObjectProxy(copy)
		{
			// Copy attributs
			m_this = new camera_i;
			*m_this = *(copy.m_this);
		}

		Camera::~Camera()
		{
			// Destroy all listeners attach to this camera
			CMessageModule* messageModule = CMessageModule::getInstancePtr();

			while (!m_this->queueListener.empty())
			{
				shared_ptr<camera_i::PairsListener> pairs = shared_ptr<camera_i::PairsListener>(m_this->queueListener.front());
				messageModule->getMessageManager()->delListener(shared_ptr<CMessageListenerCamera>(pairs->first), *pairs->second);
				m_this->queueListener.pop();
			}

			/*
			 * Save the current configuration of the camera
			 */
			// near
			shared_ptr<Float> near_value(new Float(getNearValue()));
			this->setAttribute("near", near_value);

			// far
			shared_ptr<Float> far_value(new Float(getFarValue()));
			this->setAttribute("far", far_value);

			// fov
			shared_ptr<Float> fov_value(new Float(getFovValue()));
			this->setAttribute("fov", fov_value);

			// Position
			Ogre::Vector3 pos = m_this->pOgreCamera->getPosition();
			shared_ptr<Float> pos_x(new Float(pos.x));
			this->setAttribute("pos_x", pos_x);

			shared_ptr<Float> pos_y(new Float(pos.y));
			this->setAttribute("pos_y", pos_y);

			shared_ptr<Float> pos_z(new Float(pos.z));
			this->setAttribute("pos_z", pos_z);

			// Direction
			Ogre::Vector3 dir = m_this->pOgreCamera->getDirection();
			shared_ptr<Float> dir_x(new Float(dir.x));
			this->setAttribute("dir_x", dir_x);

			shared_ptr<Float> dir_y(new Float(dir.y));
			this->setAttribute("dir_y", dir_y);

			shared_ptr<Float> dir_z(new Float(dir.z));
			this->setAttribute("dir_z", dir_z);

			Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "Saving camera [" + this->getInstance() + "]" );

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

		}

		void Camera::setOrientation(const Ogre::Quaternion& orientation)
		{
			m_this->pOgreCamera->setOrientation(orientation);
		}

		void Camera::update(float time)
		{
			// Do nothing
		}

		void Camera::enqueueListener(shared_ptr<CMessageListenerCamera> listener, shared_ptr<CMessageType> type)
		{
			// Add the listener to the queue
			m_this->queueListener.push(shared_ptr<camera_i::PairsListener>(new camera_i::PairsListener(listener, type)));
		}
	};
};
