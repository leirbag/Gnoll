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
 * This is an implement of camera spline wrapper for Ogre                  *
 ***************************************************************************/

#include "../include/ogrecamerasplinewrapper.h"
#include "../../graphic/include/graphicmodule.h"

using namespace Gnoll::Graphic;
using namespace Gnoll::Core;

namespace Gnoll
{
	namespace Scene
	{
		OgreCameraSplineWrapper::OgreCameraSplineWrapper(const Glib::ustring& name) :
			OgreCameraWrapper(name)
		{
			// Create a scene node for the spline camera, the name is fixed
			// ------------------------------------------------------------
			Ogre::SceneNode* camNode = GraphicModule::getInstancePtr()->getSceneManager()->getRootSceneNode()->createChildSceneNode("GNOLL_CAMERA_SPLINE_NODE");
			camNode->attachObject(camera);
		}

		OgreCameraSplineWrapper::~OgreCameraSplineWrapper()
		{
			// Detach the camera from the scene node
			// -------------------------------------
			Ogre::SceneNode* camNode = GraphicModule::getInstancePtr()->getSceneManager()->getRootSceneNode()->createChildSceneNode("GNOLL_CAMERA_SPLINE_NODE");
			camNode->detachObject(camera);
		}
	};
};
