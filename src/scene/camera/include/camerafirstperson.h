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

#ifndef INCLUDED_CAMERAFIRSTPERSON
#define INCLUDED_CAMERAFIRSTPERSON

#include "camera.h"

namespace Gnoll
{
	namespace Scene
	{
		class CameraFirstPerson : public Camera
		{
			public:
				/**
				 * Returns attribute name "head_position".<br/>
				 * @return The attribute name "head_position"
				 */
				inline static const char * ATTRIBUTE_HEADPOSITION() {return "head_position";}

				/*
				 * Default constructor, if no head positon is defined, the head position will be set
				 *                      to the target position
				 * @param instanceName This is the instance name of the camera, it will be use for the
				 * 					   instance name of the Ogre Camera
				 */
				explicit CameraFirstPerson(const Glib::ustring& instanceName, shared_ptr<CameraWrapper> wrapper);

				/*
				 * Destructior
				 */
				virtual ~CameraFirstPerson();

				/*
				 * This method is call each frame
				 * @param time This is the time between 2 frames
				 */
				virtual void update(float time);

				/**
				 * This set the offset between the target position and the real head position
				 * @param offset This is the offset, it's an offset in a 3 dimensions due to the
				 *               non-alignement of the head and target position
				 */
				void setHeadPosition(const Ogre::Vector3& value);

				/**
				 * This is the gettor of the head positon offset
				 * @param value The offset in 3 dimensions between the position of the target and the
				 *              real head position
				 */
				const Ogre::Vector3* getHeadPosition();

			private:
				/*
				 * Parameters of the camera
				 */
				Ogre::Vector3* headPosition;
		};
	};
};

#endif
