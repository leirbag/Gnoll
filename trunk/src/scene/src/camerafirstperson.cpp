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


/*---------------------------------CameraFirstPerson-----------------------*\
|   This is a camera first person                                           |
|                                                                           |
|   Changelog :                                                             |
|               04/12/2008 - Gabriel - Initial release                      |
\*-------------------------------------------------------------------------*/

#include "../include/camerafirstperson.h"
#include "../../dynamicobject/include/float.h"
#include "../../log/include/clogmodule.h"
#include "../../dynamicobject/include/vector3.h"

using namespace Gnoll::DynamicObject;

namespace Gnoll
{
	namespace Scene
	{
		CameraFirstPerson::CameraFirstPerson(const Glib::ustring& instanceName) :
			Camera(instanceName)
		{
			// Extract head position
			shared_ptr<Gnoll::DynamicObject::Vector3> default_head = shared_ptr<Gnoll::DynamicObject::Vector3> (new Gnoll::DynamicObject::Vector3());
			shared_ptr<Gnoll::DynamicObject::Vector3> temp_head = this->getAttributeOrDefault<Gnoll::DynamicObject::Vector3>("Head_position", default_head);

			// We need to convert the DO Vector3 to Ogre::Vector3 (inheritance)
			setHeadPosition(*dynamic_pointer_cast<Ogre::Vector3>(temp_head));

			(*Gnoll::Log::CLogModule::getInstancePtr()) << "Head position: " << *dynamic_pointer_cast<Ogre::Vector3>(temp_head) << "\t";
		}

		CameraFirstPerson::~CameraFirstPerson()
		{
			setAttribute("Head_position", shared_ptr<Gnoll::DynamicObject::Vector3>(new Gnoll::DynamicObject::Vector3(m_headPosition)));
		}

		void CameraFirstPerson::setHeadPosition(const Ogre::Vector3& head)
		{
			m_headPosition = head;
		}

		void CameraFirstPerson::update(float time)
		{
			if(getTarget() == NULL)
				return;

			// Store the old position to calculate the movement of the head position
			static Ogre::Vector3 oldPosition = getTarget()->getPosition();

			Ogre::Vector3 position = getTarget()->getPosition();
			m_headPosition += (position - oldPosition);
			position = m_headPosition;

			getOgreCamera()->setPosition(position);
			getOgreCamera()->setDirection(-getTarget()->getOrientation().zAxis());

			oldPosition = getTarget()->getPosition();
		}
	};
};
