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

/****************************** Summary ************************************
 * This is an abstract class of camera. We can't directly use this camera  *
 * but it provides some services:                                          *
 * 	- Management of listener                                              *
 * 	- Management of target                                                *
 * 	- Set up of serveral parameters: up, eye, at, far, near, fov,         *
 * 	  movement factor                                                     *
 * This camera can change the orientation and the position too.            *
 ***************************************************************************/

#include "../include/camera.h"
#include "../include/camerawrapper.h"
#include "../include/cmessagelistenercamera.h"
#include "../../graphic/include/cgraphicmodule.h"
#include "../../core/include/cmessagemodule.h"
#include "../../core/messages/include/messagetype.h"
#include "../../dynamicobject/include/float.h"
#include "../../dynamicobject/include/string.h"
#include "../../log/include/clogmodule.h"
#include "../../dynamicobject/include/vector3.h"
#include <sstream>
#include <queue>
#include <OgreRoot.h>

#include "../../config.h"

using namespace Gnoll::Graphic;
using namespace Gnoll::DynamicObject;
using namespace Gnoll::Core;

namespace Gnoll
{
	namespace Scene
	{
		Camera::Camera(const Glib::ustring& instanceName, shared_ptr<CameraWrapper> wrapper) :
			DynamicObjectProxy(instanceName)
		{

			Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "Loading camera [" + instanceName + "]" );

			// Set in the DO the name of the camera
			// ------------------------------------
			setAttribute(ATTRIBUTE_NAME(), shared_ptr<Gnoll::DynamicObject::String>(new Gnoll::DynamicObject::String(instanceName)));

			// Create the camera in the scenemanager
			// -------------------------------------
			cameraWrapper = shared_ptr<CameraWrapper>(wrapper);

			// Extract Camera's near, far and fov value
			// ----------------------------------------
			shared_ptr<Float> near_value;
			shared_ptr<Float> far_value;
			shared_ptr<Float> fov_value;
			shared_ptr<Float> default_near = shared_ptr<Float> (new Float(1.0f));
			shared_ptr<Float> default_far = shared_ptr<Float> (new Float(1000.0f));
			shared_ptr<Float> default_fov = shared_ptr<Float> (new Float(3.14/4.0f)); // TODO : change this to the real value

			near_value = this->getAttributeOrDefault<Float>(ATTRIBUTE_NEAR(), default_near);
			far_value = this->getAttributeOrDefault<Float>(ATTRIBUTE_FAR(), default_far);
			fov_value = this->getAttributeOrDefault<Float>(ATTRIBUTE_FOV(), default_fov);

			cameraWrapper->setNearValue(*near_value);
			cameraWrapper->setFarValue(*far_value);
			cameraWrapper->setFovValue(*fov_value);

			// Infinite far plane?
			// -------------------
			Ogre::Root* ogreRoot = Ogre::Root::getSingletonPtr();
			if(ogreRoot == NULL)
			{
				cameraWrapper->setFarValue(500);
			}
			else if (ogreRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
			{
				cameraWrapper->setFarValue(0);
			}

			(*Gnoll::Log::CLogModule::getInstancePtr()) << "Near: " << cameraWrapper->getNearValue() << ";\t";
			(*Gnoll::Log::CLogModule::getInstancePtr()) << "Far: " << cameraWrapper->getFarValue() << ";\t";
			(*Gnoll::Log::CLogModule::getInstancePtr()) << "Fov: " << cameraWrapper->getFovValue() << "\n";

			// Extract Camera's position
			// -------------------------
			shared_ptr<Gnoll::DynamicObject::Vector3> default_pos = shared_ptr<Gnoll::DynamicObject::Vector3> (new Gnoll::DynamicObject::Vector3());
			shared_ptr<Gnoll::DynamicObject::Vector3> temp_pos = this->getAttributeOrDefault<Gnoll::DynamicObject::Vector3>(ATTRIBUTE_POSITION(), default_pos);

			// We need to convert the DO Vector3 to Ogre::Vector3 (inheritance)
			// ----------------------------------------------------------------
			cameraWrapper->setPosition(*dynamic_pointer_cast<Ogre::Vector3>(temp_pos));

			(*Gnoll::Log::CLogModule::getInstancePtr()) << "Position: " << cameraWrapper->getPosition() << "\n";

			// Extract Camera's direction
			// --------------------------
			shared_ptr<Gnoll::DynamicObject::Vector3> default_dir = shared_ptr<Gnoll::DynamicObject::Vector3> (
					new Gnoll::DynamicObject::Vector3(Ogre::Vector3(0, 0, -1)));
			shared_ptr<Gnoll::DynamicObject::Vector3> temp_dir = this->getAttributeOrDefault<Gnoll::DynamicObject::Vector3>(ATTRIBUTE_DIRECTION(), default_dir);

			// We need to convert the DO Vector3 to Ogre::Vector3 (inheritance)
			// ----------------------------------------------------------------
			cameraWrapper->setDirection(*dynamic_pointer_cast<Ogre::Vector3>(temp_dir));

			(*Gnoll::Log::CLogModule::getInstancePtr()) << "Direction: " << cameraWrapper->getDirection() << "\n";

			// Extract Camera's movement configuration
			// ---------------------------------------
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

			// Extract Camera's target
			// -----------------------
			shared_ptr<Gnoll::DynamicObject::String> default_target = shared_ptr<Gnoll::DynamicObject::String> (
					new Gnoll::DynamicObject::String(""));
			shared_ptr<Gnoll::DynamicObject::String> tempTarget = this->getAttributeOrDefault<Gnoll::DynamicObject::String>(ATTRIBUTE_TARGET(), default_target);
			target = std::string(*tempTarget);
		}

		Camera::Camera(const Camera& copy) :
			DynamicObjectProxy(copy)
		{
		}

		Camera& Camera::operator=(const Camera& copy)
		{
			return *this;
		}

		Camera::~Camera()
		{
			// Destroy all listeners attach to this camera
			// -------------------------------------------
			CMessageModule* messageModule = CMessageModule::getInstancePtr();

			while (!queueListener.empty())
			{
				shared_ptr<PairsListener> pairs = shared_ptr<PairsListener>(queueListener.front());
				messageModule->getMessageManager()->delListener(shared_ptr<CMessageListenerCamera>(pairs->first), *pairs->second);
				queueListener.pop();
			}

			// near
			// ----
			shared_ptr<Float> near_value(new Float(cameraWrapper->getNearValue()));
			this->setAttribute(ATTRIBUTE_NEAR(), near_value);

			// far
			// ---
			shared_ptr<Float> far_value(new Float(cameraWrapper->getFarValue()));
			this->setAttribute(ATTRIBUTE_FAR(), far_value);

			// fov
			// ---
			shared_ptr<Float> fov_value(new Float(cameraWrapper->getFovValue()));
			this->setAttribute(ATTRIBUTE_FOV(), fov_value);

			// Position
			// --------
			setAttribute(ATTRIBUTE_POSITION(), shared_ptr<Gnoll::DynamicObject::Vector3>(new Gnoll::DynamicObject::Vector3(cameraWrapper->getPosition())));

			// Direction
			// ---------
			setAttribute(ATTRIBUTE_DIRECTION(), shared_ptr<Gnoll::DynamicObject::Vector3>(new Gnoll::DynamicObject::Vector3(cameraWrapper->getDirection())));

			// Target
			// ------
			setAttribute(ATTRIBUTE_TARGET(), shared_ptr<Gnoll::DynamicObject::String>(new Gnoll::DynamicObject::String(getTargetName())));

			// Camera's movement configuration
			// -------------------------------
			std::map<std::string, float>::iterator it = mapMovement.begin();
			while(it != mapMovement.end())
			{
				shared_ptr<Float> configValue(new Float(it->second));
				this->setAttribute(it->first, configValue);
				it++;
			}

			Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "Saving camera [" + this->getInstance() + "]" );
		}

		const Glib::ustring& Camera::getTargetName() const
		{
			return target;
		}

		Ogre::SceneNode* Camera::getTarget() const
		{
			if(getTargetName() == "")
				return NULL;

			if(CGraphicModule::getInstancePtr()->getSceneManager() == NULL)
				return NULL;

			return CGraphicModule::getInstancePtr()->getSceneManager()->getSceneNode(target);
		}

		CameraWrapper* Camera::getCameraWrapper()
		{
			return cameraWrapper.get();
		}

		void Camera::setTargetHelper(const Glib::ustring& target)
		{
			this->target = target;
		}

		void Camera::setTarget(const Glib::ustring& target, bool autofocus)
		{
			if(target == "")
				return;

			// Set the auto tracking on the target
			// -----------------------------------
			setTargetHelper(target);
			Ogre::SceneNode* sceneTarget = getTarget();
			cameraWrapper->setAutoTracking(autofocus, sceneTarget);

			// Update the camera
			// -----------------
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

		void Camera::update(float time)
		{
		}

		void Camera::enqueueListener(shared_ptr<CMessageListenerCamera> listener, shared_ptr<Messages::MessageType> type)
		{
			// Add the listener to the queue
			// -----------------------------
			queueListener.push(shared_ptr<PairsListener>(new PairsListener(listener, type)));
		}
	};
};
