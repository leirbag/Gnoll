/***************************************************************************
 *   Copyright (C) 2007 by Paf                                             *
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


/*-----------------------------CameraManager ------------------------------*\
  |   This is the camera manager                                              |
  |                                                                           |
  |   Changelog :                                                             |
  |               07/10/2008 - Gabriel - Initial release                      |
  \*-------------------------------------------------------------------------*/

#include "../include/cameramanager.h"
#include "../../../dynamicobject/include/list.h"
#include "../../../log/include/logmacros.h"
#include "../factory/include/camerafixefactory.h"
#include "../factory/include/camerafirstpersonfactory.h"
#include "../factory/include/camerathirdpersonfactory.h"
#include "../factory/include/camerafreeflyfactory.h"
#include "../factory/include/camerasplinefactory.h"

namespace Gnoll
{
	namespace Scene
	{
		CameraManager::CameraManager()
		{
			// Map extension with factory
			extensionsMap["camfreefly"]     = shared_ptr<AbstractCameraFactory>(new CameraFreeFlyFactory());
			extensionsMap["camfixe"]        = shared_ptr<AbstractCameraFactory>(new CameraFixeFactory());
			extensionsMap["camfirstperson"] = shared_ptr<AbstractCameraFactory>(new CameraFirstPersonFactory());
			extensionsMap["camthirdperson"] = shared_ptr<AbstractCameraFactory>(new CameraThirdPersonFactory());
			extensionsMap["camspline"]      = shared_ptr<AbstractCameraFactory>(new CameraSplineFactory());
		}

		CameraManager::~CameraManager()
		{
			GNOLL_LOG() << "Camera manager exiting\n";
		}

		shared_ptr<Camera> CameraManager::loadImpl( shared_ptr<AbstractStream> _stream, string _instance)
		{
			string type;

			/**
			 * Get camera from the file extension
			 */
			size_t pos = _instance.find_last_of('.') + 1;

			if(pos != string::npos)
				type = _instance.substr(pos);

			if (extensionsMap.find(type) != extensionsMap.end())
			{
				camerasMap[_instance] = extensionsMap[type]->createCamera(_instance);
				return camerasMap[_instance];
			}
			else
			{
				camerasMap[_instance] = extensionsMap["camfreefly"]->createCamera(_instance);
				return camerasMap[_instance];
			}
		}

		bool CameraManager::saveImpl( shared_ptr<AbstractStream> _stream, shared_ptr<Camera> _obj, string _instance)
		{
			camerasMap[_instance].reset();
			camerasMap.erase(camerasMap.find(_instance));
			return true;
		}
	}
}
