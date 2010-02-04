/**************************************************************************
*   Copyright (C) 2008 by Paf                                             *
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


#ifndef __CMOUSEMOTIONSTATETRANSLATOR_H__
#define __CMOUSEMOTIONSTATETRANSLATOR_H__

#include <boost/shared_ptr.hpp>

#include "../../dynamicobject/include/dynamicobject.h"
#include "../../core/messages/include/listener.h"
#include "../../core/include/cmessage.h"

#include "cinputmouseevents.h"

using namespace boost;
using namespace Gnoll::Core;
using namespace Gnoll::DynamicObject;

namespace Gnoll
{
	namespace Input
	{

		/// This is translate mouse motion events to stateful events
		class CMouseMotionStateTranslator : public Messages::Listener
		{
			private:
				/**
				 * DynamicObject which contains mouse config
				 */
				shared_ptr<Gnoll::DynamicObject::DynamicObject> mouseConfig;

				/**
				 * Maximum X coordinate for the mouse
				 */
				float maxX;


				/**
				 * Maximum Y coordinate for the mouse
				 */
				float maxY;


                /**
				 * Distance from the border when we send BORDER message
				 */
				float border;


                /**
				 * Sensibility of the mouse
				 */
				float sensibility;


				/**
				 * Messages::MessageType for MOUSE_MOVED messages
				 */
				Messages::MessageType mouseMovedEvent;

			public:

				/**
				* This is a constructor
				*/
				CMouseMotionStateTranslator();

				/**
				 * Send Left Border message
				 * @param _mouseEvent Contains mouse motion information
				 */
				void sendLeftBorderState(MouseEvent mouseEvent);

				/**
				 * Send Right Border message
				 * @param _mouseEvent Contains mouse motion information
				 */
				void sendRightBorderState(MouseEvent mouseEvent);

                /**
				 * Send Up Border message
				 * @param _mouseEvent Contains mouse motion information
				 */
				void sendTopBorderState(MouseEvent mouseEvent);

				/**
				 * Send Down Border message
				 * @param _mouseEvent Contains mouse motion information
				 */
				void sendBottomBorderState(MouseEvent mouseEvent);

				/**
				* This is a destructor
				*/
				virtual ~CMouseMotionStateTranslator();

				/**
				* This method is called in order to process a message
				* @param message The message this method will have to process
				*/
				virtual void handle(MessagePtr message);
		};
	};
};

#endif

