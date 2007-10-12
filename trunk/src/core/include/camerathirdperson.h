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
|               10/06/2007 - Gabriel - Initial release                      |
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
				CGenericMessageManager::getInstancePtr()->delListener ( m_listenerMove, CMessageType("GRAPHIC_FRAME_RENDERED") );
				CGenericMessageManager::getInstancePtr()->delListener ( m_listenerRotate, CMessageType("GRAPHIC_FRAME_RENDERED") );
				CGenericMessageManager::getInstancePtr()->delListener ( m_listenerKeyUp, CMessageType("KEYBOARD_KEYUP") );
				CGenericMessageManager::getInstancePtr()->delListener ( m_listenerKeyDown, CMessageType("KEYBOARD_KEYDOWN") );
				CGenericMessageManager::getInstancePtr()->delListener ( m_listenerMouseRotate, CMessageType("MOUSE_MOVED") );
			}

			/**
			 * This update the View.
			 */
			virtual void update()
			{
				if(m_pNode == NULL)
					return;

				m_ogreCamera->setPosition(m_pNode->getPosition());
				m_ogreCamera->moveRelative(m_pNode->getOrientation().zAxis() * m_fOffset);
			}

			/**
			 * This set a pointer to the target node
			 * @param node to target
			 */
			void setTarget(Ogre::SceneNode* pNode);

			/**
			 * This return a pointer to the target node
			 * @return node
			 */
			Ogre::SceneNode* getTarget();

			/**
			 * This set the offset between camera and target
			 * @param offset
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
				
		};

		// Will be change by a std::map later ... maybe ...
		static bool ctpKeyMoveUp   = false;
		static bool ctpKeyMoveDown = false;

		static bool ctpKeyRotateUp    = false;
		static bool ctpKeyRotateDown  = false;
		static bool ctpKeyRotateLeft  = false;
		static bool ctpKeyRotateRight = false;

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
						ctpKeyMoveUp = false;

					if(key == OIS::KC_DOWN)
						ctpKeyMoveDown = false;

					if(key == OIS::KC_E)
						ctpKeyRotateUp = false;

					if(key == OIS::KC_D)
						ctpKeyRotateDown = false;

					if(key == OIS::KC_S)
						ctpKeyRotateLeft = false;

					if(key == OIS::KC_F)
						ctpKeyRotateRight = false;
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
						ctpKeyMoveUp = true;

					if(key == OIS::KC_DOWN)
						ctpKeyMoveDown = true;

					if(key == OIS::KC_E)
						ctpKeyRotateUp = true;

					if(key == OIS::KC_D)
						ctpKeyRotateDown = true;

					if(key == OIS::KC_S)
						ctpKeyRotateLeft = true;

					if(key == OIS::KC_F)
						ctpKeyRotateRight = true;
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
					m_pInstanceCam->update();
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
					if(ctpKeyRotateLeft)
						m_pInstanceCam->rotateAxisY(0.02);

					if(ctpKeyRotateRight)
						m_pInstanceCam->rotateAxisY(-0.02);

					if(ctpKeyRotateUp)
						m_pInstanceCam->rotateAxisX(0.02);

					if(ctpKeyRotateDown)
						m_pInstanceCam->rotateAxisX(-0.02);
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
