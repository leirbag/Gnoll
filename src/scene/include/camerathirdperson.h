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
 * This is an implementation of a Third Person Camera, it provides some    *
 * services:                                                               *
 * 	- Set up of a "head" position                                          *
 * 	- Set up of an offset between the camera and target                    *
 * 	- Set up of a min and max offset                                       *
 * 	- Defines a rotation limit for each axes                               *
 * 	- Rotation on each axe around the target                               *
 ***************************************************************************/

#ifndef INCLUDED_CAMERATHIRDPERSON
#define INCLUDED_CAMERATHIRDPERSON

#include "camera.h"

namespace Gnoll
{
	namespace Scene
	{
		class CameraThirdPerson : public Camera
		{
		private:
			/*
			 * Parameters of the camera
			 */
			float offset;
			float minOffset;
			float maxOffset;
			float amountRotationAroundAxisX;
			float amountRotationAroundAxisY;
			float amountRotationAroundAxisZ;
			float limitRotationAroundAxisX;
			float limitRotationAroundAxisY;
			float limitRotationAroundAxisZ;

		public:
			/*
			 * Default constructor, (see camera) + offset set at 100, min offset to 0 and max offset to 200
			 * this is no limit on rotation
			 * @param instanceName This is the instance name of the camera, it will be use for the
			 * 					   instance name of the Ogre Camera
			 */
			explicit CameraThirdPerson(const Glib::ustring& instanceName, shared_ptr<CameraWrapper> wrapper);

			/*
			 * Destructior
			 */
			virtual ~CameraThirdPerson();

			/**
			 * This set the offset between camera and target
			 * @param offset This is the offset, we take the absolute value
			 * 				 and the value must be between MinOffset and MaxOffset
			 */
			void setOffset(float offset);

			/**
			 * This set the minimum amount of offset
			 * @param value This is the min offset, we take the asolute value
			 * 				and the value must be inferior to MaxOffset, if
			 * 				the new value is greater than current offset, the
			 * 				offset is set to the new MinOffset
			 */
			void setMinOffset(float value);

			 /**
			 * This set the maximum amount of offset
			 * @param value This is the max offset, we take the asolute value
			 * 				and the value must be superior to MinOffset, if
			 * 				the new value is inferior to current offset, the
			 * 				offset is set to the new MaxOffset
			 */
			void setMaxOffset(float value);

			/**
			 * This return the offset between camera and target
			 * @return the current offset
			 */
			float getOffset();

			/**
			 * This return the min amount offset between camera and target
			 * @return the min offset
			 */
			float getMinOffset();

			/**
			 * This return the max amount offset between camera and target
			 * @return the max offset
			 */
			float getMaxOffset();

			/*
			 * Accessor to get the current limit rotation on X Axis
			 * @return the limit of rotation on axis X
			 */
			float getLimitRotationAroundAxisX() const;

			/*
			 * Accessor to get the current limit rotation on Y Axis
			 * @return the limit of rotation on axis Y
			 */
			float getLimitRotationAroundAxisY() const;

			/*
			 * Accessor to get the current limit rotation on Z Axis
			 * @return the limit of rotation on axis Z
			 */
			float getLimitRotationAroundAxisZ() const;

			/*
			 * Accessor to get the current amount of rotation on X Axis
			 * @return the amount of rotation on axis X
			 */
			float getAmountRotationAroundAxisX() const;

			/*
			 * Accessor to get the current amount rotation on Y Axis
			 * @return the amount of rotation on axis Y
			 */
			float getAmountRotationAroundAxisY() const;

			/*
			 * Accessor to get the current amount rotation on Z Axis
			 * @return the amount of rotation on axis Z
			 */
			float getAmountRotationAroundAxisZ() const;

			/*
			 * Settor to set the limit of rotation aroud the target
			 * around the X axis
			 * @param xLimit This is the limit value, it must be positive and greater
			 * 				 than the current amount of rotation on axis X else no
			 * 				 changement, -1 is allowed to indicate no limit
			 */
			void setLimitRotationAroundAxisX(float xLimit);

			/*
			 * Settor to set the limit of rotation aroud the target
			 * around the Y axis
			 * @param yLimit This is the limit value, it must be positive and greater
			 * 				 than the current amount of rotation on axis Y else no
			 * 				 changement, -1 is allowed to indicate no limit
			 */
			void setLimitRotationAroundAxisY(float yLimit);

			/*
			 * Settor to set the limit of rotation aroud the target
			 * around the z axis
			 * @param zLimit This is the limit value, it must be positive and greater
			 * 				 than the current amount of rotation on axis Z else no
			 * 				 changement, -1 is allowed to indicate no limit
			 */
			void setLimitRotationAroundAxisZ(float zLimit);

			/*
			 * Rotate the camera around the targed around the X axis
			 * It doesn't rotate if the rotation is greater than limit
			 * fixed
			 * @param degree This is the angle to rotation in degree
			 */
			void rotateAroundAxisX(float degree);

			/*
			 * Rotate the camera around the targed around the Y axis
			 * It doesn't rotate if the rotation is greater than limit
			 * fixed
			 * @param degree This is the angle to rotation in degree
			 */
			void rotateAroundAxisY(float degree);

			/*
			 * Rotate the camera around the targed around the Z axis
			 * It doesn't rotate if the rotation is greater than limit
			 * fixed
			 * @param degree This is the angle to rotation in degree
			 */
			void rotateAroundAxisZ(float degree);

			/*
			 * This method is call each frame
			 * @param time This is the time between 2 frames
			 */
			void update(float time);
		};
	};
};

#endif
