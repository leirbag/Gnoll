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
  |                                      value                                |
  |               04/10/2008 - Gabriel - Add the enqueue of listener          |
  |               03/07/2008 - Gabriel - Remove insulation                    |
  \*-------------------------------------------------------------------------*/

#include "../include/camera.h"
#include "../include/cmessagelistenercamera.h"
#include "../../graphic/include/cgraphicmodule.h"
#include "../../core/include/cmessagemodule.h"
#include "../../core/include/cmessagetype.h"
#include "../../dynamicobject/include/float.h"
#include "../../dynamicobject/include/string.h"
#include "../../log/include/clogmodule.h"
#include "../../dynamicobject/include/vector3.h"
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
		Camera::Camera(const Glib::ustring& instanceName) :
			CDynamicObjectProxy(instanceName)
		{

			Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "Loading camera [" + instanceName + "]" );

			// Name of the camera
			setAttribute("name", shared_ptr<Gnoll::DynamicObject::String>(new Gnoll::DynamicObject::String(instanceName)));

			// Create the camera in the scenemanager
			pOgreCamera = CGraphicModule::getInstancePtr()->getSceneManager()->createCamera(instanceName);

			// By default we have a target, NEED TO FIXE IT !!! only camera who need the target have
			// to initialise this
			pTarget     = CGraphicModule::getInstancePtr()->getSceneManager()->createSceneNode();

			// Extract Camera's near, far and fov value
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

			(*Gnoll::Log::CLogModule::getInstancePtr()) << "Near: " << *near_value << "\t";
			(*Gnoll::Log::CLogModule::getInstancePtr()) << "Far: " << *far_value << "\t";
			(*Gnoll::Log::CLogModule::getInstancePtr()) << "Fov: " << *fov_value << "\t";

			// Extract Camera's position
			shared_ptr<Gnoll::DynamicObject::Vector3> default_pos = shared_ptr<Gnoll::DynamicObject::Vector3> (new Gnoll::DynamicObject::Vector3());
			shared_ptr<Gnoll::DynamicObject::Vector3> temp_pos = this->getAttributeOrDefault<Gnoll::DynamicObject::Vector3>("Position", default_pos);

			// We need to convert the DO Vector3 to Ogre::Vector3 (inheritance)
			pOgreCamera->setPosition(*dynamic_pointer_cast<Ogre::Vector3>(temp_pos));

			(*Gnoll::Log::CLogModule::getInstancePtr()) << "Position: " << pOgreCamera->getPosition() << "\t";

			// Extract Camera's direction
			shared_ptr<Gnoll::DynamicObject::Vector3> default_dir = shared_ptr<Gnoll::DynamicObject::Vector3> (
					new Gnoll::DynamicObject::Vector3(Ogre::Vector3(0, 0, -1)));
			shared_ptr<Gnoll::DynamicObject::Vector3> temp_dir = this->getAttributeOrDefault<Gnoll::DynamicObject::Vector3>("Direction", default_dir);

			// We need to convert the DO Vector3 to Ogre::Vector3 (inheritance)
			pOgreCamera->setDirection(*dynamic_pointer_cast<Ogre::Vector3>(temp_dir));

			(*Gnoll::Log::CLogModule::getInstancePtr()) << "Direction: " << pOgreCamera->getDirection() << "\t";

			// Extract Camera's movement configuration
			shared_ptr<Float> default_value = shared_ptr<Float> (new Float(1.0f));
			mapMovement["ROTATION_AXIS_X"] = *(this->getAttributeOrDefault<Float>("ROTATION_AXIS_X", default_value));
			mapMovement["ROTATION_AXIS_Y"] = *(this->getAttributeOrDefault<Float>("ROTATION_AXIS_Y", default_value));
			mapMovement["ROTATION_AXIS_Z"] = *(this->getAttributeOrDefault<Float>("ROTATION_AXIS_Z", default_value));

			mapMovement["STRAFE_UP"]     = *(this->getAttributeOrDefault<Float>("STRAFE_UP", default_value));
			mapMovement["STRAFE_DOWN"]    = *(this->getAttributeOrDefault<Float>("STRAFE_DOWN", default_value));
			mapMovement["STRAFE_LEFT"]     = *(this->getAttributeOrDefault<Float>("STRAFE_LEFT", default_value));
			mapMovement["STRAFE_RIGHT"]    = *(this->getAttributeOrDefault<Float>("STRAFE_RIGHT", default_value));
			mapMovement["MOVE_FORWARD"]    = *(this->getAttributeOrDefault<Float>("MOVE_FORWARD", default_value));
			mapMovement["MOVE_BACK"]       = *(this->getAttributeOrDefault<Float>("MOVE_BACK", default_value));
		}

		Camera::Camera(const Camera& copy) :
			CDynamicObjectProxy(copy)
		{
		}

		Camera& Camera::operator=(const Camera& copy)
		{
			return *this;
		}

		Camera::~Camera()
		{
			// Destroy all listeners attach to this camera
			CMessageModule* messageModule = CMessageModule::getInstancePtr();

			while (!queueListener.empty())
			{
				shared_ptr<PairsListener> pairs = shared_ptr<PairsListener>(queueListener.front());
				messageModule->getMessageManager()->delListener(shared_ptr<CMessageListenerCamera>(pairs->first), *pairs->second);
				queueListener.pop();
			}

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
			setAttribute("Position", shared_ptr<Gnoll::DynamicObject::Vector3>(new Gnoll::DynamicObject::Vector3(pOgreCamera->getPosition())));

			// Direction
			setAttribute("Direction", shared_ptr<Gnoll::DynamicObject::Vector3>(new Gnoll::DynamicObject::Vector3(pOgreCamera->getDirection())));

			// Camera's movement configuration
			std::map<std::string, float>::iterator it = mapMovement.begin();
			while(it != mapMovement.end())
			{
				shared_ptr<Float> configValue(new Float(it->second));
				this->setAttribute(it->first, configValue);
				it++;
			}

			Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "Saving camera [" + this->getInstance() + "]" );
		}

		float Camera::getNearValue() const
		{
			return pOgreCamera->getNearClipDistance();
		}

		float Camera::getFarValue() const
		{
			return pOgreCamera->getFarClipDistance();
		}

		float Camera::getFovValue() const
		{
			return pOgreCamera->getAspectRatio();
		}

		const Ogre::Vector3& Camera::getPosition() const
		{
			return pOgreCamera->getPosition();
		}

		Ogre::Vector3 Camera::getDirection() const
		{
			return pOgreCamera->getDirection();
		}

		Ogre::Quaternion Camera::getOrientation() const
		{
			return pOgreCamera->getOrientation();
		}

		Ogre::SceneNode* Camera::getTarget() const
		{
			return pTarget;
		}

		Ogre::Camera* Camera::getOgreCamera()
		{
			return pOgreCamera;
		}

		Ogre::Vector3 Camera::getUp() const
		{
			return pOgreCamera->getUp();
		}

		Ogre::Matrix4 Camera::getView() const
		{
			return pOgreCamera->getViewMatrix();
		}

		void Camera::setNearValue(float near)
		{
			if(fabs(near) > getFarValue())
				return;

			pOgreCamera->setNearClipDistance(near);
		}

		void Camera::setFarValue(float far)
		{
			pOgreCamera->setFarClipDistance(far);
		}

		void Camera::setFovValue(float fov)
		{
			pOgreCamera->setAspectRatio(fov);
		}

		void Camera::setPosition(const Ogre::Vector3& position)
		{
			pOgreCamera->setPosition(position);
		}

		void Camera::setTargetHelper(Ogre::SceneNode* target)
		{
			pTarget = target;
		}

		void Camera::setTarget(Ogre::SceneNode* target, bool autofocus)
		{
			if(target == NULL)
				return;

			pOgreCamera->setAutoTracking(autofocus, target);
			setTargetHelper(target);
			update(1.0f);
		}

		const std::map<std::string, float>& Camera::getMovementInformation()
		{
			return mapMovement;
		}

		void Camera::setMovementInformation(const std::map<std::string, float>& mapConfiguration)
		{
			mapMovement = mapConfiguration;
		}

		void Camera::setOrientation(const Ogre::Quaternion& orientation)
		{
			pOgreCamera->setOrientation(orientation);
		}

		void Camera::update(float time)
		{
			// Do nothing
		}

		void Camera::enqueueListener(shared_ptr<CMessageListenerCamera> listener, shared_ptr<CMessageType> type)
		{
			// Add the listener to the queue
			queueListener.push(shared_ptr<PairsListener>(new PairsListener(listener, type)));
		}
	};
};
