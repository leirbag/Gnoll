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


#ifndef __CAMERAMANAGER_H__
#define __CAMERAMANAGER_H__

#include <map>
#include <boost/shared_ptr.hpp>
#include "../include/abstractcamerafactory.h"
#include "../../core/include/basemanager.h"
#include "../include/camera.h"
#include "../../core/include/singleton.h"

using namespace std;
using namespace boost;

namespace Gnoll
{
	namespace Scene
	{
		class CameraManager : public BaseManager<Camera, ObjectNotFoundError<Camera> >, public Gnoll::Core::Singleton<CameraManager>
		{
			private:
				/*
				 * Map between camera's extension recognize and their factory
				 */
				map<string, shared_ptr<AbstractCameraFactory> > extensionsMap;

				/*
				 * Map between camera's instance name and the instance
				 */
				map<string, shared_ptr<Camera> > camerasMap;

			protected:

				/**
				 * @copydoc BaseManager::loadImpl
				 */
				virtual shared_ptr<Camera> loadImpl( shared_ptr<AbstractStream> _stream, string _instance);

				/**
				 * @copydoc BaseManager::saveImpl
				 */
				virtual bool saveImpl( shared_ptr<AbstractStream> _stream, shared_ptr<Camera> _obj, string _instance);

			public:

				/**
				 * This is a constructor
				 */
				CameraManager();


				/**
				 * This is a destructor
				 */
				~CameraManager();
		};
	}
}

#endif
