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


/*-------------------------------CameraThirdPerson-------------------------*\
|   This is a first person camera                                           |
|                                                                           |
|   Changelog :                                                             |
|               04/08/2008 - Gabriel - Initial release                      |
|               04/10/2008 - Gabriel - Add persistance of attributs         |
|               06/28/2008 - Gabriel - Fix critical bug on serialisation of |
|                                       offset                              |
\*-------------------------------------------------------------------------*/

#include "../include/camerathirdperson.h"
#include "../../dynamicobject/include/float.h"
#include <cmath>

using namespace Gnoll::DynamicObject;

namespace Gnoll
{
	namespace Scene
	{
		CameraThirdPerson::CameraThirdPerson(const Glib::ustring& instanceName) :
			Camera(instanceName)
		{
			// Extract Camera's offset settings
			// --------------------------------
			shared_ptr<Float> off;
			shared_ptr<Float> minOff;
			shared_ptr<Float> maxOff;
			shared_ptr<Float> default_offset = shared_ptr<Float> (new Float(100.0f));
			shared_ptr<Float> default_minOffset = shared_ptr<Float> (new Float(0.0f));
			shared_ptr<Float> default_maxOffset = shared_ptr<Float> (new Float(200.0f));

			off = this->getAttributeOrDefault<Float>("offset", default_offset);
			minOff = this->getAttributeOrDefault<Float>("minOffset", default_minOffset);
			maxOff = this->getAttributeOrDefault<Float>("maxOffset", default_maxOffset);

			offset    = (*off);
			minOffset = (*minOff);
			maxOffset = (*maxOff);

			// Extract Camera's limit rotation
			// -------------------------------
			shared_ptr<Float> limit_x;
			shared_ptr<Float> limit_y;
			shared_ptr<Float> limit_z;
			shared_ptr<Float> default_limit = shared_ptr<Float> (new Float(-1.0f));

			limit_x = this->getAttributeOrDefault<Float>("limitRotationOnAxisX", default_limit);
			limit_y = this->getAttributeOrDefault<Float>("limitRotationOnAxisY", default_limit);
			limit_z = this->getAttributeOrDefault<Float>("limitRotationOnAxisZ", default_limit);

			limitRotationAroundAxisX = (*limit_x);
			limitRotationAroundAxisY = (*limit_y);
			limitRotationAroundAxisZ = (*limit_z);

			// Extract Camera's amount rotation
			// --------------------------------
			shared_ptr<Float> amount_x;
			shared_ptr<Float> amount_y;
			shared_ptr<Float> amount_z;
			shared_ptr<Float> default_amount = shared_ptr<Float> (new Float(0.0f));

			amount_x = this->getAttributeOrDefault<Float>("amountRotationOnAxisX", default_amount);
			amount_y = this->getAttributeOrDefault<Float>("amountRotationOnAxisY", default_amount);
			amount_z = this->getAttributeOrDefault<Float>("amountRotationOnAxisZ", default_amount );

			amountRotationAroundAxisX = *amount_x;
			amountRotationAroundAxisY = *amount_y;
			amountRotationAroundAxisZ = *amount_z;
		}

		CameraThirdPerson::~CameraThirdPerson()
		{
			// limit
			// -----
			shared_ptr<Float> limitX(new Float(getLimitRotationAroundAxisX()));
			shared_ptr<Float> limitY(new Float(getLimitRotationAroundAxisY()));
			shared_ptr<Float> limitZ(new Float(getLimitRotationAroundAxisZ()));
			this->setAttribute("limitRotationOnAxisX", limitX);
			this->setAttribute("limitRotationOnAxisY", limitY);
			this->setAttribute("limitRotationOnAxisZ", limitZ);

			// amount
			// ------
			shared_ptr<Float> amountX(new Float(getAmountRotationAroundAxisX()));
			shared_ptr<Float> amountY(new Float(getAmountRotationAroundAxisY()));
			shared_ptr<Float> amountZ(new Float(getAmountRotationAroundAxisZ()));
			this->setAttribute("amountRotationOnAxisX", amountX);
			this->setAttribute("amountRotationOnAxisY", amountY);
			this->setAttribute("amountRotationOnAxisZ", amountZ);

			// offset settings
			// ---------------
			shared_ptr<Float> offset(new Float(getOffset()));
			shared_ptr<Float> minOffset(new Float(getMinOffset()));
			shared_ptr<Float> maxOffset(new Float(getMaxOffset()));
			this->setAttribute("offset", offset);
			this->setAttribute("minOffset", minOffset);
			this->setAttribute("maxOffset", maxOffset);
		}

		void CameraThirdPerson::setOffset(float value)
		{
			if(value < 1)
				return;

			if(value < getMinOffset())
				return;

			if(value > getMaxOffset())
				return;

			offset = fabs(value);
			setTarget(getTargetName());
		}

		void CameraThirdPerson::setMinOffset(float value)
		{
			if(value < 0)
				return;

			if(value > getMaxOffset())
				return;

			if(value > getOffset())
				setOffset(value);

			minOffset = fabs(value);
		}

		void CameraThirdPerson::setMaxOffset(float value)
		{
			if(value < getMinOffset())
				return;

			if(value < getOffset())
				setOffset(value);

			maxOffset = fabs(value);
		}

		float CameraThirdPerson::getOffset()
		{
			return offset;
		}

		float CameraThirdPerson::getMinOffset()
		{
			return minOffset;
		}

		float CameraThirdPerson::getMaxOffset()
		{
			return maxOffset;
		}

		float CameraThirdPerson::getLimitRotationAroundAxisX() const
		{
			return limitRotationAroundAxisX;
		}

		float CameraThirdPerson::getLimitRotationAroundAxisY() const
		{
			return limitRotationAroundAxisY;
		}

		float CameraThirdPerson::getLimitRotationAroundAxisZ() const
		{
			return limitRotationAroundAxisZ;
		}

		float CameraThirdPerson::getAmountRotationAroundAxisX() const
		{
			return amountRotationAroundAxisX;
		}

		float CameraThirdPerson::getAmountRotationAroundAxisY() const
		{
			return amountRotationAroundAxisY;
		}

		float CameraThirdPerson::getAmountRotationAroundAxisZ() const
		{
			return amountRotationAroundAxisZ;
		}

		void CameraThirdPerson::setLimitRotationAroundAxisX(float xLimit)
		{
			if(xLimit == -1)
			{
				limitRotationAroundAxisX = -1;
				return;
			}

			if(xLimit < 1)
				return;

			if(xLimit < getAmountRotationAroundAxisX())
				return;

			limitRotationAroundAxisX = std::fmod(xLimit, 360);
		}

		void CameraThirdPerson::setLimitRotationAroundAxisY(float yLimit)
		{
			if(yLimit == -1)
			{
				limitRotationAroundAxisY = -1;
				return;
			}

			if(yLimit < 1)
				return;

			if(yLimit < getAmountRotationAroundAxisY())
				return;

			limitRotationAroundAxisX = std::fmod(yLimit, 360);
		}

		void CameraThirdPerson::setLimitRotationAroundAxisZ(float zLimit)
		{
			if(zLimit == -1)
			{
				limitRotationAroundAxisZ = -1;
				return;
			}

			if(zLimit < 1)
				return;

			if(zLimit < getAmountRotationAroundAxisZ())
				return;

			limitRotationAroundAxisZ = std::fmod(zLimit, 360);
		}

		void CameraThirdPerson::rotateAroundAxisX(float degree)
		{
			if(getLimitRotationAroundAxisX() >= 0)
				if(getAmountRotationAroundAxisX() + degree > getLimitRotationAroundAxisX())
					return;

			amountRotationAroundAxisX += degree;
			amountRotationAroundAxisX = std::fmod(amountRotationAroundAxisX, 360);

			getOgreCamera()->pitch(Ogre::Radian(Ogre::Degree(degree * mapMovement["ROTATION_AXIS_X"])));
		}

		void CameraThirdPerson::rotateAroundAxisY(float degree)
		{
			if(getLimitRotationAroundAxisY() >= 0)
				if(getAmountRotationAroundAxisY() + degree > getLimitRotationAroundAxisY())
					return;

			amountRotationAroundAxisY += degree;
			amountRotationAroundAxisY = std::fmod(amountRotationAroundAxisY, 360);

			getOgreCamera()->yaw(Ogre::Radian(Ogre::Degree(degree * mapMovement["ROTATION_AXIS_Y"])));
		}

		void CameraThirdPerson::rotateAroundAxisZ(float degree)
		{
			if(getLimitRotationAroundAxisZ() >= 0)
				if(getAmountRotationAroundAxisZ() + degree > getLimitRotationAroundAxisZ())
					return;

			amountRotationAroundAxisZ += degree;
			amountRotationAroundAxisZ = std::fmod(amountRotationAroundAxisZ, 360);

			getOgreCamera()->roll(Ogre::Radian(Ogre::Degree(degree * mapMovement["ROTATION_AXIS_Z"])));
		}

		void CameraThirdPerson::update(float time)
		{
			// Check if we have a target
			// -------------------------
			if(getTargetName() == "")
				return;

			setPosition(getTarget()->getPosition() + (getDirection() * getOffset() * -1.0f));
		}
	};
};
