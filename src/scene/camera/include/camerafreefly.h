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

#ifndef INCLUDED_CAMERAFREEFLY
#define INCLUDED_CAMERAFREEFLY

#include "camera.h"

namespace Gnoll
{
	namespace Scene
	{
		class CameraFreeFly : public Camera
		{
			public:
				/*
				 * Default constructor, (see camera) + offset set at 100, min offset to 0 and max offset to 200
				 * this is no limit on rotation
				 * @param instanceName This is the instance name of the camera, it will be use for the
				 * 					   instance name of the Ogre Camera
				 */
				explicit CameraFreeFly(const Glib::ustring& instanceName, shared_ptr<CameraWrapper> wrapper);

				/*
				 * Destructior
				 */
				virtual ~CameraFreeFly();

				/*
				 * Strafe the camera in right or left direction
				 * @param speed Negative value means left and Positive means right
				 */
				void strafeLR(float speed);

				/*
				 * Strafe the camera in up or down direction
				 * @param speed Negative value means down and Positive means up
				 */
				void strafeUD(float speed);

				/*
				 * Move the camera in the Z position
				 * @param speed The value to move
				 */
				void move(float speed);

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

			private:
				float m_rotationX;
				float m_rotationY;
				float m_rotationZ;
		};
	};
};

#endif
