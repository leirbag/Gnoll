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

namespace Gnoll
{
	namespace Scene
	{
		CameraFirstPerson::CameraFirstPerson(const Glib::ustring& instanceName) :
			Camera(instanceName)
		{
		}

		CameraFirstPerson::CameraFirstPerson(const CameraFirstPerson& copy) :
			Camera(copy)
		{
		}

		CameraFirstPerson::~CameraFirstPerson()
		{
			// NOTHING TO DO
			// NOTHING TO SERIALISE
		}

		void CameraFirstPerson::update(float time)
		{
			if(getTarget() == NULL)
				return;

			getOgreCamera()->setPosition(getTarget()->getPosition());
			getOgreCamera()->setDirection(-getTarget()->getOrientation().zAxis());
		}
	};
};