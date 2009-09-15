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
 * This is an implementation of a Free Fly Camera, it provides some        *
 * services:                                                               *
 * 	- Move forward and backward                                           *
 * 	- Strafe on left/right/up/down                                        *
 * 	- Rotate the "at" position on each axes                               *
 ***************************************************************************/

#include "../include/camerafreefly.h"
#include "../include/camerawrapper.h"
#include "../../dynamicobject/include/float.h"
#include "../../log/include/clogmodule.h"
#include <cmath>

using namespace Gnoll::DynamicObject;

namespace Gnoll
{
	namespace Scene
	{
		CameraFreeFly::CameraFreeFly(const Glib::ustring& instanceName, shared_ptr<CameraWrapper> wrapper) :
			Camera(instanceName, wrapper)
		{
			rotationX = rotationY = rotationZ = 0.0f;
		}

		CameraFreeFly::~CameraFreeFly()
		{
		}

		void CameraFreeFly::strafeLR(float speed)
		{
			Ogre::Vector3 distance = cameraWrapper->getPosition() + cameraWrapper->getRight() * speed;
			if(speed < 0)
				distance *= mapMovement["STRAFE_LEFT"];
			else
				distance *= mapMovement["STRAFE_RIGHT"];
			cameraWrapper->setPosition(distance);
		}

		void CameraFreeFly::strafeUD(float speed)
		{
			Ogre::Vector3 distance = cameraWrapper->getPosition() + cameraWrapper->getUp() * speed;
			if(speed < 0)
				distance *= mapMovement["STRAFE_DOWN"];
			else
				distance *= mapMovement["STRAFE_UP"];
			cameraWrapper->setPosition(distance);
		}

		void CameraFreeFly::move(float speed)
		{
			Ogre::Vector3 distance = cameraWrapper->getPosition() + cameraWrapper->getDirection() * speed;
			if(speed < 0)
				distance *= mapMovement["MOVE_BACK"];
			else
				distance *= mapMovement["MOVE_FORWARD"];
			cameraWrapper->setPosition(distance);
		}

		void CameraFreeFly::rotateAroundAxisX(float degree)
		{
			rotationX += degree;
		}

		void CameraFreeFly::rotateAroundAxisY(float degree)
		{
			rotationY += degree;
		}

		void CameraFreeFly::rotateAroundAxisZ(float degree)
		{
			rotationZ += degree;
		}

		void CameraFreeFly::update(float time)
		{
			cameraWrapper->yaw(Ogre::Radian(Ogre::Degree(rotationY * mapMovement["ROTATION_AXIS_Y"])));
			cameraWrapper->pitch(Ogre::Radian(Ogre::Degree(rotationX * mapMovement["ROTATION_AXIS_X"])));
			cameraWrapper->roll(Ogre::Radian(Ogre::Degree(rotationZ * mapMovement["ROTATION_AXIS_Z"])));
			rotationX = rotationY = rotationZ = 0.0f;
			Ogre::Quaternion q = cameraWrapper->getOrientation();
			q.normalise();
			cameraWrapper->setOrientation(q);
		}
	};
};
