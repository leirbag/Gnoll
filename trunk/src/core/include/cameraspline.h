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
|   This is a spline camera                                                 |
|                                                                           |
|   Changelog :                                                             |
|               27/06/2007 - Gabriel - Initial release                      |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include <OgreVector3.h>
#include <OgreSimpleSpline.h>
#include <OgreAnimation.h>
#include <OgreAnimationState.h> 
#include "camera.h" 
#include "../../core/include/cmessagelistener.h"
#include "../../input/include/coisinputmodule.h"
#include "../../input/include/cinputmouseevents.h"

#include "../../core/include/cmessage.h"
#include "../../core/include/cmessagetype.h"
#include "../../core/include/cmessagemanager.h"
#include "../../core/include/cgenericmessagemanager.h"

#ifndef __CAMERASPLINE_H__
#define __CAMERASPLINE_H__

namespace Gnoll
{

	namespace Core 
	{

		class UpdateCameraSplineListener;
		/**
		*	This is the interface of all Sources.</br>
		*   This class cannot be instanciable.
		*/ 
		class CameraSpline : public Gnoll::Core::Camera
		{
		private:
			unsigned long m_lenght;
			Ogre::SceneNode* m_pTarget;
			Ogre::Animation* m_pAnim;
			Ogre::NodeAnimationTrack* m_pNodeAT;
			Ogre::AnimationState* m_pAnimState;
			std::map<unsigned long, Ogre::Vector3> m_mapAnim;
			shared_ptr<CMessageListener> m_listenerUpdate;

		public:

			/**
			 * This is the constructor.
			 * @param _instance This is the instance name; the Camera's name
			 */
			CameraSpline(const Glib::ustring& instanceName, Ogre::SceneManager* pSM);

			/**
			 * This is the destructor
			 */
			virtual ~CameraSpline()
			{
			}

			/**
			 * This update the View.
			 */
			virtual void update(float time)
			{
				if(m_pAnimState->getEnabled())
					m_pAnimState->addTime(time);
			}

			/**
			 * This set a pointer to the target node
			 * @param node to target
			 */
			void setTarget(Ogre::SceneNode* pNode)
			{
				if(pNode != NULL)
					m_pTarget = pNode;
				else
					return;

				m_ogreCamera->setAutoTracking(true, pNode);
			}

			/**
			 * This return a pointer to the target node
			 * @return node
			 */
			Ogre::SceneNode* getTarget()
			{
				return m_pTarget;
			}

			/**
			 * This add a point at a frame of the animation
			 * @param vec3 position of the camera
			 * @param frame specify the frame number
			 */
			void addPoint(const Ogre::Vector3& vec3, unsigned long frame);

			/**
			 * This return the position of camera at a frame number
			 * @param frame specify the frame number
			 * @return position of the camera
			 */
			const Ogre::Vector3* getPoint(unsigned long frame);

			/**
			 * This specify the lenght (in key frame) of the animation
			 * @param lenght number of key frame
			 */
			void setLenght(unsigned long lenght)
			{
				m_lenght = lenght;
			}

			/**
			 * This return the number of key frame
			 * @return number of key frame
			 */
			unsigned long getLenght()
			{
				return m_lenght;
			}

			/**
			 * This start the animation of camera
			 */
			void start()
			{
				m_pAnimState->setEnabled(true);
			}

			/**
			 * This stop the animation of camera
			 */
			void stop()
			{
				m_pAnimState->setEnabled(false);
			}

			/**
			 * This set the status "loop" or not to the animation
			 * @param loop specify if loop or not
			 */
			void setLoop(bool loop)
			{
				m_pAnimState->setLoop(loop);
			}

			/**
			 * This return the current key frame
			 * @return the current key frame
			 */
			float getCurrentKeyFrame()
			{
				return m_pAnimState->getTimePosition();
			}
		};

		class UpdateCameraSplineListener : public CMessageListener
		{
			private:
				Gnoll::Core::CameraSpline* m_pInstanceCam;

			public:
				
				/**
				 * This is a constructor
				 */
				UpdateCameraSplineListener(Gnoll::Core::CameraSpline* pInstanceCam) 
				{
					m_pInstanceCam = pInstanceCam;
				}

				/**
				 * This is a destructor
				 */
				virtual ~UpdateCameraSplineListener() 
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

					m_pInstanceCam->update(lasttime);
				}
		};
	};
};

#endif // __CAMERASPLINE_H__
