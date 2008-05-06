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
|                                                                           |
\*-------------------------------------------------------------------------*/

#include "../include/camerathirdperson.h"
#include "../../dynamicobject/include/float.h"
#include <cmath>

using namespace Gnoll::DynamicObject;

namespace Gnoll
{
	namespace Core
	{
		struct camerathirdperson_i
		{
			float offset;
			float minOffset;
			float maxOffset;
			float amountRotationAroundAxisX;
			float amountRotationAroundAxisY;
			float amountRotationAroundAxisZ;
			float limitRotationAroundAxisX;
			float limitRotationAroundAxisY;
			float limitRotationAroundAxisZ;

			camerathirdperson_i() :
				offset(100.0f),
				minOffset(0.0f),
				maxOffset(200.0f),
				amountRotationAroundAxisX(0.0f),
				amountRotationAroundAxisY(0.0f),
				amountRotationAroundAxisZ(0.0f),
				limitRotationAroundAxisX(-1.0f),
				limitRotationAroundAxisY(-1.0f),
				limitRotationAroundAxisZ(-1.0f)
			{
			}
		};

		CameraThirdPerson::CameraThirdPerson(const Glib::ustring& instanceName) :
			Camera(instanceName),
			m_this(new camerathirdperson_i)
		{
			/**
			 * Extract Camera's offset settings
			 */
			shared_ptr<Float> offset;
			shared_ptr<Float> minOffset;
			shared_ptr<Float> maxOffset;
			shared_ptr<Float> default_offset = shared_ptr<Float> (new Float(100.0f));
			shared_ptr<Float> default_minOffset = shared_ptr<Float> (new Float(0.0f));
			shared_ptr<Float> default_maxOffset = shared_ptr<Float> (new Float(200.0f));

			offset = this->getAttributeOrDefault<Float>("offset", default_offset);
			minOffset = this->getAttributeOrDefault<Float>("minOffset", default_minOffset);
			maxOffset = this->getAttributeOrDefault<Float>("maxOffset", default_maxOffset);

			setOffset(*offset);
			setMinOffset(*minOffset);
			setMaxOffset(*maxOffset);

			/**
			 * Extract Camera's limit rotation
			 */
			shared_ptr<Float> limit_x;
			shared_ptr<Float> limit_y;
			shared_ptr<Float> limit_z;
			shared_ptr<Float> default_limit = shared_ptr<Float> (new Float(-1.0f));

			limit_x = this->getAttributeOrDefault<Float>("limitRotationOnAxisX", default_limit);
			limit_y = this->getAttributeOrDefault<Float>("limitRotationOnAxisY", default_limit);
			limit_z = this->getAttributeOrDefault<Float>("limitRotationOnAxisZ", default_limit);

			setLimitRotationAroundAxisX(*limit_x);
			setLimitRotationAroundAxisY(*limit_y);
			setLimitRotationAroundAxisZ(*limit_z);

			/**
			 * Extract Camera's amount rotation
			 */
			shared_ptr<Float> amount_x;
			shared_ptr<Float> amount_y;
			shared_ptr<Float> amount_z;
			shared_ptr<Float> default_amount = shared_ptr<Float> (new Float(0.0f));

			amount_x = this->getAttributeOrDefault<Float>("amountRotationOnAxisX", default_amount);
			amount_y = this->getAttributeOrDefault<Float>("amountRotationOnAxisY", default_amount);
			amount_z = this->getAttributeOrDefault<Float>("amountRotationOnAxisZ", default_amount );

			m_this->amountRotationAroundAxisX = *amount_x;
			m_this->amountRotationAroundAxisY = *amount_y;
			m_this->amountRotationAroundAxisZ = *amount_z;
		}

		CameraThirdPerson::CameraThirdPerson(const CameraThirdPerson& copy) :
			Camera(copy),
			m_this(new camerathirdperson_i)
		{
		}

		CameraThirdPerson::~CameraThirdPerson()
		{
			/*
			 * Save the current configuration of the camera
			 */
			// limit
			shared_ptr<Float> limitX(new Float(getLimitRotationAroundAxisX()));
			shared_ptr<Float> limitY(new Float(getLimitRotationAroundAxisY()));
			shared_ptr<Float> limitZ(new Float(getLimitRotationAroundAxisZ()));
			this->setAttribute("limitRotationOnAxisX", limitX);
			this->setAttribute("limitRotationOnAxisY", limitY);
			this->setAttribute("limitRotationOnAxisZ", limitZ);

			// amount
			shared_ptr<Float> amountX(new Float(getAmountRotationAroundAxisX()));
			shared_ptr<Float> amountY(new Float(getAmountRotationAroundAxisY()));
			shared_ptr<Float> amountZ(new Float(getAmountRotationAroundAxisZ()));
			this->setAttribute("amountRotationOnAxisX", amountX);
			this->setAttribute("amountRotationOnAxisY", amountY);
			this->setAttribute("amountRotationOnAxisZ", amountZ);

			// offset settings
			shared_ptr<Float> offset(new Float(getOffset()));
			shared_ptr<Float> minOffset(new Float(getMinOffset()));
			shared_ptr<Float> maxOffset(new Float(getMaxOffset()));
			this->setAttribute("offset", amountX);
			this->setAttribute("minOffset", amountY);
			this->setAttribute("maxOffset", amountZ);

			delete m_this;
		}

		void CameraThirdPerson::setOffset(float offset)
		{
			if(offset < 1)
				return;

			if(offset < getMinOffset())
				return;

			if(offset > getMaxOffset())
				return;

			m_this->offset = fabs(offset);
			setTarget(getTarget());
		}

		void CameraThirdPerson::setMinOffset(float value)
		{
			if(value > getMaxOffset())
				return;

			if(value > getOffset())
				setOffset(value);

			m_this->minOffset = fabs(value);
		}

		void CameraThirdPerson::setMaxOffset(float value)
		{
			if(value < getMinOffset())
				return;

			if(value < getOffset())
				setOffset(value);

			m_this->maxOffset = fabs(value);
		}

		float CameraThirdPerson::getOffset()
		{
			return m_this->offset;
		}

		float CameraThirdPerson::getMinOffset()
		{
			return m_this->minOffset;
		}

		float CameraThirdPerson::getMaxOffset()
		{
			return m_this->maxOffset;
		}

		float CameraThirdPerson::getLimitRotationAroundAxisX() const
		{
			return m_this->limitRotationAroundAxisX;
		}

		float CameraThirdPerson::getLimitRotationAroundAxisY() const
		{
			return m_this->limitRotationAroundAxisY;
		}

		float CameraThirdPerson::getLimitRotationAroundAxisZ() const
		{
			return m_this->limitRotationAroundAxisZ;
		}

		float CameraThirdPerson::getAmountRotationAroundAxisX() const
		{
			return m_this->amountRotationAroundAxisX;
		}

		float CameraThirdPerson::getAmountRotationAroundAxisY() const
		{
			return m_this->amountRotationAroundAxisY;
		}

		float CameraThirdPerson::getAmountRotationAroundAxisZ() const
		{
			return m_this->amountRotationAroundAxisZ;
		}

		void CameraThirdPerson::setLimitRotationAroundAxisX(float xLimit)
		{
			if(xLimit < 1)
				return;

			if(xLimit < getAmountRotationAroundAxisX())
				return;

			m_this->limitRotationAroundAxisX = std::fmod(xLimit, 360);
		}

		void CameraThirdPerson::setLimitRotationAroundAxisY(float yLimit)
		{
			if(yLimit < 1)
				return;

			if(yLimit < getAmountRotationAroundAxisY())
				return;

			m_this->limitRotationAroundAxisX = std::fmod(yLimit, 360);
		}

		void CameraThirdPerson::setLimitRotationAroundAxisZ(float zLimit)
		{
			if(zLimit < 1)
				return;

			if(zLimit < getAmountRotationAroundAxisZ())
				return;

			m_this->limitRotationAroundAxisZ = std::fmod(zLimit, 360);
		}

		void CameraThirdPerson::rotateAroundAxisX(float degree)
		{
			if(getLimitRotationAroundAxisX() >= 0)
				if(getAmountRotationAroundAxisX() + degree > getLimitRotationAroundAxisX())
					return;

			m_this->amountRotationAroundAxisX += degree;
			m_this->amountRotationAroundAxisX = std::fmod(m_this->amountRotationAroundAxisX, 360);

			getOgreCamera()->pitch(Ogre::Radian(Ogre::Degree(degree)));
		}

		void CameraThirdPerson::rotateAroundAxisY(float degree)
		{
			if(getLimitRotationAroundAxisY() >= 0)
				if(getAmountRotationAroundAxisY() + degree > getLimitRotationAroundAxisY())
					return;

			m_this->amountRotationAroundAxisY += degree;
			m_this->amountRotationAroundAxisY = std::fmod(m_this->amountRotationAroundAxisY, 360);

			getOgreCamera()->yaw(Ogre::Radian(Ogre::Degree(degree)));
		}

		void CameraThirdPerson::rotateAroundAxisZ(float degree)
		{
			if(getLimitRotationAroundAxisZ() >= 0)
				if(getAmountRotationAroundAxisZ() + degree > getLimitRotationAroundAxisZ())
					return;

			m_this->amountRotationAroundAxisZ += degree;
			m_this->amountRotationAroundAxisZ = std::fmod(m_this->amountRotationAroundAxisZ, 360);

			getOgreCamera()->roll(Ogre::Radian(Ogre::Degree(degree)));
		}

		void CameraThirdPerson::update(float time)
		{
			setPosition(getTarget()->getPosition() + (getDirection() * getOffset() * -1.0f));
		}
	};
};
