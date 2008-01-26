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

#include "../include/cameraspline.h"
#include "../../graphic/include/cgraphicmodule.h"

namespace Gnoll
{

	namespace Core 
	{
		CameraSpline::CameraSpline(const Glib::ustring& instanceName) : Camera(instanceName)
		{
			Ogre::SceneManager* pSM = CGraphicModule::getInstancePtr()->getSceneManager();
			Ogre::SceneNode* camNode = pSM->getRootSceneNode()->createChildSceneNode();
			camNode->attachObject(m_ogreCamera);

			m_lenght  = 0;
			m_pTarget = NULL;
			m_pAnim = pSM->createAnimation(instanceName, 10);
			m_pAnim->setInterpolationMode(Ogre::Animation::IM_SPLINE);
			m_pNodeAT = m_pAnim->createNodeTrack(0, camNode);

			m_pAnimState = pSM->createAnimationState(instanceName);

			m_listenerUpdate = shared_ptr<CMessageListener>(new Gnoll::Core::UpdateCameraSplineListener(this));
			CMessageModule::getInstancePtr()->getMessageManager()->addListener ( m_listenerUpdate, CMessageType("GRAPHIC_FRAME_RENDERED") );
		}

		void CameraSpline::addPoint(const Ogre::Vector3& vec3, unsigned long frame)
		{
			if(frame > m_lenght)
				return;

			Ogre::TransformKeyFrame* key = m_pNodeAT->createNodeKeyFrame(frame);
			key->setTranslate(vec3);
			m_mapAnim[frame] = vec3;
		}

		const Ogre::Vector3* CameraSpline::getPoint(unsigned long frame)
		{
			if(frame > m_lenght)
				return NULL;

			return &m_mapAnim[frame];
		}

		void CameraSpline::update(float time)
		{
			if(m_pAnimState->getEnabled())
					m_pAnimState->addTime(time);
		}

		void CameraSpline::setTarget(Ogre::SceneNode* pNode)
		{
			if(pNode != NULL)
					m_pTarget = pNode;
				else
					return;

				m_ogreCamera->setAutoTracking(true, pNode);
		}

		Ogre::SceneNode* CameraSpline::getTarget()
		{
			return m_pTarget;
		}

		void CameraSpline::setLenght(unsigned long lenght)
		{
			m_lenght = lenght;
		}

		unsigned long CameraSpline::getLenght()
		{
			return m_lenght;
		}

		void CameraSpline::start()
		{
			m_pAnimState->setEnabled(true);
		}

		void CameraSpline::stop()
		{
			m_pAnimState->setEnabled(false);
		}

		void CameraSpline::setLoop(bool loop)
		{
			m_pAnimState->setLoop(loop);
		}

		float CameraSpline::getCurrentKeyFrame()
		{
			return m_pAnimState->getTimePosition();;
		}
	}
}


