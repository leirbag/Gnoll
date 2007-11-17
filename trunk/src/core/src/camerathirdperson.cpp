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


/*----------------------------CameraThirdPerson----------------------------*\
|   This is a free fly camera                                               |
|                                                                           |
|   Changelog :                                                             |
|               10/06/2007 - Gabriel - Initial release                      |
|               19/06/2007 - Gabriel - Change all variables for listener    |
|                                       by a map.                           |
|                                      Add limitation of rotation           |
|                                      Add time to the transformation       |
|               11/16/2007 - Paf - Remove all references to                 |
|                                   CGenericMessageManager                  |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include "../include/camerathirdperson.h"

namespace Gnoll
{

	namespace Core 
	{

		CameraThirdPerson::CameraThirdPerson(const Glib::ustring& instanceName, Ogre::SceneManager* pSM) : Gnoll::Core::Camera(instanceName, pSM)
		{
			// Initialize attributs
			m_pNode = NULL;
			m_fOffset = 30.0f; 
			m_fAmountDegreeX = m_fAmountDegreeY = m_fAmountDegreeZ = 0.0f;
			m_fLimitRotX     = m_fLimitRotY     = m_fLimitRotZ     = 0.0f;

			// Add listener
			CMessageManager* messageManager = CMessageModule::getInstancePtr()->getMessageManager();
			
			m_listenerKeyUp = shared_ptr<CMessageListener>(new Gnoll::Core::CtpKeyUp);
			messageManager->addListener ( m_listenerKeyUp, CMessageType("KEYBOARD_KEYUP") );

			m_listenerKeyDown = shared_ptr<CMessageListener>(new Gnoll::Core::CtpKeyDown);
			messageManager->addListener ( m_listenerKeyDown, CMessageType("KEYBOARD_KEYDOWN") );


			m_listenerMove = shared_ptr<CMessageListener>(new Gnoll::Core::MoveCameraThirdPersonListener(static_cast<Gnoll::Core::CameraThirdPerson*>(this)));
			messageManager->addListener ( m_listenerMove, CMessageType("GRAPHIC_FRAME_RENDERED") );

			m_listenerRotate = shared_ptr<CMessageListener>(new Gnoll::Core::RotateCameraThirdPersonListener(static_cast<Gnoll::Core::CameraThirdPerson*>(this)));
			messageManager->addListener ( m_listenerRotate, CMessageType("GRAPHIC_FRAME_RENDERED") );

			m_listenerMouseRotate = shared_ptr<CMessageListener>(new Gnoll::Core::MouseRotateCameraThirdPersonListener(static_cast<Gnoll::Core::CameraThirdPerson*>(this)));
			messageManager->addListener ( m_listenerMouseRotate, CMessageType("MOUSE_MOVED") );


			// Creation of controller
			g_mapCtpKeys["MoveUp"]         = false;
			g_mapCtpKeys["ctpKeyMoveDown"] = false;

			g_mapCtpKeys["RotateUp"]       = false;
			g_mapCtpKeys["RotateDown"]     = false;
			g_mapCtpKeys["RotateLeft"]     = false;
			g_mapCtpKeys["RotateRight"]    = false;
		}

		void CameraThirdPerson::setTarget(Ogre::SceneNode* pNode)
		{
			m_pNode = pNode;
			if(m_pNode == NULL)
				return;

			m_ogreCamera->setOrientation (m_pNode->getWorldOrientation());
			update(0);
		}

		Ogre::SceneNode* CameraThirdPerson::getTarget()
		{
			return m_pNode;
		}

		void CameraThirdPerson::setOffset(float offset)
		{
			m_fOffset = offset;
			update(0);
		}

		float CameraThirdPerson::getOffset()
		{
			return m_fOffset;
		}

		void CameraThirdPerson::setLimitRotationX(float angle)
		{
			m_fLimitRotX = angle;
		}

		void CameraThirdPerson::setLimitRotationY(float angle)
		{
			m_fLimitRotY = angle;
		}

		void CameraThirdPerson::setLimitRotationZ(float angle)
		{
			m_fLimitRotZ = angle;
		}

		void CameraThirdPerson::rotateAxisX(float angle)
		{
			if(fabs(m_fAmountDegreeX - Ogre::Radian(angle).valueDegrees()) > m_fLimitRotX)
				return;

			m_fAmountDegreeX -= Ogre::Radian(angle).valueDegrees();
			m_ogreCamera->pitch(-Ogre::Radian(angle));
			update(0);
		}

		void CameraThirdPerson::rotateAxisY(float angle)
		{
			if(fabs(m_fAmountDegreeY - Ogre::Radian(angle).valueDegrees()) > m_fLimitRotY)
				return;

			m_fAmountDegreeY -= Ogre::Radian(angle).valueDegrees();
			m_ogreCamera->yaw(-Ogre::Radian(angle));
			update(0);
		}

		void CameraThirdPerson::rotateAxisZ(float angle)
		{
			if(fabs(m_fAmountDegreeZ - Ogre::Radian(angle).valueDegrees()) > m_fLimitRotZ)
				return;

			m_fAmountDegreeZ -= Ogre::Radian(angle).valueDegrees();
			m_ogreCamera->roll(-Ogre::Radian(angle));
			update(0);
		}
	};
};


