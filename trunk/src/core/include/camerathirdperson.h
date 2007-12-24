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
#include "camerathirdpersonlistener.h"

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
			virtual ~CameraThirdPerson();

			/**
			 * This update the View.
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
	};
};

#endif // __CAMERATHIRDPERSON_H__
