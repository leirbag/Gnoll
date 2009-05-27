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


/*----------------------CMouseButtonStateTranslator------------------------*\
|   This is translate mouse button events to action events                  |
|                                                                           |
|   Changelog :                                                             |
|               01/11/2008 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/

#ifndef __CMOUSEMOTIONSTATETRANSLATOR_H__
#define __CMOUSEMOTIONSTATETRANSLATOR_H__

#include <boost/shared_ptr.hpp>

#include "../../dynamicobject/include/dynamicobject.h"
#include "../../core/include/cmessagelistener.h"
#include "../../core/include/cmessage.h"

#include "cinputmouseevents.h"

using namespace boost;
using namespace Gnoll::Core;
using namespace Gnoll::DynamicObject;

namespace Gnoll
{
	namespace Input
	{

		class CMouseMotionStateTranslator : public CMessageListener
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
				 * CMessageType for MOUSE_MOVED messages
				 */
				CMessageType mouseButtonMovedEvent;


			public:

				/**
				* This is a constructor
				*/
				CMouseMotionStateTranslator();

				/**
				 * Send Y Border message
				 * @param _mouseEvent Contains mouse motion information
				 */
				void sendYBorderState(MouseEvent mouseEvent);

				/**
				 * Send X Border message
				 * @param _mouseEvent Contains mouse motion information
				 */
				void sendXBorderState(MouseEvent mouseEvent);

				/**
				* This is a destructor
				*/
				virtual ~CMouseMotionStateTranslator();

				/**
				* This method is called in order to process a message
				* @param message The message this method will have to process
				*/
				virtual void handle ( shared_ptr<CMessage> message );
		};
	};
};

#endif // __CMOUSESTATETRANSLATOR_H__
