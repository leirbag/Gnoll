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
|   This is the listener for camera free fly                                |
|                                                                           |
|   Changelog :                                                             |
|               11/20/2007 - Gabriel - Initial release                      |
|                                                                           |
\*-------------------------------------------------------------------------*/

#ifndef __CAMERAFREEFLYLISTENER_H__
#define __CAMERAFREEFLYLISTENER_H__

#include <string>
#include <map>
#include "cmessagelistener.h"
#include "cmessage.h"

namespace Gnoll
{
	namespace Core 
	{
		class CameraFreeFly;
		static std::map<std::string, bool> g_mapCffKeys;

		class KeyUp : public CMessageListener
		{
		public:

			/**
			* This is a constructor
			*/
			KeyUp() {}

			/**
			* This is a destructor
			*/
			virtual ~KeyUp() {}

			/**
			* This method is called in order to process a message
			* @param message The message this method will have to process
			*/
			virtual void handle ( shared_ptr<CMessage> message );
		};

		class KeyDown : public CMessageListener
		{
		public:

			/**
			* This is a constructor
			*/
			KeyDown() {}

			/**
			* This is a destructor
			*/
			virtual ~KeyDown() {}

			/**
			* This method is called in order to process a message
			* @param message The message this method will have to process
			*/
			virtual void handle ( shared_ptr<CMessage> message );
		};

		class MoveCameraFreeFlyListener : public CMessageListener
		{
		private:
			/**
			* This is the instance of the camera
			*/
			Gnoll::Core::CameraFreeFly* m_pInstanceCam;

		public:

			/**
			* This is a constructor
			*/
			MoveCameraFreeFlyListener(Gnoll::Core::CameraFreeFly* pInstanceCam);

			/**
			* This is a destructor
			*/
			virtual ~MoveCameraFreeFlyListener() {}

			/**
			* This method is called in order to process a message
			* @param message The message this method will have to process
			*/
			virtual void handle ( shared_ptr<CMessage> message );
		};

		class RotateCameraFreeFlyListener : public CMessageListener
		{
		private:
			/**
			* This is the instance of the camera
			*/
			Gnoll::Core::CameraFreeFly* m_pInstanceCam;

		public:

			/**
			* This is a constructor
			*/
			RotateCameraFreeFlyListener(Gnoll::Core::CameraFreeFly* pInstanceCam);

			/**
			* This is a destructor
			*/
			virtual ~RotateCameraFreeFlyListener() {}

			/**
			* This method is called in order to process a message
			* @param message The message this method will have to process
			*/
			virtual void handle ( shared_ptr<CMessage> message );
		};

		class MouseRotateCameraFreeFlyListener : public CMessageListener
		{
		private:
			/**
			* This is the instance of the camera
			*/
			Gnoll::Core::CameraFreeFly* m_pInstanceCam;

		public:

			/**
			* This is a constructor
			*/
			MouseRotateCameraFreeFlyListener(Gnoll::Core::CameraFreeFly* pInstanceCam);

			/**
			* This is a destructor
			*/
			virtual ~MouseRotateCameraFreeFlyListener() {}

			/**
			* This method is called in order to process a message
			* @param message The message this method will have to process
			*/
			virtual void handle ( shared_ptr<CMessage> message );
		};

		class StrafeCameraFreeFlyListener : public CMessageListener
		{
		private:
			/**
			* This is the instance of the camera
			*/
			Gnoll::Core::CameraFreeFly* m_pInstanceCam;

		public:

			/**
			* This is a constructor
			*/
			StrafeCameraFreeFlyListener(Gnoll::Core::CameraFreeFly* pInstanceCam);

			/**
			* This is a destructor
			*/
			virtual ~StrafeCameraFreeFlyListener() {}

			/**
			* This method is called in order to process a message
			* @param message The message this method will have to process
			*/
			virtual void handle ( shared_ptr<CMessage> message );
		};
	};
};

#endif // __CAMERAFREEFLYLISTENER_H__
