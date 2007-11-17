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


/*---------------------------CameraThirdPerson-----------------------------*\
|   This is a free fly camera                                               |
|                                                                           |
|   Changelog :                                                             |
|               10/06/2007 - Gabriel - Initial release                      |
|               19/06/2007 - Gabriel - Change all variables for listener    |
|                                       by a map.                           |
|                                      Add limitation of rotation           |
|                                      Add time to the transformation       |
|               10/30/2007 - Gabriel - add time to update()                 |
|               11/16/2007 - Paf - Remove all references to                 |
|                                   CGenericMessageManager                  |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include <OgreVector3.h>
#include "camera.h" 
#include "../../core/include/cmessagelistener.h"
#include "../../input/include/coisinputmodule.h"
#include "../../input/include/cinputmouseevents.h"

#include "../../core/include/cmessage.h"
#include "../../core/include/cmessagetype.h"
#include "../../core/include/cmessagemodule.h"

#ifndef __CAMERATHIRDPERSON_H__
#define __CAMERATHIRDPERSON_H__

namespace Gnoll
{

	namespace Core 
	{
		/**
		*	This is the interface of all Sources.</br>
		*   This class cannot be instanciable.
		*/ 
		class CameraThirdPerson : public Gnoll::Core::Camera
		{
		private:
			Ogre::SceneNode* m_pNode;
			float            m_fOffset;
			float            m_fAmountDegreeX, m_fAmountDegreeY, m_fAmountDegreeZ;
			float            m_fLimitRotX, m_fLimitRotY, m_fLimitRotZ;

			shared_ptr<CMessageListener> m_listenerMove;
			shared_ptr<CMessageListener> m_listenerRotate;
			shared_ptr<CMessageListener> m_listenerMouseRotate;

			shared_ptr<CMessageListener> m_listenerKeyUp;
			shared_ptr<CMessageListener> m_listenerKeyDown;

		public:

			/**
			 * This is the constructor.
			 * @param _instance This is the instance name; the Camera's name
			 */
			CameraThirdPerson(const Glib::ustring& instanceName, Ogre::SceneManager* pSM);

			/**
			 * This is the destructor
			 */
			virtual ~CameraThirdPerson()
			{
				CMessageManager* messageManager = CMessageModule::getInstancePtr()->getMessageManager();

				messageManager->delListener ( m_listenerMove, CMessageType("GRAPHIC_FRAME_RENDERED") );
				messageManager->delListener ( m_listenerRotate, CMessageType("GRAPHIC_FRAME_RENDERED") );
				messageManager->delListener ( m_listenerKeyUp, CMessageType("KEYBOARD_KEYUP") );
				messageManager->delListener ( m_listenerKeyDown, CMessageType("KEYBOARD_KEYDOWN") );
				messageManager->delListener ( m_listenerMouseRotate, CMessageType("MOUSE_MOVED") );
			}

			/**
			 * This update the View.
			 */
			virtual void update(float time)
			{
				if(m_pNode == NULL)
					return;

				m_ogreCamera->setPosition(m_pNode->getPosition());
				m_ogreCamera->moveRelative(m_pNode->getOrientation().zAxis() * m_fOffset);
			}

			/**
			 * This set a pointer to the target node
			 * @param pNode This is the target
			 */
			void setTarget(Ogre::SceneNode* pNode);

			/**
			 * This return a pointer to the target node
			 * @return node
			 */
			Ogre::SceneNode* getTarget();

			/**
			 * This set the offset between camera and target
			 * @param offset This is the offset between target and camera
			 */
			void setOffset(float offset);

			/**
			 * This return the offset between camera and target
			 * @return offset (default 30)
			 */
			float getOffset();

			/**
			 * This set the rotation limit of camera around axis X
			 * @param angle limit in degree
			 */
			void setLimitRotationX(float angle);

			/**
			 * This set the rotation limit of camera around axis Y
			 * @param angle limit in degree
			 */
			void setLimitRotationY(float angle);

			/**
			 * This set the rotation limit of camera around axis Z
			 * @param angle limit in degree
			 */
			void setLimitRotationZ(float angle);

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
				
		};

		// Map for the key listener
		static std::map<std::string, bool> g_mapCtpKeys;

		class CtpKeyUp : public CMessageListener
		{
			public:
				
				/**
				 * This is a constructor
				 */
				CtpKeyUp() 
				{
				}

				/**
				 * This is a destructor
				 */
				virtual ~CtpKeyUp() 
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
						g_mapCtpKeys["MoveUp"] = false;

					if(key == OIS::KC_DOWN)
						g_mapCtpKeys["MoveDown"] = false;

					if(key == OIS::KC_E)
						g_mapCtpKeys["RotateUp"] = false;

					if(key == OIS::KC_D)
						g_mapCtpKeys["RotateDown"] = false;

					if(key == OIS::KC_S)
						g_mapCtpKeys["RotateLeft"] = false;

					if(key == OIS::KC_F)
						g_mapCtpKeys["RotateRight"] = false;
				}
		};

		class CtpKeyDown : public CMessageListener
		{
			public:
				
				/**
				 * This is a constructor
				 */
				CtpKeyDown() 
				{
				}

				/**
				 * This is a destructor
				 */
				virtual ~CtpKeyDown() 
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
						g_mapCtpKeys["MoveUp"] = true;

					if(key == OIS::KC_DOWN)
						g_mapCtpKeys["MoveDown"] = true;

					if(key == OIS::KC_E)
						g_mapCtpKeys["RotateUp"] = true;

					if(key == OIS::KC_D)
						g_mapCtpKeys["RotateDown"] = true;

					if(key == OIS::KC_S)
						g_mapCtpKeys["RotateLeft"] = true;

					if(key == OIS::KC_F)
						g_mapCtpKeys["RotateRight"] = true;
				}
		};

		class MoveCameraThirdPersonListener : public CMessageListener
		{
			private:
				Gnoll::Core::CameraThirdPerson* m_pInstanceCam;

			public:
				
				/**
				 * This is a constructor
				 */
				MoveCameraThirdPersonListener(Gnoll::Core::CameraThirdPerson* pInstanceCam) 
				{
					m_pInstanceCam = pInstanceCam;
				}

				/**
				 * This is a destructor
				 */
				virtual ~MoveCameraThirdPersonListener() 
				{
					delete m_pInstanceCam;
				}

				/**
				 * This method is called in order to process a message
				 * @param message The message this method will have to process
				 */
				virtual void handle ( shared_ptr<CMessage> message ) 
				{ 
					m_pInstanceCam->update(0);
				}
		};

		class RotateCameraThirdPersonListener : public CMessageListener
		{
			private:
				Gnoll::Core::CameraThirdPerson* m_pInstanceCam;

			public:
				
				/**
				 * This is a constructor
				 */
				RotateCameraThirdPersonListener(Gnoll::Core::CameraThirdPerson* pInstanceCam) 
				{
					m_pInstanceCam = pInstanceCam;
				}

				/**
				 * This is a destructor
				 */
				virtual ~RotateCameraThirdPersonListener() 
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

					if(g_mapCtpKeys["RotateLeft"])
						m_pInstanceCam->rotateAxisY(1.0f * lasttime);

					if(g_mapCtpKeys["RotateRight"])
						m_pInstanceCam->rotateAxisY(-1.0f * lasttime);

					if(g_mapCtpKeys["RotateUp"])
						m_pInstanceCam->rotateAxisX(1.0f * lasttime);

					if(g_mapCtpKeys["RotateDown"])
						m_pInstanceCam->rotateAxisX(-1.0f * lasttime);
				}
		};

		class MouseRotateCameraThirdPersonListener : public CMessageListener
		{
			private:
				Gnoll::Core::CameraThirdPerson* m_pInstanceCam;

			public:
				
				/**
				 * This is a constructor
				 */
				MouseRotateCameraThirdPersonListener(Gnoll::Core::CameraThirdPerson* pInstanceCam) 
				{
					m_pInstanceCam = pInstanceCam;
				}

				/**
				 * This is a destructor
				 */
				virtual ~MouseRotateCameraThirdPersonListener() 
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
	};
};

#endif // __CAMERATHIRDPERSON_H__
