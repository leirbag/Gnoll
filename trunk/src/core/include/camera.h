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


/*---------------------------------integer---------------------------------*\
|   This is the interface of all camera                                     |
|                                                                           |
|   Changelog :                                                             |
|               08/30/2007 - Gabriel - Initial release                      |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include <glibmm/ustring.h>
#include <OgreVector3.h>
#include <OgreCamera.h>
#include <OgreSceneManager.h>
#include "persistentobject.h" 

#ifndef __CAMERA_H__
#define __CAMERA_H__

namespace Viracocha
{

	namespace Core 
	{

		/**
		*	This is the interface of all Sources.</br>
		*   This class cannot be instanciable.
		*/ 
		class Camera : public PersistentObject
		{
		protected:
			/**
			 * This is the Ogre camera
			 */ 
			Ogre::Camera* m_ogreCamera;

		public:

			/**
			 * This is the constructor.
			 * @param _instance This is the instance name; the Camera's name
			 */
			Camera(const Glib::ustring& instanceName, Ogre::SceneManager* pSM) : PersistentObject(instanceName)
			{
				m_ogreCamera = pSM->createCamera(instanceName);
			}

			/**
			 * This is the destructor
			 */
			virtual ~Camera()
			{
			}

			/**
			 * This update the View.
			 * Method virtual pure
			 */
			virtual void update() = 0;

			/**
			 * Get the look at of the current camera
			 * @return the position of the look at
			 */
			virtual Ogre::Vector3 getLookAt()
			{
				return m_ogreCamera->getRealDirection();
			}

			/**
			 * Set the look at to the current camera
			 */
			virtual void setLookAt(const Ogre::Vector3& vLookAt)
			{
				m_ogreCamera->lookAt(vLookAt);
			}

			/**
			 * Get the up of the current camera
			 * @return the position of the up
			 */
			virtual Ogre::Vector3 getUp()
			{
				return m_ogreCamera->getRealUp();
			}

			/**
			 * Get the eye of the current camera
			 * @return the position of the eye
			 */
			virtual Ogre::Vector3 getEye()
			{
				return m_ogreCamera->getRealPosition();
			}

			/**
			 * Set the eye to the current camera
			 */
			virtual void setEye(const Ogre::Vector3& vEye)
			{
				m_ogreCamera->setPosition(vEye);
			}

			/**
			 * Set the near distance to the current camera
			 */
			virtual void setNearDistance(float distance)
			{
				m_ogreCamera->setNearClipDistance(distance);
			}

			/**
			 * Set the far distance to the current camera
			 */
			virtual void setFarDistance(float distance)
			{
				m_ogreCamera->setFarClipDistance(distance);
			}

			/**
			 * Set the far distance to the current camera
			 */
			virtual void setFov(float angle)
			{
				m_ogreCamera->setAspectRatio(angle);
			}

			/**
			 * Get the instance of the current ogre camera
			 * @return the ogre camera
			 */
			virtual Ogre::Camera& getOgreCamera()
			{
				return *m_ogreCamera;
			}
				
		};
	};
};

#endif // __CAMERA_H__
