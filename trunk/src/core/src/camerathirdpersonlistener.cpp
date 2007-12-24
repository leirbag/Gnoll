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


/*---------------------------------integer---------------------------------*\
|   This is the listener for camera third person                            |
|                                                                           |
|   Changelog :                                                             |
|               11/20/2007 - Gabriel - Initial release                      |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include "../include/camerathirdpersonlistener.h"
#include "../../input/include/coisinputmodule.h"
#include "../../input/include/cinputmouseevents.h"
#include "../include/camerathirdperson.h"

namespace Gnoll
{
	namespace Core
	{
		void CtpKeyUp::handle ( shared_ptr<CMessage> message ) 
		{ 
			OIS::KeyCode key = message->getData<OIS::KeyCode>();

			if(key == OIS::KC_UP)
				g_mapCtpKeys["MoveUp"] = false;

			if(key == OIS::KC_DOWN)
				g_mapCtpKeys["MoveDown"] = false;

			if(key == OIS::KC_E)
				g_mapCtpKeys["RotateUp"] = false;

			if(key == OIS::KC_D)
				g_mapCtpKeys["RotateDown"] = false;

			if(key == OIS::KC_S)
				g_mapCtpKeys["RotateLeft"] = false;

			if(key == OIS::KC_F)
				g_mapCtpKeys["RotateRight"] = false;
		}
		
		void CtpKeyDown::handle ( shared_ptr<CMessage> message ) 
		{ 
			OIS::KeyCode key = message->getData<OIS::KeyCode>();

			if(key == OIS::KC_UP)
				g_mapCtpKeys["MoveUp"] = true;

			if(key == OIS::KC_DOWN)
				g_mapCtpKeys["MoveDown"] = true;

			if(key == OIS::KC_E)
				g_mapCtpKeys["RotateUp"] = true;

			if(key == OIS::KC_D)
				g_mapCtpKeys["RotateDown"] = true;

			if(key == OIS::KC_S)
				g_mapCtpKeys["RotateLeft"] = true;

			if(key == OIS::KC_F)
				g_mapCtpKeys["RotateRight"] = true;
		}

		MoveCameraThirdPersonListener::MoveCameraThirdPersonListener(Gnoll::Core::CameraThirdPerson* pInstanceCam) 
		{
			m_pInstanceCam = pInstanceCam;
		}

		void MoveCameraThirdPersonListener::handle ( shared_ptr<CMessage> message ) 
		{ 
			m_pInstanceCam->update(0);
		}

		RotateCameraThirdPersonListener::RotateCameraThirdPersonListener(Gnoll::Core::CameraThirdPerson* pInstanceCam) 
		{
			m_pInstanceCam = pInstanceCam;
		}

		void RotateCameraThirdPersonListener::handle ( shared_ptr<CMessage> message ) 
		{ 
			unsigned long temp = message->getData<unsigned long>();
			float lasttime = temp / 1000.0f;

			if(g_mapCtpKeys["RotateLeft"])
				m_pInstanceCam->rotateAxisY(1.0f * lasttime);

			if(g_mapCtpKeys["RotateRight"])
				m_pInstanceCam->rotateAxisY(-1.0f * lasttime);

			if(g_mapCtpKeys["RotateUp"])
				m_pInstanceCam->rotateAxisX(1.0f * lasttime);

			if(g_mapCtpKeys["RotateDown"])
				m_pInstanceCam->rotateAxisX(-1.0f * lasttime);
		}

		MouseRotateCameraThirdPersonListener::MouseRotateCameraThirdPersonListener(Gnoll::Core::CameraThirdPerson* pInstanceCam) 
		{
			m_pInstanceCam = pInstanceCam;
		}

		void MouseRotateCameraThirdPersonListener::handle ( shared_ptr<CMessage> message ) 
		{ 
			MouseEvent event = message->getData<MouseEvent>();

			m_pInstanceCam->rotateAxisY(-(event.relX / 20.0f) * 3.14 / 180);
			m_pInstanceCam->rotateAxisX(-(event.relY / 20.0f) * 3.14 / 180);
		}
	}
}