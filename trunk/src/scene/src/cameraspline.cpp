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
#include "../../dynamicobject/include/float.h"

using namespace Gnoll::Graphic;
using namespace Gnoll::DynamicObject;

namespace Gnoll
{
	namespace Scene
	{
		CameraSpline::CameraSpline(const Glib::ustring& instanceName) :
			Camera(instanceName)
		{
			shared_ptr<Float> len;
			shared_ptr<Float> default_length = shared_ptr<Float> (new Float(10.0f));
			len = this->getAttributeOrDefault<Float>("length", default_length);
			length = *len;

			// Get the scenemanager
			Ogre::SceneManager* pSM = CGraphicModule::getInstancePtr()->getSceneManager();

			// Create a scene node of the camera
			Ogre::SceneNode* camNode = pSM->getRootSceneNode()->createChildSceneNode();
			camNode->attachObject(getOgreCamera());

			// Create the animation with a SPLINE
			pAnim = pSM->createAnimation(instanceName, length);
			pAnim->setInterpolationMode(Ogre::Animation::IM_SPLINE);
			pNodeAT = pAnim->createNodeTrack(0, camNode);

			pAnimState = pSM->createAnimationState(instanceName);

			// Get back an ancient animation (if instance existed only)
			// TODO : need to speak with Paf

		}

		CameraSpline::~CameraSpline()
		{
		}

		void CameraSpline::update(float time)
		{
			if(pAnimState->getEnabled())
				pAnimState->addTime(time);
		}

		void CameraSpline::addPoint(const Ogre::Vector3& vec3, unsigned long frame)
		{
			if(frame > length)
				return;

			Ogre::TransformKeyFrame* key = pNodeAT->createNodeKeyFrame(frame);
			key->setTranslate(vec3);
			mapAnim[frame] = vec3;
		}

		const Ogre::Vector3* CameraSpline::getPoint(unsigned long frame)
		{
			if(frame > length)
				return NULL;

			return &mapAnim[frame];
		}

		void CameraSpline::setLength(unsigned long length)
		{
			length = length;
		}

		unsigned long CameraSpline::getLength()
		{
			return length;
		}

		void CameraSpline::start()
		{
			pAnimState->setEnabled(true);
		}

		void CameraSpline::stop()
		{
			pAnimState->setEnabled(false);
		}

		void CameraSpline::setLoop(bool loop)
		{
			pAnimState->setLoop(loop);
		}

		float CameraSpline::getCurrentKeyFrame()
		{
			return pAnimState->getTimePosition();
		}
	};
};
