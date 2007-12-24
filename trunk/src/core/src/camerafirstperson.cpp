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
|               10/06/2007 - Gabriel - Initial release                      |
|               12/24/2007 - Gabriel - delete scenemanager from Ctor        |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include "../include/camerafirstperson.h"
#include "../include/cmessagemodule.h"

namespace Gnoll
{

	namespace Core 
	{
		CameraFirstPerson::CameraFirstPerson(const Glib::ustring& instanceName) : Gnoll::Core::Camera(instanceName)
		{
			// Initialize attributs
			m_pTarget = NULL;
			m_fAmountDegreeX = m_fAmountDegreeY = m_fAmountDegreeZ = 0.0f;
			m_fLimitRotX     = m_fLimitRotY     = m_fLimitRotZ     = 0.0f;

			CMessageModule* messageModule = CMessageModule::getInstancePtr(); 
			m_listenerUpdate = shared_ptr<CMessageListener>(new Gnoll::Core::UpdateCameraFirstPersonListener(static_cast<Gnoll::Core::CameraFirstPerson*>(this)));
			messageModule->getMessageManager()->addListener ( m_listenerUpdate, CMessageType("GRAPHIC_FRAME_RENDERED") );
		}

		CameraFirstPerson::~CameraFirstPerson()
		{
			CMessageModule* messageModule = CMessageModule::getInstancePtr(); 
			messageModule->getMessageManager()->delListener ( m_listenerUpdate, CMessageType("GRAPHIC_FRAME_RENDERED") );
		}

		void CameraFirstPerson::update(float time)
		{
			if(m_pTarget == NULL)
				return;

			m_ogreCamera->setPosition(m_pTarget->getPosition());
			m_ogreCamera->setDirection(-m_pTarget->getOrientation().zAxis());
		}

		void CameraFirstPerson::setTarget(Ogre::SceneNode* pNode)
		{
			m_pTarget = pNode;
			if(m_pTarget == NULL)
				return;

			m_ogreCamera->setOrientation(m_pTarget->getWorldOrientation());
			update(0);
		}

		Ogre::SceneNode* CameraFirstPerson::getTarget()
		{
			return m_pTarget;
		}

		void CameraFirstPerson::setLimitRotationX(float angle)
		{
			m_fLimitRotX = angle;
		}

		void CameraFirstPerson::setLimitRotationY(float angle)
		{
			m_fLimitRotY = angle;
		}

		void CameraFirstPerson::setLimitRotationZ(float angle)
		{
			m_fLimitRotZ = angle;
		}

		void CameraFirstPerson::rotateAxisX(float angle)
		{
			m_fAmountDegreeX = m_ogreCamera->getOrientation().getPitch().valueDegrees();
			if(fabs(m_fAmountDegreeX - Ogre::Radian(angle).valueDegrees()) > m_fLimitRotX)
				return;

			m_fAmountDegreeX -= Ogre::Radian(angle).valueDegrees();
			m_ogreCamera->pitch(-Ogre::Radian(angle));
			update(0);
		}

		void CameraFirstPerson::rotateAxisY(float angle)
		{
			m_fAmountDegreeY = m_ogreCamera->getOrientation().getYaw().valueDegrees();
			if(fabs(m_fAmountDegreeY - Ogre::Radian(angle).valueDegrees()) > m_fLimitRotY)
				return;

			m_fAmountDegreeY -= Ogre::Radian(angle).valueDegrees();
			m_ogreCamera->yaw(-Ogre::Radian(angle));
			update(0);
		}

		void CameraFirstPerson::rotateAxisZ(float angle)
		{
			if(fabs(m_fAmountDegreeZ - Ogre::Radian(angle).valueDegrees()) > m_fLimitRotZ)
				return;

			m_fAmountDegreeZ -= Ogre::Radian(angle).valueDegrees();
			m_ogreCamera->roll(-Ogre::Radian(angle));
			update(0);
		}
	};
};