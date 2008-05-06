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


/*-------------------------------DefaultCameraThirdPersonListener----------*\
|   This is a first person camera listener                                  |
|                                                                           |
|   Changelog :                                                             |
|               04/08/2008 - Gabriel - Initial release                      |
|               04/10/2008 - Gabriel - Add the management of message        |
|                                                                           |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include "../include/defaultcamerathirdpersonlistener.h"
#include "../include/camerathirdperson.h"
#include "../../stats/include/cstatsmodule.h"
#include "../../input/include/ctranslationevents.h"
#include "../../input/include/cinputmouseevents.h"
#include "../../dynamicobject/include/float.h"


using namespace Gnoll::DynamicObject;


namespace Gnoll
{
	namespace Core
	{
		DefaultCameraThirdPersonListener::DefaultCameraThirdPersonListener()
		{
		}

		DefaultCameraThirdPersonListener::~DefaultCameraThirdPersonListener()
		{
		}

		void DefaultCameraThirdPersonListener::handle ( shared_ptr<CMessage> message )
		{
			Gnoll::Input::ActionEvent ae = message->getData<Gnoll::Input::ActionEvent>();
			float lasttime = Gnoll::Stats::CStatsModule::getInstancePtr()->getRenderTime();
			lasttime = lasttime * 1000.0f;

			shared_ptr<CameraThirdPerson> pCam = static_pointer_cast<CameraThirdPerson>(m_pCamera);

			// ZOOM
			if(ae.action == "ACTION_ZOOM_CAMERA_IN")
				pCam->setOffset(pCam->getOffset() + (-ae.intensity * lasttime));
			if(ae.action == "ACTION_ZOOM_CAMERA_OUT")
				pCam->setOffset(pCam->getOffset() + (-ae.intensity * lasttime));

			shared_ptr<Float> maxRotX = pCam->getAttributeOrDefault("MaximumRotationAroundX", shared_ptr<Float> (new Float(300.0f)));
			shared_ptr<Float> maxRotY = pCam->getAttributeOrDefault("MaximumRotationAroundY", shared_ptr<Float> (new Float(300.0f)));

			// KEYBOARD
			if(ae.action == "ACTION_ROTATE_CAMERA_LEFT")
			{
				std::cout << "Value : " << *maxRotY << std::endl;
				pCam->rotateAroundAxisY(*maxRotY * ae.intensity * lasttime);
			}
			if(ae.action == "ACTION_ROTATE_CAMERA_RIGHT")
				pCam->rotateAroundAxisY(*maxRotY * ae.intensity * lasttime);
			if(ae.action == "ACTION_ROTATE_CAMERA_UP")
				pCam->rotateAroundAxisX(*maxRotX * ae.intensity * lasttime);
			if(ae.action == "ACTION_ROTATE_CAMERA_DOWN")
				pCam->rotateAroundAxisX(*maxRotX * ae.intensity * lasttime);

			// MOUSE
			if(ae.action == "ACTION_MROTATE_CAMERA_LEFT")
				pCam->rotateAroundAxisY(*maxRotY * ae.intensity * lasttime);
			if(ae.action == "ACTION_MROTATE_CAMERA_RIGHT")
				pCam->rotateAroundAxisY(*maxRotY * ae.intensity * lasttime);
			if(ae.action == "ACTION_MROTATE_CAMERA_UP")
				pCam->rotateAroundAxisX(*maxRotX * ae.intensity * lasttime);
			if(ae.action == "ACTION_MROTATE_CAMERA_DOWN")
				pCam->rotateAroundAxisX(*maxRotX * ae.intensity * lasttime);

			// Update
			pCam->update(lasttime);
		}
	};
};
