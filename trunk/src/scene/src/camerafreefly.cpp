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


/*-------------------------------CameraFreeFly-------------------------*\
|   This is a first person camera                                           |
|                                                                           |
|   Changelog :                                                             |
|               12/08/2008 - Gabriel - Initial release                      |
\*-------------------------------------------------------------------------*/

#include "../include/camerafreefly.h"
#include "../../dynamicobject/include/float.h"
#include "../../log/include/clogmodule.h"
#include <cmath>

using namespace Gnoll::DynamicObject;

namespace Gnoll
{
	namespace Scene
	{
		CameraFreeFly::CameraFreeFly(const Glib::ustring& instanceName) :
			Camera(instanceName)
		{
			rotationX = rotationY = rotationZ = 0.0f;
		}

		CameraFreeFly::~CameraFreeFly()
		{
		}

		void CameraFreeFly::strafeLR(float speed)
		{
			getOgreCamera()->setPosition(getOgreCamera()->getPosition() + getOgreCamera()->getRight() * speed);
		}

		void CameraFreeFly::strafeUD(float speed)
		{
			getOgreCamera()->setPosition(getOgreCamera()->getPosition() + getOgreCamera()->getUp() * speed);
		}

		void CameraFreeFly::move(float speed)
		{
			getOgreCamera()->setPosition(getOgreCamera()->getPosition() + getOgreCamera()->getDirection() * speed);
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
			getOgreCamera()->yaw(Ogre::Radian(Ogre::Degree(rotationY)));
			getOgreCamera()->pitch(Ogre::Radian(Ogre::Degree(rotationX)));
			getOgreCamera()->roll(Ogre::Radian(Ogre::Degree(rotationZ)));
			rotationX = rotationY = rotationZ = 0.0f;
			Ogre::Quaternion q = getOgreCamera()->getOrientation();
			q.normalise();
			getOgreCamera()->setOrientation(q);
		}
	};
};
