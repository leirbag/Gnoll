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
|               06/10/2007 - Gabriel - Add hack to enable key repeat        |
|                                    - Add management of mouse for the      |
|									   rotation                             |
|               10/10/2007 - Gabriel - Change all variables for listener    |
|                                      by a map.                            |
|									   Add time to the transformation       |
|               10/30/2007 - Gabriel - add time to update()                 |
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
			virtual void update(float time)
			{
			}

			/**
			 * This move the camera
			 */
			virtual void move(const Ogre::Vector3& dir);

			/**
			 * This rotate the camera around the axis X
			 * @param angle The angle to rotate in degree
			 */
			virtual void rotateAxisX(float angle);

			/**
			 * This rotate the camera around the axis Y
			 * @param angle The angle to rotate in degree
			 */
			virtual void rotateAxisY(float angle);

			/**
			 * This rotate the camera around the axis Z
			 * @param angle The angle to rotate in degree
			 */
			virtual void rotateAxisZ(float angle);

			/**
			 * This strafe the camera on left or right
			 * @param distance The distance to strafe (left or right)
			 */
			virtual void strafe(float distance);

			/**
			 * This strafe the camera on up or down
			 * @param distance The distance to strafe (up or down)
			 */
			virtual void strafeUp(float distance);
				
		};


		// Map for the key listener
		static std::map<std::string, bool> g_mapCffKeys;

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
						g_mapCffKeys["MoveUp"] = false;

					if(key == OIS::KC_DOWN)
						g_mapCffKeys["MoveDown"] = false;

					if(key == OIS::KC_E)
						g_mapCffKeys["RotateUp"] = false;

					if(key == OIS::KC_D)
						g_mapCffKeys["RotateDown"] = false;

					if(key == OIS::KC_S)
						g_mapCffKeys["RotateLeft"] = false;

					if(key == OIS::KC_F)
						g_mapCffKeys["RotateRight"] = false;

					if(key == OIS::KC_C)
						g_mapCffKeys["StrafeRight"] = false;

					if(key == OIS::KC_V)
						g_mapCffKeys["StrafeLeft"] = false;

					if(key == OIS::KC_B)
						g_mapCffKeys["StrafeUp"] = false;

					if(key == OIS::KC_N)
						g_mapCffKeys["StrafeDown"] = false;
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
						g_mapCffKeys["MoveUp"] = true;

					if(key == OIS::KC_DOWN)
						g_mapCffKeys["MoveDown"] = true;

					if(key == OIS::KC_E)
						g_mapCffKeys["RotateUp"] = true;

					if(key == OIS::KC_D)
						g_mapCffKeys["RotateDown"] = true;

					if(key == OIS::KC_S)
						g_mapCffKeys["RotateLeft"] = true;

					if(key == OIS::KC_F)
						g_mapCffKeys["RotateRight"] = true;

					if(key == OIS::KC_C)
						g_mapCffKeys["StrafeRight"] = true;

					if(key == OIS::KC_V)
						g_mapCffKeys["StrafeLeft"] = true;

					if(key == OIS::KC_B)
						g_mapCffKeys["StrafeUp"] = true;

					if(key == OIS::KC_N)
						g_mapCffKeys["StrafeDown"] = true;
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
					unsigned long temp = message->getData<unsigned long>();
					float lasttime = temp / 1000.0f;

					if(g_mapCffKeys["MoveUp"])
						m_pInstanceCam->move(Ogre::Vector3(0.0f, 0.0f, -10.0f * lasttime));

					if(g_mapCffKeys["MoveDown"])
						m_pInstanceCam->move(Ogre::Vector3(0.0f, 0.0f, 10.0f * lasttime));
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
					unsigned long temp = message->getData<unsigned long>();
					float lasttime = temp / 1000.0f;

					if(g_mapCffKeys["RotateUp"])
						m_pInstanceCam->rotateAxisX(1.0f * lasttime);

					if(g_mapCffKeys["RotateDown"])
						m_pInstanceCam->rotateAxisX(-1.0f * lasttime);

					if(g_mapCffKeys["RotateLeft"])
						m_pInstanceCam->rotateAxisY(1.0f * lasttime);

					if(g_mapCffKeys["RotateRight"])
						m_pInstanceCam->rotateAxisY(-1.0f * lasttime);
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
					unsigned long temp = message->getData<unsigned long>();
					float lasttime = temp / 1000.0f;

					if(g_mapCffKeys["StrafeLeft"])
						m_pInstanceCam->strafe(10.0f * lasttime);

					if(g_mapCffKeys["StrafeRight"])
						m_pInstanceCam->strafe(-10.0f * lasttime);

					if(g_mapCffKeys["StrafeUp"])
						m_pInstanceCam->strafeUp(10.0f * lasttime);

					if(g_mapCffKeys["StrafeDown"])
						m_pInstanceCam->strafeUp(-10.0f * lasttime);
				}
		};

	};
};

#endif // __CAMERAFREEFLY_H__
