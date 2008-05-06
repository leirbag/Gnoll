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

#ifndef INCLUDED_CAMERAFIRSTPERSON
#define INCLUDED_CAMERAFIRSTPERSON

#include "camera.h"

namespace Gnoll
{
	namespace Core
	{
		class CameraFirstPerson : public Camera
		{
		public:
			/*
			 * Default constructor
			 * @param instanceName This is the instance name of the camera, it will be use for the
			 * 					   instance name of the Ogre Camera
			 */
			explicit CameraFirstPerson(const Glib::ustring& instanceName);

			/*
			 * Copy constructor
			 * @param copy This is the camera to copy
			 */
			explicit CameraFirstPerson(const CameraFirstPerson& copy);

			/*
			 * Destructior
			 */
			virtual ~CameraFirstPerson();

			/*
			 * This method is call each frame
			 * @param time This is the time between 2 frames
			 */
			virtual void update(float time);
		};
	};
};

#endif
