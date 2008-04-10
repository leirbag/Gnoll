
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


/*---------------------------------CameraFixe------------------------------*\
|   This is a fixed camera                                                  |
|                                                                           |
|   Changelog :                                                             |
|               08/30/2007 - Gabriel - Initial release                      |
|               10/30/2007 - Gabriel - add time to update()                 |
|               12/24/2007 - Gabriel - delete scenemanager from Ctor        |
|                                                                           |
\*-------------------------------------------------------------------------*/

#ifndef INCLUDED_CAMERAFIXE
#define INCLUDED_CAMERAFIXE

#include "camera.h"

namespace Gnoll
{
	namespace Core
	{
		/*
		 * Structure that contains camera attributs
		 */
		class CameraFixe : public Camera
		{
		public:
			/*
			 * Default constructor, it initializes the camera with default settings :
			 * position (0, 0, 0), direction (0, 0, 1), up (0, 1, 0), near 0, far is 200,
			 * fov PI/4 OR get back ancient configuration with persistant objet if exists
			 * @param instanceName This is the instance name of the camera, it will be use for the
			 * 					   instance name of the Ogre Camera
			 */
			explicit CameraFixe(const Glib::ustring& instanceName);

			/*
			 * Copy constructor
			 * @param copy This is the camera to copy
			 */
			explicit CameraFixe(const CameraFixe& copy);

			/*
			 * Destructior
			 */
			virtual ~CameraFixe();
		};
	};
};

#endif
