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

/*--------------------------CameraThirdPersonFactory-----------------------*\
|   This is an implementation of the abstract factory                       |
\*-------------------------------------------------------------------------*/

#ifndef INCLUDED_CAMERATHIRDPERSONFACTORY
#define INCLUDED_CAMERATHIRDPERSONFACTORY

#include "abstractcamerafactory.h"

namespace Gnoll
{
	namespace Scene
	{
		class CameraThirdPersonFactory : public AbstractCameraFactory
		{
		public:
			/**
			 * This method create a fixe camera : inherits methods
			 * @param instanceName This is the instance name of the camera
			 * @return the camera create, NULL if error
			 */
			boost::shared_ptr<Camera> createCamera(const Glib::ustring& instanceName);
		};
	};
};

#endif
