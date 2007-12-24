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

#ifndef __CAMERATHIRDPERSONLISTENER_H__
#define __CAMERATHIRDPERSONLISTENER_H__

#include <string>
#include <map>
#include "cmessagelistener.h"
#include "cmessage.h"

namespace Gnoll
{
	namespace Core 
	{
		class CameraThirdPerson;
		static std::map<std::string, bool> g_mapCtpKeys;

		class CtpKeyUp : public CMessageListener
		{
		public:

			/**
			* This is a constructor
			*/
			CtpKeyUp() {}

			/**
			* This is a destructor
			*/
			virtual ~CtpKeyUp() {}

			/**
			* This method is called in order to process a message
			* @param message The message this method will have to process
			*/
			virtual void handle ( shared_ptr<CMessage> message );
		};

		class CtpKeyDown : public CMessageListener
		{
		public:

			/**
			* This is a constructor
			*/
			CtpKeyDown() {}

			/**
			* This is a destructor
			*/
			virtual ~CtpKeyDown() {}

			/**
			* This method is called in order to process a message
			* @param message The message this method will have to process
			*/
			virtual void handle ( shared_ptr<CMessage> message );
		};

		class MoveCameraThirdPersonListener : public CMessageListener
		{
		private:
			/**
			* This is the instance of the camera
			*/
			Gnoll::Core::CameraThirdPerson* m_pInstanceCam;

		public:

			/**
			* This is a constructor
			*/
			MoveCameraThirdPersonListener(Gnoll::Core::CameraThirdPerson* pInstanceCam);

			/**
			* This is a destructor
			*/
			virtual ~MoveCameraThirdPersonListener() {}

			/**
			* This method is called in order to process a message
			* @param message The message this method will have to process
			*/
			virtual void handle ( shared_ptr<CMessage> message );
		};

		class RotateCameraThirdPersonListener : public CMessageListener
		{
		private:
			/**
			* This is the instance of the camera
			*/
			Gnoll::Core::CameraThirdPerson* m_pInstanceCam;

		public:

			/**
			* This is a constructor
			*/
			RotateCameraThirdPersonListener(Gnoll::Core::CameraThirdPerson* pInstanceCam);

			/**
			* This is a destructor
			*/
			virtual ~RotateCameraThirdPersonListener() {}

			/**
			* This method is called in order to process a message
			* @param message The message this method will have to process
			*/
			virtual void handle ( shared_ptr<CMessage> message );
		};

		class MouseRotateCameraThirdPersonListener : public CMessageListener
		{
		private:
			/**
			* This is the instance of the camera
			*/
			Gnoll::Core::CameraThirdPerson* m_pInstanceCam;

		public:

			/**
			* This is a constructor
			*/
			MouseRotateCameraThirdPersonListener(Gnoll::Core::CameraThirdPerson* pInstanceCam);

			/**
			* This is a destructor
			*/
			virtual ~MouseRotateCameraThirdPersonListener() {}

			/**
			* This method is called in order to process a message
			* @param message The message this method will have to process
			*/
			virtual void handle ( shared_ptr<CMessage> message );
		};
	};
};

#endif // __CAMERATHIRDPERSONLISTENER_H__
