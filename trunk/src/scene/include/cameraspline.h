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


/*-------------------------------cameraspline------------------------------*\
|   This is a spline camera                                                 |
|                                                                           |
|   Changelog :                                                             |
|               04/12/2008 - Gabriel - Initial release                      |
\*-------------------------------------------------------------------------*/

#ifndef __CAMERASPLINE_H__
#define __CAMERASPLINE_H__

#include <OgreVector3.h>
#include <OgreSimpleSpline.h>
#include <OgreAnimation.h>
#include <OgreAnimationState.h>
#include "camera.h"

namespace Gnoll
{

	namespace Scene
	{
		struct cameraspline_i;

		class CameraSpline : public Camera
		{
		private:
			cameraspline_i *m_this;

			/*
			 * Copy constructor
			 * @param copy This is the camera to copy
			 */
			explicit CameraSpline(const CameraSpline& copy);

			/*
			 * Operator to copy a camera
			 * @param copy This is the camera to copy
			 */
			virtual CameraSpline& operator=(const CameraSpline& copy);

		public:
			/*
			 * Default constructor
			 * @param instanceName This is the instance name of the camera, it will be use for the
			 * 					   instance name of the Ogre Camera
			 */
			explicit CameraSpline(const Glib::ustring& instanceName);

			/*
			 * Destructior
			 */
			virtual ~CameraSpline();

			/**
			 * This update the View.
			 * @param time This is the time between 2 frames
			 */
			virtual void update(float time);

			/**
			 * This add a point at a frame of the animation
			 * @param vec3 position of the camera
			 * @param frame specify the frame number
			 */
			void addPoint(const Ogre::Vector3& vec3, unsigned long frame);

			/**
			 * This return the position of camera at a frame number
			 * @param frame specify the frame number
			 * @return position of the camera
			 */
			const Ogre::Vector3* getPoint(unsigned long frame);

			/**
			 * This specify the length (in key frame) of the animation
			 * @param length number of key frame
			 */
			void setLength(unsigned long length);

			/**
			 * This return the number of key frame
			 * @return number of key frame
			 */
			unsigned long getLength();

			/**
			 * This start the animation of camera
			 */
			void start();

			/**
			 * This stop the animation of camera
			 */
			void stop();

			/**
			 * This set the status "loop" or not to the animation
			 * @param loop specify if loop or not
			 */
			void setLoop(bool loop);

			/**
			 * This return the current key frame
			 * @return the current key frame
			 */
			float getCurrentKeyFrame();
		};
	};
};

#endif // __CAMERASPLINE_H__
