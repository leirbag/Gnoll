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
|   This is a first person camera                                           |
|                                                                           |
|   Changelog :                                                             |
|               20/06/2007 - Gabriel - Initial release                      |
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

#ifndef __CAMERAFIRSTPERSON_H__
#define __CAMERAFIRSTPERSON_H__

namespace Gnoll
{

	namespace Core 
	{
		/**
		*	This is the interface of all Sources.</br>
		*   This class cannot be instanciable.
		*/ 
		class CameraFirstPerson : public Gnoll::Core::Camera
		{
		private:
			Ogre::SceneNode* m_pNode;
			float            m_fAmountDegreeX, m_fAmountDegreeY, m_fAmountDegreeZ;
			float            m_fLimitRotX, m_fLimitRotY, m_fLimitRotZ;

			shared_ptr<CMessageListener> m_listenerUpdate;

		public:
			/**
			 * This is the constructor.
			 * @param _instance This is the instance name; the Camera's name
			 */
			CameraFirstPerson(const Glib::ustring& instanceName, Ogre::SceneManager* pSM);

			/**
			 * This is the destructor
			 */
			virtual ~CameraFirstPerson()
			{
				CGenericMessageManager::getInstancePtr()->delListener ( m_listenerUpdate, CMessageType("GRAPHIC_FRAME_RENDERED") );
			}

			/**
			 * This update the View
			 * @param time This is the time between 2 frames
			 */
			virtual void update(float time)
			{
				if(m_pNode == NULL)
					return;

				m_ogreCamera->setPosition(m_pNode->getPosition());
				m_ogreCamera->setDirection(-m_pNode->getOrientation().zAxis());
			}

			/**
			 * This set a pointer to the target node
			 * @param pNode This is the target
			 */
			void setTarget(Ogre::SceneNode* pNode);

			/**
			 * This return a pointer to the target node
			 * @return node that target
			 */
			Ogre::SceneNode* getTarget();

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

		class UpdateCameraFirstPersonListener : public CMessageListener
		{
			private:
				Gnoll::Core::CameraFirstPerson* m_pInstanceCam;

			public:
				
				/**
				 * This is a constructor
				 */
				UpdateCameraFirstPersonListener(Gnoll::Core::CameraFirstPerson* pInstanceCam) 
				{
					m_pInstanceCam = pInstanceCam;
				}

				/**
				 * This is a destructor
				 */
				virtual ~UpdateCameraFirstPersonListener() 
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
	};
};

#endif // __CAMERAFIRSTPERSON_H__
