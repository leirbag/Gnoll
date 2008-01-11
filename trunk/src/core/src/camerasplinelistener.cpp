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
|   This is the listener for camera spline                                  |
|                                                                           |
|   Changelog :                                                             |
|               11/20/2007 - Gabriel - Initial release                      |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include "../include/cameraspline.h"

namespace Gnoll
{
	namespace Core
	{
		UpdateCameraSplineListener::UpdateCameraSplineListener(Gnoll::Core::CameraSpline* pInstanceCam) 
		{
			m_pInstanceCam = pInstanceCam;
		}

		void UpdateCameraSplineListener::handle ( shared_ptr<CMessage> message ) 
		{ 
			unsigned long temp = message->getData<unsigned long>();
			float lasttime = temp / 1000.0f;

			m_pInstanceCam->update(lasttime);
		}
	}
}



