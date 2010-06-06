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

/****************************** Summary ************************************
 * This is an abstract class of camera. We can't directly use this camera  *
 * but it provides some services:                                          *
 * 	- Management of listener                                             *
 * 	- Management of target                                               *
 * 	- Set up of serveral parameters: up, eye, at, far, near, fov,        *
 * 	  movement factor                                                    *
 * This camera can change the orientation and the position too.            *
 ***************************************************************************/

#ifndef INCLUDED_CAMERA
#define INCLUDED_CAMERA

#include <glibmm/ustring.h>
#include <OgreSceneNode.h>
#include <map>
#include "camerawrapper.h"
#include "../../dynamicobject/include/dynamicobjectproxy.h"

namespace Gnoll
{
	namespace Core
	{
		namespace Messages
		{
			class MessageType;
		}
	}

	namespace Scene
	{
		class CMessageListenerCamera;

		class Camera : public Gnoll::DynamicObject::DynamicObjectProxy
		{
			private:
				typedef std::pair<shared_ptr<CMessageListenerCamera>, shared_ptr<Messages::MessageType> > PairsListener;
				typedef std::queue<shared_ptr<PairsListener> > QueueListener;

				/*
				 * This is the queue of cameras listener
				 */
				QueueListener queueListener;

				/*
				 * This is the name of the target (load from scenemanager, "" means no target
				 */
				Glib::ustring target;

				/*
				 * Operator to copy a camera
				 * @param copy This is the camera to copy
				 */
				virtual Camera& operator=(const Camera& copy);

				/*
				 * Copy constructor
				 * @param copy This is the camera to copy
				 */
				explicit Camera(const Camera& copy);

			protected:
				/*
				 * This is a pointer to a Camera Wrapper
				 */
				shared_ptr<CameraWrapper> cameraWrapper;

				/*
				 * This is a map for the speed movement of camera
				 */
				std::map<std::string, float> mapMovement;

				/*
				 * Default constructor, it initializes the camera with default settings :
				 * position (0, 0, 0), direction (0, 0, 1), up (0, 1, 0), near 0, far is 200,
				 * fov PI/4 OR get back ancient configuration with dynamic objet if exists
				 * @param instanceName This is the instance name of the camera, it will be use for the
				 * 					   instance name of the Ogre Camera
				 */
				explicit Camera(const Glib::ustring& instanceName, shared_ptr<CameraWrapper> wrapper);

				/*
				 * Helper method for setTarget()
				 * @param (see setTarget)
				 */
				void setTargetHelper(const Glib::ustring& target);

			public:

				/**
				 * Returns attribute name "name".<br/>
				 * @return The attribute name "name"
				 */
				inline static const char * ATTRIBUTE_NAME() {return "name";}

				/**
				 * Returns attribute name "far".<br/>
				 * @return The attribute name "far"
				 */
				inline static const char * ATTRIBUTE_FAR() {return "far";}

				/**
				 * Returns attribute name "near".<br/>
				 * @return The attribute name "near"
				 */
				inline static const char * ATTRIBUTE_NEAR() {return "near";}

				/**
				 * Returns attribute name "fov".<br/>
				 * @return The attribute name "fov"
				 */
				inline static const char * ATTRIBUTE_FOV() {return "fov";}

				/**
				 * Returns attribute name "position".<br/>
				 * @return The attribute name "position"
				 */
				inline static const char * ATTRIBUTE_POSITION() {return "position";}

				/**
				 * Returns attribute name "direction".<br/>
				 * @return The attribute name "direction"
				 */
				inline static const char * ATTRIBUTE_DIRECTION() {return "direction";}

				/**
				 * Returns attribute name "target".<br/>
				 * @return The attribute name "target"
				 */
				inline static const char * ATTRIBUTE_TARGET() {return "target";}

				/*
				 * Destructior
				 */
				virtual ~Camera();

				/*
				 * Accessor to get the target name
				 * @return the name of the target in the scenemanager
				 */
				const Glib::ustring& getTargetName() const;

				/*
				 * Accessor to get the target
				 * @return a pointer to the SceneNode
				 */
				Ogre::SceneNode* getTarget() const;

				/*
				 * Accessor to get the Camera Wrapper
				 * @return the Camera Wrapper
				 */
				CameraWrapper* getCameraWrapper();

				/*
				 * Settor to set the new target of the camera
				 * @param target This is the name in the scenemanager of the target
				 * @param autofocus This specify if the camera follow the object (orientation)
				 */
				virtual void setTarget(const Glib::ustring& target, bool autofocus = true);

				/*
				 * Put a new listener in the queue of the camera
				 * @param listener This is the listener to put on the queue
				 * @param type This is the type of message associate to the listener
				 */
				void enqueueListener(shared_ptr<CMessageListenerCamera> listener, shared_ptr<Gnoll::Core::Messages::MessageType> type);

				/*
				 * This method provide the configuration of each speed movement
				 * element
				 * @return The map between the name of the movement with the value
				 */
				const std::map<std::string, float>& getMovementInformation();

				/*
				 * This method set up the configuration of each speed movement
				 * element
				 * @param The map between the name of the movement with the value
				 */
				void setMovementInformation(const std::map<std::string, float>& mapConfiguration);

				/*
				 * This method is call each frame
				 * @param time This is the time between 2 frames
				 */
				virtual void update(float time);
		};
	};
};

#endif
