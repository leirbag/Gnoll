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


/*---------------------------------camera----------------------------------*\
|   This is the interface of all camera                                     |
|                                                                           |
|   Changelog :                                                             |
|               08/30/2007 - Gabriel - Initial release                      |
|               10/30/2007 - Gabriel - add time to update()                 |
|               12/17/2007 - Paf - Camera inherits of CPersistentObjectProxy|      
|               12/24/2007 - Gabriel - delete scenemanager from Ctor        |
|               02/15/2008 - Bruno Mahe - Implements destructor             |
|               04/08/2008 - Gabriel - total refactoring                    |
|               04/10/2008 - Gabriel - Add the enqueue of listener          |
\*-------------------------------------------------------------------------*/

#ifndef INCLUDED_CAMERA
#define INCLUDED_CAMERA

#include <glibmm/ustring.h>
#include <OgreSceneNode.h>
#include <OgreVector3.h>
#include <OgreCamera.h>
#include "cpersistentobjectproxy.h"

namespace Gnoll
{
	namespace Core
	{
		/*
		 * Structure that contains camera attributs
		 */
		struct camera_i;

		class CMessageListenerCamera;
		class CMessageType;

		class Camera : public CPersistentObjectProxy
		{
		private:
			/*
			 * This is a pointer to camera attributs
			 */
			camera_i* m_this;

		protected:
			/*
			 * Default constructor, it initializes the camera with default settings :
			 * position (0, 0, 0), direction (0, 0, 1), up (0, 1, 0), near 0, far is 200,
			 * fov PI/4 OR get back ancient configuration with persistant objet if exists
			 * @param instanceName This is the instance name of the camera, it will be use for the
			 * 					   instance name of the Ogre Camera
			 */
			explicit Camera(const Glib::ustring& instanceName);

			/*
			 * Copy constructor
			 * @param copy This is the camera to copy
			 */
			explicit Camera(const Camera& copy);

			/*
			 * Helper method for setTarget()
			 * @param (see setTarget)
			 */
			void setTargetHelper(Ogre::SceneNode* target);

		public:
			/*
			 * Destructior
			 */
			virtual ~Camera();

			/*
			 * Operator to copy a camera
			 * @param copy This is the camera to copy
			 */
			virtual Camera& operator=(const Camera& copy);

			/*
			 * Acessor to the near value
			 * @return the near value
			 */
			float getNearValue() const;

			/*
			 * Acessor to the far value
			 * @return the far value
			 */
			float getFarValue() const;

			/*
			 * Acessor to the fov value
			 * @return the fov value
			 */
			float getFovValue() const;

			/*
			 * Accessor to get the position
			 * @return the current position of the camera
			 */
			const Ogre::Vector3& getPosition() const;

			/*
			 * Accessor to get the direction
			 * @return the current direction of the camera
			 */
			Ogre::Vector3 getDirection() const;

			/*
			 * Accessor to get the target
			 * @return the scene node of the current target, NULL if default setting
			 */
			Ogre::SceneNode* getTarget() const;

			/*
			 * Accessor to get the orientation
			 * @return the current orientation of the camera
			 */
			Ogre::Quaternion getOrientation() const;

			/*
			 * Accessor to get the up vector
			 * @return the current up vector
			 */
			Ogre::Vector3 getUp() const;

			/*
			 * Accessor to get the current view matrix
			 * @return the current view matrix
			 */
			Ogre::Matrix4 getView() const;

			/*
			 * Accessor to get the OgreCamera
			 * @return the OgreCamera
			 */
			Ogre::Camera* getOgreCamera();

			/*
			 * Settor to set the near value
			 * @param near This is the near value to set, the method take the absolute value
			 * 			   of this parameter, and must be more less than far value (not equal)
			 */
			void setNearValue(float near);

			/*
			 * Settor to set the far value
			 * @param far This is the far value to set, the method take the absolute value
			 * 			  of this parameter, and must be greater than near value (not equal)
			 */
			void setFarValue(float far);

			/*
			 * Settor to set the fov value
			 * @param fov This is the fov value to set, the method take the absolute value
			 */
			void setFovValue(float fov);

			/*
			 * Settor to set the new position of the camera
			 * @param position This is the new position, it must be different to the target
			 * 				   position (see setPosition of OgreCamera)
			 */
			void setPosition(const Ogre::Vector3& position);

		    /*
			 * Settor to set the new target of the camera
			 * @param target This is the new target, NULL for no target
			 */
			virtual void setTarget(Ogre::SceneNode* target);

			/*
			 * Settor to set the orientation
			 * @param orientation This is the new orientation
			 */
			void setOrientation(const Ogre::Quaternion& orientation);

			/*
			 * Put a new listener in the queue of the camera
			 * @param listener This is the listener to put on the queue
			 * @param type This is the type of message associate to the listener
			 */
			void enqueueListener(shared_ptr<CMessageListenerCamera> listener, shared_ptr<CMessageType> type);

			/*
			 * This method is call each frame
			 * @param time This is the time between 2 frames
			 */
			virtual void update(float time);
		};
	};
};

#endif
