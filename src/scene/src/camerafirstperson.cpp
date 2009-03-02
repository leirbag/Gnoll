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

/****************************** Summary ************************************
 * This is an implementation of a First Person Camera, it provides some    *
 * services:                                                               *
 * 	- Set up of a "head" position                                      *
 ***************************************************************************/

#include "../include/camerafirstperson.h"
#include "../../dynamicobject/include/float.h"
#include "../../log/include/clogmacros.h"
#include "../../dynamicobject/include/vector3.h"

using namespace Gnoll::DynamicObject;

namespace Gnoll
{
	namespace Scene
	{
		CameraFirstPerson::CameraFirstPerson(const Glib::ustring& instanceName, shared_ptr<CameraWrapper> wrapper) :
			Camera(instanceName, wrapper)
		{
            Ogre::Vector3* pPosition = NULL;
            Ogre::SceneNode* pTarget = getTarget();
            if(pTarget == NULL)
                pPosition = new Ogre::Vector3();
            else
                pPosition = new Ogre::Vector3(pTarget->getPosition());

            shared_ptr<Gnoll::DynamicObject::Vector3> default_pos = shared_ptr<Gnoll::DynamicObject::Vector3> (new Gnoll::DynamicObject::Vector3(*pPosition));
            shared_ptr<Gnoll::DynamicObject::Vector3> temp_pos = this->getAttributeOrDefault<Gnoll::DynamicObject::Vector3>("HeadPosition", default_pos);
            headPosition = new Ogre::Vector3(*dynamic_pointer_cast<Ogre::Vector3>(temp_pos));
		}

		CameraFirstPerson::~CameraFirstPerson()
		{
            setAttribute("HeadPosition", shared_ptr<Gnoll::DynamicObject::Vector3>(new Gnoll::DynamicObject::Vector3(*getHeadPosition())));
		}

		void CameraFirstPerson::setHeadPosition(const Ogre::Vector3& value)
		{
            *headPosition = value;
		}

        const Ogre::Vector3* CameraFirstPerson::getHeadPosition()
		{
            return headPosition;
		}

		void CameraFirstPerson::update(float time)
		{
			// Get back the position of the target
			// -----------------------------------
			Ogre::SceneNode* pTarget = getTarget();
			if(pTarget == NULL) {
				GNOLL_LOG() << "No target\n";
				return;
			}

            // Check the validity of the head position
            // ---------------------------------------
            if(getHeadPosition() == NULL) {
                GNOLL_LOG() << "No head position defined\n";
				return;
            }

			// Process to the position of the camera
			// -------------------------------------
			Ogre::Vector3* newPosition = new Ogre::Vector3(pTarget->getPosition() + *getHeadPosition());
			cameraWrapper->setPosition(*newPosition);
		}
	};
};
