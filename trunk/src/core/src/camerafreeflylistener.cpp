/**************************************************************************
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


/*---------------------------------integer---------------------------------*\
|   This is the listener for camera free fly                                |
|                                                                           |
|   Changelog :                                                             |
|               11/20/2007 - Gabriel - Initial release                      |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include "../include/camerafreeflylistener.h"
#include "../../input/include/coisinputmodule.h"
#include "../../input/include/cinputmouseevents.h"
#include "../include/camerafreefly.h"

namespace Gnoll
{
	namespace Core
	{
		void KeyUp::handle(shared_ptr<CMessage> message)
		{
			OIS::KeyCode key = message->getData<OIS::KeyCode>();

			if(key == OIS::KC_UP)
				g_mapCffKeys["MoveUp"] = false;

			if(key == OIS::KC_DOWN)
				g_mapCffKeys["MoveDown"] = false;

			if(key == OIS::KC_E)
				g_mapCffKeys["RotateUp"] = false;

			if(key == OIS::KC_D)
				g_mapCffKeys["RotateDown"] = false;

			if(key == OIS::KC_S)
				g_mapCffKeys["RotateLeft"] = false;

			if(key == OIS::KC_F)
				g_mapCffKeys["RotateRight"] = false;

			if(key == OIS::KC_C)
				g_mapCffKeys["StrafeRight"] = false;

			if(key == OIS::KC_V)
				g_mapCffKeys["StrafeLeft"] = false;

			if(key == OIS::KC_B)
				g_mapCffKeys["StrafeUp"] = false;

			if(key == OIS::KC_N)
				g_mapCffKeys["StrafeDown"] = false;
		}

		void KeyDown::handle(shared_ptr<CMessage> message)
		{
			OIS::KeyCode key = message->getData<OIS::KeyCode>();

			if(key == OIS::KC_UP)
				g_mapCffKeys["MoveUp"] = true;

			if(key == OIS::KC_DOWN)
				g_mapCffKeys["MoveDown"] = true;

			if(key == OIS::KC_E)
				g_mapCffKeys["RotateUp"] = true;

			if(key == OIS::KC_D)
				g_mapCffKeys["RotateDown"] = true;

			if(key == OIS::KC_S)
				g_mapCffKeys["RotateLeft"] = true;

			if(key == OIS::KC_F)
				g_mapCffKeys["RotateRight"] = true;

			if(key == OIS::KC_C)
				g_mapCffKeys["StrafeRight"] = true;

			if(key == OIS::KC_V)
				g_mapCffKeys["StrafeLeft"] = true;

			if(key == OIS::KC_B)
				g_mapCffKeys["StrafeUp"] = true;

			if(key == OIS::KC_N)
				g_mapCffKeys["StrafeDown"] = true;
		}

		MoveCameraFreeFlyListener::MoveCameraFreeFlyListener(Gnoll::Core::CameraFreeFly* pInstanceCam)
		{
			m_pInstanceCam = pInstanceCam;
		}

		void MoveCameraFreeFlyListener::handle(shared_ptr<CMessage> message)
		{
			unsigned long temp = message->getData<unsigned long>();
			float lasttime = temp / 1000.0f;

			if(g_mapCffKeys["MoveUp"])
				m_pInstanceCam->move(Ogre::Vector3(0.0f, 0.0f, -10.0f * lasttime));

			if(g_mapCffKeys["MoveDown"])
				m_pInstanceCam->move(Ogre::Vector3(0.0f, 0.0f, 10.0f * lasttime));
		}

		RotateCameraFreeFlyListener::RotateCameraFreeFlyListener(Gnoll::Core::CameraFreeFly* pInstanceCam)
		{
			m_pInstanceCam = pInstanceCam;
		}

		void RotateCameraFreeFlyListener::handle(shared_ptr<CMessage> message)
		{
			unsigned long temp = message->getData<unsigned long>();
			float lasttime = temp / 1000.0f;

			if(g_mapCffKeys["RotateUp"])
				m_pInstanceCam->rotateAxisX(1.0f * lasttime);

			if(g_mapCffKeys["RotateDown"])
				m_pInstanceCam->rotateAxisX(-1.0f * lasttime);

			if(g_mapCffKeys["RotateLeft"])
				m_pInstanceCam->rotateAxisY(1.0f * lasttime);

			if(g_mapCffKeys["RotateRight"])
				m_pInstanceCam->rotateAxisY(-1.0f * lasttime);
		}

		MouseRotateCameraFreeFlyListener::MouseRotateCameraFreeFlyListener(Gnoll::Core::CameraFreeFly* pInstanceCam)
		{
			m_pInstanceCam = pInstanceCam;
		}

		void MouseRotateCameraFreeFlyListener::handle(shared_ptr<CMessage> message)
		{
			MouseEvent event = message->getData<MouseEvent>();

			m_pInstanceCam->rotateAxisY(-(event.relX / 20.0f) * 3.14 / 180);
			m_pInstanceCam->rotateAxisX(-(event.relY / 20.0f) * 3.14 / 180);
		}

		StrafeCameraFreeFlyListener::StrafeCameraFreeFlyListener(Gnoll::Core::CameraFreeFly* pInstanceCam)
		{
			m_pInstanceCam = pInstanceCam;
		}

		void StrafeCameraFreeFlyListener::handle(shared_ptr<CMessage> message)
		{
			unsigned long temp = message->getData<unsigned long>();
			float lasttime = temp / 1000.0f;

			if(g_mapCffKeys["StrafeLeft"])
				m_pInstanceCam->strafe(10.0f * lasttime);

			if(g_mapCffKeys["StrafeRight"])
				m_pInstanceCam->strafe(-10.0f * lasttime);

			if(g_mapCffKeys["StrafeUp"])
				m_pInstanceCam->strafeUp(10.0f * lasttime);

			if(g_mapCffKeys["StrafeDown"])
				m_pInstanceCam->strafeUp(-10.0f * lasttime);
		}
	}
}


