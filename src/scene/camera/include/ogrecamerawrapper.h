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

#ifndef INCLUDED_OGRECAMERAWRAPPER
#define INCLUDED_OGRECAMERAWRAPPER

#include <glibmm/ustring.h>
#include <OgreCamera.h>
#include <boost/shared_ptr.hpp>

#include "camerawrapper.h"

using namespace boost;

namespace Gnoll
{
	namespace Scene
	{
		class OgreCameraWrapper : public CameraWrapper
		{
			public:
				/*
				 * Constructor
				 * @param name The name of the camera
				 */
				OgreCameraWrapper(const Glib::ustring& name);

				/*
				 * Destructor
				 */
				virtual ~OgreCameraWrapper();

				/*
				 * Set the near value
				 * @param value The value
				 */
				virtual void setNearValue(float value);

				/*
				 * Get the near value
				 * @return the near value
				 */
				virtual float getNearValue();

				/*
				 * Set the far value
				 * @param value The value
				 */
				virtual void setFarValue(float value);

				/*
				 * Get the far value
				 * @return the far value
				 */
				virtual float getFarValue();

				/*
				 * Set the fov value
				 * @param value The value
				 */
				virtual void setFovValue(float value);

				/*
				 * Get the fov value
				 * @return the fov value
				 */
				virtual float getFovValue();

				/*
				 * Set the position of the camera
				 * @param value The position
				 */
				virtual void setPosition(const Ogre::Vector3& value);

				/*
				 * Get the position of the camera
				 * @return the position of the camera
				 */
				virtual Ogre::Vector3 getPosition();

				/*
				 * Set the direction of the camera
				 * @param value The direction
				 */
				virtual void setDirection(const Ogre::Vector3& value);

				/*
				 * Get the direction of the camera
				 * @return the durection of the camera
				 */
				virtual Ogre::Vector3 getDirection();

				/*
				 * Set the orientation of the camera
				 * @param value The orientation
				 */
				virtual void setOrientation(const Ogre::Quaternion& value);

				/*
				 * Get the orientation of the camera
				 * @return the orientation of the camera
				 */
				virtual Ogre::Quaternion getOrientation();

				/*
				 * Get the orientation of the camera
				 * @return the orientation of the camera
				 */
				virtual Ogre::Vector3 getUp();

				/*
				 * Get the orientation of the camera
				 * @return the orientation of the camera
				 */
				virtual Ogre::Vector3 getRight();

				/*
				 * Set the auto tracking of the camera around a target
				 * @param autofocus If we want to have always the orientation on the target
				 * @param target The target to track
				 */
				virtual void setAutoTracking(bool autofocus, Ogre::SceneNode* target);

				/*
				 * Rotate the camera around the X axis
				 * @param value Rotation in radian
				 */
				virtual void pitch(Ogre::Radian value);

				/*
				 * Rotate the camera around the Y axis
				 * @param value Rotation in radian
				 */
				virtual void yaw(Ogre::Radian value);

				/*
				 * Rotate the camera around the Z axis
				 * @param value Rotation in radian
				 */
				virtual void roll(Ogre::Radian value);

			protected:
				Ogre::Camera* camera;
		};
	};
};

#endif
