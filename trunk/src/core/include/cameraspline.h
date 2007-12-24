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
|               12/24/2007 - Gabriel - delete scenemanager from Ctor        |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include <OgreVector3.h>
#include <OgreSimpleSpline.h>
#include <OgreAnimation.h>
#include <OgreAnimationState.h> 
#include "camera.h" 
#include "camerasplinelistener.h"

#ifndef __CAMERASPLINE_H__
#define __CAMERASPLINE_H__

namespace Gnoll
{

	namespace Core 
	{
		/**
		*	This is the interface of all Sources.</br>
		*   This class cannot be instanciable.
		*/ 
		class CameraSpline : public Gnoll::Core::Camera
		{
		private:
			/**
			* This is the lenght (in key) of the animation
			*/
			unsigned long m_lenght;

			/**
			* This is the target (NULL if no target)
			*/
			Ogre::SceneNode* m_pTarget;

			/**
			* This is a pointer to the animation
			*/
			Ogre::Animation* m_pAnim;

			/**
			* This is a pointer to the animation track
			*/
			Ogre::NodeAnimationTrack* m_pNodeAT;

			/**
			* This is a pointer to the animation state
			*/
			Ogre::AnimationState* m_pAnimState;

			/**
			* This is the storage of position of all key
			*/
			std::map<unsigned long, Ogre::Vector3> m_mapAnim;

			/**
			* This is the listener
			*/
			shared_ptr<CMessageListener> m_listenerUpdate;

		public:

			/**
			 * This is the constructor.
			 * @param _instance This is the instance name; the Camera's name
			 */
			CameraSpline(const Glib::ustring& instanceName);

			/**
			 * This is the destructor
			 */
			virtual ~CameraSpline() {}

			/**
			 * This update the View.
			 * @param time This is the time between 2 frames
			 */
			virtual void update(float time);

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
			void setLenght(unsigned long lenght);

			/**
			 * This return the number of key frame
			 * @return number of key frame
			 */
			unsigned long getLenght();

			/**
			 * This start the animation of camera
			 */
			void start();

			/**
			 * This stop the animation of camera
			 */
			void stop();

			/**
			 * This set the status "loop" or not to the animation
			 * @param loop specify if loop or not
			 */
			void setLoop(bool loop);

			/**
			 * This return the current key frame
			 * @return the current key frame
			 */
			float getCurrentKeyFrame();
		};
	};
};

#endif // __CAMERASPLINE_H__
