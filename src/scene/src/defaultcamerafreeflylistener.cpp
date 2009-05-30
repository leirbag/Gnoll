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


/*-------------------------------DefaultCameraFreeFlyListener----------*\
|   This is a first person camera listener                                  |
|                                                                           |
|   Changelog :                                                             |
|               12/09/2008 - Gabriel - Initial release                      |
\*-------------------------------------------------------------------------*/

#include "../include/defaultcamerafreeflylistener.h"
#include "../include/camerafreefly.h"
#include "../../stats/include/cstatsmodule.h"
#include "../../input/include/ctranslationevents.h"
#include "../../input/include/cinputmouseevents.h"
#include "../../dynamicobject/include/float.h"
#include "../../log/include/clogmodule.h"

#include <sstream>

using namespace Gnoll::DynamicObject;


namespace Gnoll
{
	namespace Scene
	{
		DefaultCameraFreeFlyListener::DefaultCameraFreeFlyListener()
		{
			borderX = borderY = -1;
		}

		DefaultCameraFreeFlyListener::~DefaultCameraFreeFlyListener()
		{
		}

		void DefaultCameraFreeFlyListener::handle ( shared_ptr<CMessage> message )
		{
			Gnoll::Input::ActionEvent ae = message->getData<Gnoll::Input::ActionEvent>();
			float lasttime = Gnoll::Stats::CStatsModule::getInstancePtr()->getRenderTime();

			/**
			 * Converts weak ptr m_pCamera to a shared_ptr<>
			 * This is not an issue since cam has a very short live.
			 *
			 * We can't directly convert a weak_ptr<Camera> to a shared_ptr<CameraThirdPerson>
			 */
			shared_ptr<Camera> cam(m_pCamera);

			/**
			 * Static conversion to a shared_ptr<CameraThirdPerson>
			 */
			shared_ptr<CameraFreeFly> pCam = static_pointer_cast<CameraFreeFly>(cam);

			if(ae.action == "BORDER_X")
				borderX = ae.intensity;
			if(ae.action == "BORDER_Y")
				borderY = ae.intensity;

			if(ae.action == "ACTION_STRAFE_LEFT")
				pCam->strafeLR(ae.intensity * lasttime * -1);
			if(ae.action == "ACTION_STRAFE_RIGHT")
				pCam->strafeLR(ae.intensity * lasttime);
			if(ae.action == "ACTION_STEP_FORWARD")
				pCam->move(ae.intensity * lasttime);
			if(ae.action == "ACTION_STEP_BACKWARD")
				pCam->move(ae.intensity * lasttime * -1);

			float sensibility = -20.0f;  // Negative pour inverser les axes en meme temps

			if(borderX >= 0)
			{
				if(ae.action == "ACTION_ROTATE_CAMERA_LEFT" && borderX < 0.1)
					pCam->rotateAroundAxisY(ae.intensity * lasttime * sensibility);

				if(ae.action == "ACTION_ROTATE_CAMERA_RIGHT" && borderX > 0.9)
					pCam->rotateAroundAxisY(ae.intensity * lasttime * sensibility);
			}

			if(borderY >= 0)
			{
				if(ae.action == "ACTION_ROTATE_CAMERA_UP" && borderY > 0.9)
					pCam->rotateAroundAxisX(ae.intensity * lasttime * sensibility);

				if(ae.action == "ACTION_ROTATE_CAMERA_DOWN" && borderY < 0.1)
					pCam->rotateAroundAxisX(ae.intensity * lasttime * sensibility);
			}

			// Update
			pCam->update(lasttime);
		}
	};
};
