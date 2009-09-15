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
 * This is an implement of camera wrapper for Ogre                         *
 ***************************************************************************/

#include "../include/ogrecamerawrapper.h"
#include "../../graphic/include/cgraphicmodule.h"
#include <OgreRoot.h>

using namespace Gnoll::Graphic;
using namespace Gnoll::Core;

namespace Gnoll
{
	namespace Scene
	{
		OgreCameraWrapper::OgreCameraWrapper(const Glib::ustring& name) :
			CameraWrapper(name)
		{
			// Create the camera from the scenemanager
			// ---------------------------------------
			camera = CGraphicModule::getInstancePtr()->getSceneManager()->createCamera(name);

			// Create one viewport, entire window
			// ----------------------------------
			Ogre::Root* ogreRoot       = Ogre::Root::getSingletonPtr();
			Ogre::RenderWindow* window = ogreRoot->getAutoCreatedWindow();
			Ogre::Viewport* vp         = window->addViewport(camera);
			vp->setBackgroundColour(Ogre::ColourValue(0.5,1,0));
		}

		OgreCameraWrapper::~OgreCameraWrapper()
		{
			CGraphicModule::getInstancePtr()->getSceneManager()->destroyCamera(camera->getName());
		}

		void OgreCameraWrapper::setNearValue(float value)
		{
			camera->setNearClipDistance(value);
		}

		float OgreCameraWrapper::getNearValue()
		{
			return camera->getNearClipDistance();
		}

		void OgreCameraWrapper::setFarValue(float value)
		{
			camera->setFarClipDistance(value);
		}

		float OgreCameraWrapper::getFarValue()
		{
			return camera->getFarClipDistance();
		}

		void OgreCameraWrapper::setFovValue(float value)
		{
			camera->setAspectRatio(value);
		}

		float OgreCameraWrapper::getFovValue()
		{
			return camera->getAspectRatio();
		}

		void OgreCameraWrapper::setPosition(const Ogre::Vector3& value)
		{
			camera->setPosition(value);
		}

		Ogre::Vector3 OgreCameraWrapper::getPosition()
		{
			return camera->getPosition();
		}

		void OgreCameraWrapper::setDirection(const Ogre::Vector3& value)
		{
			camera->setDirection(value);
		}

		Ogre::Vector3 OgreCameraWrapper::getDirection()
		{
			return camera->getDirection();
		}

		void OgreCameraWrapper::setOrientation(const Ogre::Quaternion& value)
		{
			camera->setOrientation(value);
		}

		Ogre::Quaternion OgreCameraWrapper::getOrientation()
		{
			return camera->getOrientation();
		}

		Ogre::Vector3 OgreCameraWrapper::getUp()
		{
			return camera->getUp();
		}

		Ogre::Vector3 OgreCameraWrapper::getRight()
		{
			return camera->getRight();
		}

		void OgreCameraWrapper::setAutoTracking(bool autofocus, Ogre::SceneNode* target)
		{
			camera->setAutoTracking(autofocus, target);
		}

		void OgreCameraWrapper::pitch(Ogre::Radian value)
		{
			camera->pitch(value);
		}

		void OgreCameraWrapper::yaw(Ogre::Radian value)
		{
			camera->yaw(value);
		}

		void OgreCameraWrapper::roll(Ogre::Radian value)
		{
			camera->roll(value);
		}
	};
};
