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
#include "../../dynamicobject/include/dynamicobject.h"
#include <boost/lexical_cast.hpp>

using namespace Gnoll::Graphic;
using namespace Gnoll::DynamicObject;

namespace Gnoll
{
	namespace Scene
	{
		/*
		 * PRIVATE STRUCTURE
		 */
		CameraSpline::Spline::Spline(const Glib::ustring& instanceName, Ogre::NodeAnimationTrack* nodeAT, unsigned long len) :
			Gnoll::DynamicObject::CDynamicObjectProxy(instanceName)
		{
			// Set the max length
			// ------------------
			length = len;

			// Set the Node Animation Track
			// ----------------------------
			pNodeAT = nodeAT;

			// Extract each point of each Key Frame
			// ------------------------------------
			for(Gnoll::DynamicObject::DynamicObject::mapAttributes::const_iterator it = begin(); it != end(); it++)
			{
				addPoint(*dynamic_pointer_cast<Ogre::Vector3>(it->second), lexical_cast<unsigned long>(it->first));
			}
		}

		CameraSpline::Spline::~Spline()
		{
			// Save each point of each Key Frame
			// ---------------------------------
			std::map<unsigned long, Ogre::Vector3>::iterator it = mapAnim.begin();
			while(it != mapAnim.end())
			{
				setAttribute(lexical_cast<string>(it->first), shared_ptr<Gnoll::DynamicObject::Vector3>(
							new Gnoll::DynamicObject::Vector3(it->second)));
				it++;
			}
		}

		void CameraSpline::Spline::addPoint(const Ogre::Vector3& vec3, unsigned long frame)
		{
			// Check if we dont want to add a point at an invalidate key frame
			// ---------------------------------------------------------------
			if(frame > length)
				return;

			Ogre::TransformKeyFrame* key = pNodeAT->createNodeKeyFrame(frame);
			key->setTranslate(vec3);
			mapAnim[frame] = vec3;
		}

		/*
		 * CLASS
		 */
		CameraSpline::CameraSpline(const Glib::ustring& instanceName) :
			Camera(instanceName)
		{
			// Extract the length of the animation
			// -----------------------------------
			shared_ptr<Float> len;
			shared_ptr<Float> default_length = shared_ptr<Float> (new Float(30.0f));
			len = this->getAttributeOrDefault<Float>("length", default_length);

			// Get the scenemanager
			// --------------------
			Ogre::SceneManager* pSM = CGraphicModule::getInstancePtr()->getSceneManager();

			// Create a scene node of the camera
			// ---------------------------------
			Ogre::SceneNode* camNode = pSM->getRootSceneNode()->createChildSceneNode();
			camNode->attachObject(getOgreCamera());

			// Create the animation with a SPLINE
			// ----------------------------------
			pAnim = pSM->createAnimation(instanceName, *len);
			pAnim->setInterpolationMode(Ogre::Animation::IM_SPLINE);
			pNodeAT = pAnim->createNodeTrack(0, camNode);

			pAnimState = pSM->createAnimationState(instanceName);

			// Extract the animation
			// ---------------------
			shared_ptr<Gnoll::DynamicObject::String> default_name_spline = shared_ptr<Gnoll::DynamicObject::String> (
					new Gnoll::DynamicObject::String(getInstance() + ".spline"));
			shared_ptr<Gnoll::DynamicObject::String> name_spline = this->getAttributeOrDefault<Gnoll::DynamicObject::String>("Spline", default_name_spline);
			spline = shared_ptr<Spline>(new Spline(*name_spline, pNodeAT, *len));
		}

		CameraSpline::~CameraSpline()
		{
			// Save the length
			// ---------------
			this->setAttribute("length", shared_ptr<Float>(new Float(spline->length)));

			// Save the Spline
			// ---------------
			setAttribute("Spline", shared_ptr<Gnoll::DynamicObject::String>(new Gnoll::DynamicObject::String(getInstance() + ".spline")));
		}

		void CameraSpline::update(float time)
		{
			if(pAnimState->getEnabled())
				pAnimState->addTime(time);
		}

		void CameraSpline::addPoint(const Ogre::Vector3& vec3, unsigned long frame)
		{
			if(frame > spline->length)
				return;

			spline->addPoint(vec3, frame);
		}

		void CameraSpline::setLength(unsigned long length)
		{
			spline->length = length;
		}

		unsigned long CameraSpline::getLength()
		{
			return spline->length;
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

		const Ogre::Vector3* CameraSpline::getPoint(unsigned long frame)
		{
			if(frame > spline->length)
				return NULL;

			if(spline->mapAnim.find(frame) == spline->mapAnim.end())
				return NULL;

			return &spline->mapAnim[frame];
		}

		void CameraSpline::removePoint(unsigned long frame)
		{
			if(frame > spline->length)
				return;

			std::map<unsigned long, Ogre::Vector3>::iterator it = spline->mapAnim.find(frame);

			if(it == spline->mapAnim.end())
				return;

			spline->mapAnim.erase(it);
		}
	};
};
