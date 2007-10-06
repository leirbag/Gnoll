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


/*-------------------------------cmessage----------------------------------*\
|   This is a free fly camera                                               |
|                                                                           |
|   Changelog :                                                             |
|               08/31/2007 - Gabriel - Initial release                      |
|               10/06/2007 - Gabriel - Add hack to enable key repeat        |
|                                    - Add management of mouse for the      |
|									   rotation                             |
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

namespace Gnoll
{

	namespace Core 
	{
		class StrafeCameraFreeFlyListener;
		class MoveCameraFreeFlyListener;
		class RotateCameraFreeFlyListener;
		class MouseRotateCameraFreeFlyListener;

		/**
		*	This is the interface of all Sources.</br>
		*   This class cannot be instanciable.
		*/ 
		class CameraFreeFly : public Gnoll::Core::Camera
		{
		private:
			shared_ptr<CMessageListener> m_listenerMove;
			shared_ptr<CMessageListener> m_listenerRotate;
			shared_ptr<CMessageListener> m_listenerMouseRotate;
			shared_ptr<CMessageListener> m_listenerStrafe;

			shared_ptr<CMessageListener> m_listenerKeyUp;
			shared_ptr<CMessageListener> m_listenerKeyDown;

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
				CGenericMessageManager::getInstancePtr()->delListener ( m_listenerMove, CMessageType("GRAPHIC_FRAME_RENDERED") );
				CGenericMessageManager::getInstancePtr()->delListener ( m_listenerRotate, CMessageType("GRAPHIC_FRAME_RENDERED") );
				CGenericMessageManager::getInstancePtr()->delListener ( m_listenerStrafe, CMessageType("GRAPHIC_FRAME_RENDERED") );
				CGenericMessageManager::getInstancePtr()->delListener ( m_listenerKeyUp, CMessageType("KEYBOARD_KEYUP") );
				CGenericMessageManager::getInstancePtr()->delListener ( m_listenerKeyDown, CMessageType("KEYBOARD_KEYDOWN") );
				CGenericMessageManager::getInstancePtr()->delListener ( m_listenerMouseRotate, CMessageType("MOUSE_MOVED") );
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


		// Will be change by a std::map later ... maybe ...
		static bool cffKeyMoveUp   = false;
		static bool cffKeyMoveDown = false;

		static bool cffKeyStrafeUp    = false;
		static bool cffKeyStrafeDown  = false;
		static bool cffKeyStrafeLeft  = false;
		static bool cffKeyStrafeRight = false;

		static bool cffKeyRotateUp    = false;
		static bool cffKeyRotateDown  = false;
		static bool cffKeyRotateLeft  = false;
		static bool cffKeyRotateRight = false;

		class KeyUp : public CMessageListener
		{
			public:
				
				/**
				 * This is a constructor
				 */
				KeyUp() 
				{
				}

				/**
				 * This is a destructor
				 */
				virtual ~KeyUp() 
				{
				}

				/**
				 * This method is called in order to process a message
				 * @param message The message this method will have to process
				 */
				virtual void handle ( shared_ptr<CMessage> message ) 
				{ 
					OIS::KeyCode key = message->getData<OIS::KeyCode>();

					if(key == OIS::KC_UP)
						cffKeyMoveUp = false;

					if(key == OIS::KC_DOWN)
						cffKeyMoveDown = false;

					if(key == OIS::KC_E)
						cffKeyRotateUp = false;

					if(key == OIS::KC_D)
						cffKeyRotateDown = false;

					if(key == OIS::KC_S)
						cffKeyRotateLeft = false;

					if(key == OIS::KC_F)
						cffKeyRotateRight = false;

					if(key == OIS::KC_C)
						cffKeyStrafeRight = false;

					if(key == OIS::KC_V)
						cffKeyStrafeLeft = false;

					if(key == OIS::KC_B)
						cffKeyStrafeUp = false;

					if(key == OIS::KC_N)
						cffKeyStrafeDown = false;
				}
		};

		class KeyDown : public CMessageListener
		{
			public:
				
				/**
				 * This is a constructor
				 */
				KeyDown() 
				{
				}

				/**
				 * This is a destructor
				 */
				virtual ~KeyDown() 
				{
				}

				/**
				 * This method is called in order to process a message
				 * @param message The message this method will have to process
				 */
				virtual void handle ( shared_ptr<CMessage> message ) 
				{ 
					OIS::KeyCode key = message->getData<OIS::KeyCode>();

					if(key == OIS::KC_UP)
						cffKeyMoveUp = true;

					if(key == OIS::KC_DOWN)
						cffKeyMoveDown = true;

					if(key == OIS::KC_E)
						cffKeyRotateUp = true;

					if(key == OIS::KC_D)
						cffKeyRotateDown = true;

					if(key == OIS::KC_S)
						cffKeyRotateLeft = true;

					if(key == OIS::KC_F)
						cffKeyRotateRight = true;

					if(key == OIS::KC_C)
						cffKeyStrafeRight = true;

					if(key == OIS::KC_V)
						cffKeyStrafeLeft = true;

					if(key == OIS::KC_B)
						cffKeyStrafeUp = true;

					if(key == OIS::KC_N)
						cffKeyStrafeDown = true;
				}
		};

		class MoveCameraFreeFlyListener : public CMessageListener
		{
			private:
				Gnoll::Core::CameraFreeFly* m_pInstanceCam;

			public:
				
				/**
				 * This is a constructor
				 */
				MoveCameraFreeFlyListener(Gnoll::Core::CameraFreeFly* pInstanceCam) 
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
					unsigned long lasttime = message->getData<unsigned long>();

					if(cffKeyMoveUp)
						m_pInstanceCam->move(Ogre::Vector3(0.0f, 0.0f, -1.0f));

					if(cffKeyMoveDown)
						m_pInstanceCam->move(Ogre::Vector3(0.0f, 0.0f, 1.0f));
				}
		};

		class RotateCameraFreeFlyListener : public CMessageListener
		{
			private:
				Gnoll::Core::CameraFreeFly* m_pInstanceCam;

			public:
				
				/**
				 * This is a constructor
				 */
				RotateCameraFreeFlyListener(Gnoll::Core::CameraFreeFly* pInstanceCam) 
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
					if(cffKeyRotateUp)
						m_pInstanceCam->rotateAxisX(0.02);

					if(cffKeyRotateDown)
						m_pInstanceCam->rotateAxisX(-0.02);

					if(cffKeyRotateLeft)
						m_pInstanceCam->rotateAxisY(0.02);

					if(cffKeyRotateRight)
						m_pInstanceCam->rotateAxisY(-0.02);
				}
		};

		class MouseRotateCameraFreeFlyListener : public CMessageListener
		{
			private:
				Gnoll::Core::CameraFreeFly* m_pInstanceCam;

			public:
				
				/**
				 * This is a constructor
				 */
				MouseRotateCameraFreeFlyListener(Gnoll::Core::CameraFreeFly* pInstanceCam) 
				{
					m_pInstanceCam = pInstanceCam;
				}

				/**
				 * This is a destructor
				 */
				virtual ~MouseRotateCameraFreeFlyListener() 
				{
					delete m_pInstanceCam;
				}

				/**
				 * This method is called in order to process a message
				 * @param message The message this method will have to process
				 */
				virtual void handle ( shared_ptr<CMessage> message ) 
				{ 
					MouseEvent event = message->getData<MouseEvent>();

					m_pInstanceCam->rotateAxisY(-(event.relX / 20.0f) * 3.14 / 180);
					m_pInstanceCam->rotateAxisX(-(event.relY / 20.0f) * 3.14 / 180);
				}
		};

		class StrafeCameraFreeFlyListener : public CMessageListener
		{
			private:
				Gnoll::Core::CameraFreeFly* m_pInstanceCam;

			public:
				
				/**
				 * This is a constructor
				 */
				StrafeCameraFreeFlyListener(Gnoll::Core::CameraFreeFly* pInstanceCam) 
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
					if(cffKeyStrafeLeft)
						m_pInstanceCam->strafe(1.0f);

					if(cffKeyStrafeRight)
						m_pInstanceCam->strafe(-1.0f);

					if(cffKeyStrafeUp)
						m_pInstanceCam->strafeUp(1.0f);

					if(cffKeyStrafeDown)
						m_pInstanceCam->strafeUp(-1.0f);
				}
		};

	};
};

#endif // __CAMERAFREEFLY_H__
