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


/*----------------------------CameraFreeFly--------------------------------*\
|   This is a free fly camera                                               |
|                                                                           |
|   Changelog :                                                             |
|               08/31/2007 - Gabriel - Initial release                      |
|               06/10/2007 - Gabriel - Add hack to enable key repeat        |
|                                    - Add management of mouse for the      |
|									   rotation                                      |
|               10/10/2007 - Gabriel - Change all variables for listener    |
|                                      by a map.                            |
|									   Add time to the transformation                |
|               10/30/2007 - Gabriel - add time to update()                 |
|               11/16/2007 - Paf - Remove all references to                 |
|                                   CGenericMessageManager                  |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include <OgreVector3.h>
#include "camera.h" 
#include "../../core/include/cmessagelistener.h"
#include "camerafreeflylistener.h"

#ifndef __CAMERAFREEFLY_H__
#define __CAMERAFREEFLY_H__

namespace Gnoll
{

	namespace Core 
	{
		/**
		*	This is the interface of all Sources.</br>
		*   This class cannot be instanciable.
		*/ 
		class CameraFreeFly : public Gnoll::Core::Camera
		{
		private:
			/**
			* These are the listeners
			*/
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
			virtual ~CameraFreeFly();

			/**
			 * This update the View.
			 */
			virtual void update(float time) {}

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
	};
};

#endif // __CAMERAFREEFLY_H__
