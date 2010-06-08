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

#include "../include/ogrecamerawrapper.h"
#include "../include/camerafreeflyfactory.h"
#include "../include/camerafreefly.h"
#include "../include/defaultcamerafreeflylistener.h"
#include "../include/cmessagelistenercamera.h"
#include "../../core/include/message.h"
#include "../../core/messages/include/messagetype.h"
#include "../../core/messages/include/messenger.h"
#include "../../input/include/coisinputmodule.h"
#include "../../input/include/ctranslationevents.h"
#include "../../input/include/cinputmouseevents.h"

namespace Gnoll
{
	namespace Scene
	{
		boost::shared_ptr<Camera> CameraFreeFlyFactory::createCamera(const Glib::ustring& instanceName)
		{
			// Create the wrapper
			// ------------------
			shared_ptr<OgreCameraWrapper> wrapper = shared_ptr<OgreCameraWrapper>(new OgreCameraWrapper(instanceName));

			Gnoll::Core::MessageModule* messageModule = Gnoll::Core::MessageModule::getInstancePtr();
			shared_ptr<MessageListenerCamera> listenerInput(new DefaultCameraFreeFlyListener);
			messageModule->getMessageManager()->addListener ( listenerInput, Gnoll::Core::Messages::MessageType(Gnoll::Input::ACTION_EVENT_TYPE) );
			messageModule->getMessageManager()->addListener ( listenerInput, Gnoll::Core::Messages::MessageType(Gnoll::Input::ACTION_EVENT_STATE_TYPE) );
			boost::shared_ptr<Camera> pCam = boost::shared_ptr<Camera>(new CameraFreeFly(instanceName, wrapper));
			listenerInput->setCamera(pCam);

			return pCam;
		}
	};
};
