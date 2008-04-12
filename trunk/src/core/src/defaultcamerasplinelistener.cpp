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


/*-------------------------------DefaultCameraSplineListener---------------*\
|   This is a spline camera listener                                        |
|                                                                           |
|   Changelog :                                                             |
|               04/12/2008 - Gabriel - Initial release                      |
\*-------------------------------------------------------------------------*/

#include "../include/defaultcamerasplinelistener.h"
#include "../include/cameraspline.h"
#include "../../stats/include/cstatsmodule.h"
#include "../../input/include/ctranslationevents.h"
#include "../../input/include/cinputmouseevents.h"
#include "../include/float.h"

namespace Gnoll
{
	namespace Core
	{
		DefaultCameraSplineListener::DefaultCameraSplineListener()
		{
		}

		DefaultCameraSplineListener::~DefaultCameraSplineListener()
		{
		}

		void DefaultCameraSplineListener::handle ( shared_ptr<CMessage> message )
		{
			Gnoll::Input::ActionEvent ae = message->getData<Gnoll::Input::ActionEvent>();
			float lasttime = Gnoll::Stats::CStatsModule::getInstancePtr()->getRenderTime();
			lasttime = lasttime * 1000.0f;

			shared_ptr<CameraSpline> pCam = static_pointer_cast<CameraSpline>(m_pCamera);

			// Update
			pCam->update(lasttime);
		}
	};
};
