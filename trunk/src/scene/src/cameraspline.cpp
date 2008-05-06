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


/*-------------------------------cameraspline------------------------------*\
|   This is a spline camera                                                 |
|                                                                           |
|   Changelog :                                                             |
|               04/12/2008 - Gabriel - Initial release                      |
\*-------------------------------------------------------------------------*/

#include "../include/cameraspline.h"
#include "../../graphic/include/cgraphicmodule.h"

using namespace Gnoll::Graphic;

namespace Gnoll
{
	namespace Scene
	{
		cameraspline_i
		{
			unsigned long length;
			Ogre::Animation* pAnim;
			Ogre::NodeAnimationTrack* pNodeAT;
			Ogre::AnimationState* pAnimState;
			std::map<unsigned long, Ogre::Vector3> mapAnim;

			cameraspline_i() :
				length(0),
				pAnim(NULL),
				pNodeAT(NULL),
				pAnimState(NULL)
			{
			}
		};

		CameraSpline::CameraSpline(const Glib::ustring& instanceName) :
			Camera(instanceName),
			m_this(new cameraspline_i)
		{
			// TODO: Serialize
			Ogre::SceneManager* pSM = CGraphicModule::getInstancePtr()->getSceneManager();
			Ogre::SceneNode* camNode = pSM->getRootSceneNode()->createChildSceneNode();
			camNode->attachObject(m_ogreCamera);

			m_lenght  = 10;
			m_pAnim = pSM->createAnimation(instanceName, 10);
			m_pAnim->setInterpolationMode(Ogre::Animation::IM_SPLINE);
			m_pNodeAT = m_pAnim->createNodeTrack(0, camNode);

			m_pAnimState = pSM->createAnimationState(instanceName);
		}

		CameraSpline::CameraSpline(const Camera& copy)
		{
			// Copy attributs
			m_this = new camera_i;
			*m_this = *(copy.m_this);
		}

		CameraSpline::CameraSpline& operator=(const CameraSpline& copy)
		{
			// Copy attributs
			*m_this = *(copy.m_this);
			return *this;
		}

		CameraSpline::~CameraSpline()
		{
			delete m_this;
		}

		void CameraSpline::update(float time)
		{
			if(m_this->pAnimState->getEnabled())
				m_this->pAnimState->addTime(time);
		}

		void CameraSpline::addPoint(const Ogre::Vector3& vec3, unsigned long frame)
		{
			if(frame > m_this->length)
				return;

			Ogre::TransformKeyFrame* key = m_this->pNodeAT->createNodeKeyFrame(frame);
			key->setTranslate(vec3);
			m_this->mapAnim[frame] = vec3;
		}

		const Ogre::Vector3* CameraSpline::getPoint(unsigned long frame)
		{
			if(frame > m_this->length)
				return NULL;

			return &m_this->mapAnim[frame];
		}

		void CameraSpline::setLength(unsigned long length)
		{
			m_this->length = length;
		}

		unsigned long CameraSpline::getLength()
		{
			return m_this->length;
		}

		void CameraSpline::start()
		{
			m_this->pAnimState->setEnabled(true);
		}

		void CameraSpline::stop()
		{
			m_this->pAnimState->setEnabled(false);
		}

		void CameraSpline::setLoop(bool loop)
		{
			m_this->pAnimState->setLoop(loop);
		}

		float CameraSpline::getCurrentKeyFrame()
		{
			return m_this->pAnimState->getTimePosition();
		}
	};
};
