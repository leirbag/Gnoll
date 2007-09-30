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
|   This is a free fly camera                                               |
|                                                                           |
|   Changelog :                                                             |
|               08/30/2007 - Gabriel - Initial release                      |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include <OgreVector3.h>
#include "camera.h" 
#include "../../core/include/cmessagelistener.h"
#include "../../input/include/coisinputmodule.h"
#include "../../input/include/cinputmouseevents.h"

#include "../../core/include/cmessage.h"
#include "../../core/include/cmessagetype.h"
#include "../../core/include/cmessagemanager.h"
#include "../../core/include/cgenericmessagemanager.h"

#ifndef __CAMERAFREEFLY_H__
#define __CAMERAFREEFLY_H__

namespace Viracocha
{

	namespace Core 
	{
		class StrafeCameraFreeFlyListener;
		class MoveCameraFreeFlyListener;
		class RotateCameraFreeFlyListener;

		/**
		*	This is the interface of all Sources.</br>
		*   This class cannot be instanciable.
		*/ 
		class CameraFreeFly : public Viracocha::Core::Camera
		{
		private:
			shared_ptr<CMessageListener> mylistener;
			shared_ptr<CMessageListener> mylistener1;
			shared_ptr<CMessageListener> mylistener2;

		public:

			/**
			 * This is the constructor.
			 * @param _instance This is the instance name; the Camera's name
			 */
			CameraFreeFly(const Glib::ustring& instanceName, Ogre::SceneManager* pSM);

			/**
			 * This is the destructor
			 */
			virtual ~CameraFreeFly()
			{
				CGenericMessageManager::getInstance().delListener ( mylistener, CMessageType("KEYBOARD_KEYDOWN") );
				CGenericMessageManager::getInstance().delListener ( mylistener1, CMessageType("KEYBOARD_KEYDOWN") );
				CGenericMessageManager::getInstance().delListener ( mylistener2, CMessageType("KEYBOARD_KEYDOWN") );
			}

			/**
			 * This update the View.
			 */
			virtual void update()
			{
			}

			/**
			 * This move the camera
			 */
			virtual void move(const Ogre::Vector3& dir);

			/**
			 * This rotate the camera around the axis X
			 * @param angle The angle to rotate
			 */
			virtual void rotateAxisX(float angle);

			/**
			 * This rotate the camera around the axis Y
			 * @param angle The angle to rotate
			 */
			virtual void rotateAxisY(float angle);

			/**
			 * This rotate the camera around the axis Z
			 * @param angle The angle to rotate
			 */
			virtual void rotateAxisZ(float angle);

			/**
			 * This strafe the camera on left or right
			 * @param distance The distance to strafe
			 */
			virtual void strafe(float distance);

			/**
			 * This strafe the camera on up or down
			 * @param distance The distance to strafe
			 */
			virtual void strafeUp(float distance);
				
		};

		class MoveCameraFreeFlyListener : public CMessageListener
		{
			private:
				Viracocha::Core::CameraFreeFly* m_pInstanceCam;

			public:
				
				/**
				 * This is a constructor
				 */
				MoveCameraFreeFlyListener(Viracocha::Core::CameraFreeFly* pInstanceCam) 
				{
					m_pInstanceCam = pInstanceCam;
				}

				/**
				 * This is a destructor
				 */
				virtual ~MoveCameraFreeFlyListener() 
				{
					delete m_pInstanceCam;
				}

				/**
				 * This method is called in order to process a message
				 * @param message The message this method will have to process
				 */
				virtual void handle ( shared_ptr<CMessage> message ) 
				{ 
					OIS::KeyCode key = message->getData<OIS::KeyCode>();

					if(key == OIS::KC_UP)
						m_pInstanceCam->move(Ogre::Vector3(0.0f, 0.0f, -1.0f));

					if(key == OIS::KC_DOWN)
						m_pInstanceCam->move(Ogre::Vector3(0.0f, 0.0f, 1.0f));
				}
		};

		class RotateCameraFreeFlyListener : public CMessageListener
		{
			private:
				Viracocha::Core::CameraFreeFly* m_pInstanceCam;

			public:
				
				/**
				 * This is a constructor
				 */
				RotateCameraFreeFlyListener(Viracocha::Core::CameraFreeFly* pInstanceCam) 
				{
					m_pInstanceCam = pInstanceCam;
				}

				/**
				 * This is a destructor
				 */
				virtual ~RotateCameraFreeFlyListener() 
				{
					delete m_pInstanceCam;
				}

				/**
				 * This method is called in order to process a message
				 * @param message The message this method will have to process
				 */
				virtual void handle ( shared_ptr<CMessage> message ) 
				{ 
					OIS::KeyCode key = message->getData<OIS::KeyCode>();

					if(key == OIS::KC_E)
						m_pInstanceCam->rotateAxisX(0.02);

					if(key == OIS::KC_D)
						m_pInstanceCam->rotateAxisX(-0.02);

					if(key == OIS::KC_S)
						m_pInstanceCam->rotateAxisY(0.02);

					if(key == OIS::KC_F)
						m_pInstanceCam->rotateAxisY(-0.02);
				}
		};

		class StrafeCameraFreeFlyListener : public CMessageListener
		{
			private:
				Viracocha::Core::CameraFreeFly* m_pInstanceCam;

			public:
				
				/**
				 * This is a constructor
				 */
				StrafeCameraFreeFlyListener(Viracocha::Core::CameraFreeFly* pInstanceCam) 
				{
					m_pInstanceCam = pInstanceCam;
				}

				/**
				 * This is a destructor
				 */
				virtual ~StrafeCameraFreeFlyListener() 
				{
					delete m_pInstanceCam;
				}

				/**
				 * This method is called in order to process a message
				 * @param message The message this method will have to process
				 */
				virtual void handle ( shared_ptr<CMessage> message ) 
				{ 
					OIS::KeyCode key = message->getData<OIS::KeyCode>();

					if(key == OIS::KC_C)
						m_pInstanceCam->strafe(1.0f);

					if(key == OIS::KC_V)
						m_pInstanceCam->strafe(-1.0f);

					if(key == OIS::KC_B)
						m_pInstanceCam->strafeUp(1.0f);

					if(key == OIS::KC_N)
						m_pInstanceCam->strafeUp(-1.0f);
				}
		};

	};
};

#endif // __CAMERAFREEFLY_H__
