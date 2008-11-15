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


/*----------------------CMouseMotionEventsTranslator-----------------------*\
|   This is translate mouse motion events to action events                  |
|                                                                           |
|   Changelog :                                                             |
|               01/11/2008 - Paf - Initial release                          |
|               01/13/2008 - Paf - Add sendZMotionEvents() to take Z axis   |
|                                    in account                             |
|                                                                           |
\*-------------------------------------------------------------------------*/

#ifndef __CMOUSEMOTIONEVENTSTRANSLATOR_H__
#define __CMOUSEMOTIONEVENTSTRANSLATOR_H__

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

		class CMouseMotionEventsTranslator : public CMessageListener
		{

			private:

				/**
				 * DynamicObject which contains a translation map for events from mouse
				 */
				shared_ptr<Gnoll::DynamicObject::DynamicObject> mouseMotionEventTranslationMap;


				/**
				 * DynamicObject which contains mouse config
				 */
				shared_ptr<Gnoll::DynamicObject::DynamicObject> mouseConfig;


				/**
				 * CMessageType for MOUSE_MOVED messages
				 */
				CMessageType mouseMoved;


				/**
				 * Maximum X coordinate for the mouse
				 */
				float maxX;


				/**
				 * Maximum Y coordinate for the mouse
				 */
				float maxY;


				/**
				 * Create an INPUT_ACTION_EVENT message from an event type and its intensity
				 * @param _event event type
				 * @param _intensity Intensity of the event
				 */
				void sendActionEventForEventAndIntensity(string _event, float _intensity);


				/**
				 * Extract X mouse motion and convert it to an INPUT_ACTION_EVENT
				 * @param _mouseEvent Contains mouse motion information
				 */
				void sendXMotionEvents(MouseEvent _mouseEvent);


				/**
				 * Extract Y mouse motion and convert it to an INPUT_ACTION_EVENT
				 * @param _mouseEvent Contains mouse motion information
				 */
				void sendYMotionEvents(MouseEvent _mouseEvent);


				/**
				 * Extract Z mouse motion and convert it to an INPUT_ACTION_EVENT
				 * @param _mouseEvent Contains mouse motion information
				 */
				void sendZMotionEvents(MouseEvent _mouseEvent);


			public:

				/**
				* This is a constructor
				*/
				CMouseMotionEventsTranslator();


				/**
				* This is a destructor
				*/
				virtual ~CMouseMotionEventsTranslator(); 



				/**
				* This method is called in order to process a message
				* @param message The message this method will have to process
				*/
				virtual void handle ( shared_ptr<CMessage> message );
		};
	};
};

#endif // __CMOUSEMOTIONEVENTSTRANSLATOR_H__


