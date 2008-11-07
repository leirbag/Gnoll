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
#include "../../dynamicobject/include/list.h"
#include "../../log/include/clogmodule.h"
#include "../include/camerafixefactory.h"
#include "../include/camerafirstpersonfactory.h"
#include "../include/camerathirdpersonfactory.h"
#include "../include/camerafreeflyfactory.h"
#include "../include/camerasplinefactory.h"

namespace Gnoll
{
    namespace Scene
    {
	CameraManager::CameraManager()
	{
	    // Map extension with factory
	    extensionsMap["camfreefly"]     = shared_ptr<AbstractCameraFactory>(new CameraFreeFlyFactory());
	    extensionsMap["camfixe"]        = shared_ptr<AbstractCameraFactory>(new CameraFixeFactory());
	    extensionsMap["camfirstperson"] = shared_ptr<AbstractCameraFactory>(new CameraSplineFactory());
	    extensionsMap["camthisperson"]  = shared_ptr<AbstractCameraFactory>(new CameraThirdPersonFactory());
	    extensionsMap["camspline"]      = shared_ptr<AbstractCameraFactory>(new CameraFirstPersonFactory());
	}

	CameraManager::~CameraManager()
	{
	}

	shared_ptr<Camera> CameraManager::loadImpl( shared_ptr<IStream> _stream, string _instance)
	{
	    string type;

	    /**
	     * Get camera from the file extension
	     */
	    size_t pos = _instance.find_last_of('.') + 1;

	    if(pos != string::npos)
		type = _instance.substr(pos);

	    if (extensionsMap.find(type) != extensionsMap.end())
		return extensionsMap[type]->createCamera(_instance);
	    else
		return shared_ptr<Camera>();
	}

	bool CameraManager::saveImpl( shared_ptr<IStream> _stream, shared_ptr<Camera> _obj, string _instance)
	{
		return true;
	}
    }
}
