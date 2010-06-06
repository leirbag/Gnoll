/***************************************************************************
 *   Copyright (C) 2007 by Paf                                             *
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

#ifndef __CSCENEMANAGER_H__
#define __CSCENEMANAGER_H__

#include <boost/shared_ptr.hpp>
#include <libxml++/libxml++.h>
#include <glibmm/ustring.h>

#include "../../dynamicobject/include/dynamicobjectproxy.h"
#include "../../core/include/cpoolthreads.h"
#include "../../core/messages/include/messagetype.h"
#include "../include/cpage.h"
#include "../../core/messages/include/listener.h"

#include "../../dynamicobject/include/string.h"
#include "../../dynamicobject/include/float.h"
#include "../../dynamicobject/include/set.h"

using namespace std;
using namespace boost;
using namespace Gnoll::Core;
using namespace Gnoll::DynamicObject;

namespace Gnoll
{
	namespace Scene
	{

		/**
		 * This class manages the scene content.<br/>
		 * The world has been divided in chunks, and only the visible ones will be <br/>
		 * displayed.
		 */
		class CSceneManager : public DynamicObjectProxy
		{

			private:

				/**
				 * Pool of threads used for all sort of jobs
				 */
				CPoolThreads m_poolOfThreads;


				/**
				 * Listener which will update the scene manager
				 */
				shared_ptr<Messages::Listener> sceneUpdateListener;


				/**
				 * Setup the sky to be displayed, if any has been configured <br/>
				 * A sky is defined by two attributes : <br/>
				 * - skyType which defines the type of sky (eg. dome) <br/>
				 * - skyMaterial which is the material used for drawing the sky <br/>
				 */
				void setupSky();


				/**
				 * Setup the camera used for displaying the scene
				 */
				void setupCamera();


				/**
				 * Setup loadedPages attribute
				 */
				void setupLoadedPages();


				/**
				 * Setup message listeners and such
				 */
				void setupMessages();


				/**
				 * Translate all visible pages by vector _translate
				 * @param _translate amount to translate pages of
				 */
				void translateVisiblePages(const  Ogre::Vector3 _translate);


			public:


				/**
				 * The sky type defines the type of sky (eg. dome)
				 * @return Attribute name for the sky type attribute
				 */
				inline static const char * ATTRIBUTE_SKY_TYPE() {return "skyType";}


				/**
				 * The sky material defines which material is used for drawing the sky
				 * @return Attribute name for the sky material attribute
				 */
				inline static const char * ATTRIBUTE_SKY_MATERIAL() {return "skyMaterial";}


				/**
				 * The current camera is the camera having currently the focus
				 * @return Attribute name for the current camera attribute
				 */
				inline static const char * ATTRIBUTE_CURRENT_CAMERA() {return "currentCamera";}


				/**
				 * The loaded pages attribute is the list of all currently loaded pages
				 * @return Attribute name for the loaded pages attribute
				 */
				inline static const char * ATTRIBUTE_LOADED_PAGES() {return "loadedPages";}


				/**
				 * The focused page is the page where the camera is currently located.<br/>
				 * This is also the page currently at the center of the universe
				 * @return Attribute name for the focused page attribute
				 */
				inline static const char * ATTRIBUTE_FOCUSED_PAGE() {return "focusedPage";}


				/**
				 * String for message type signaling the scene should be updated
				 */
				static const char * UPDATE_MSG_TYPE_STR() {return "UPDATE_SCENE_CONTENT";};


				/**
				 * Message type signaling the scene should be updated
				 */
				static const Messages::MessageType UPDATE_MSG_TYPE() {return Core::Messages::MessageType( UPDATE_MSG_TYPE_STR() );};

				/**
				 * This is a constructor
				 */
				CSceneManager(string _instanceName);


				/**
				 * This is a destructor
				 */
				~CSceneManager();


				/**
				 * Update scene module <br/>
				 * It will loaded visible or pretty soon visible pages and unload <br/>
				 * those that are not used anymore
				 */
				void update();


				/**
				 * Enqueue a job in the pool of threads
				 */
				void queueJob( shared_ptr<CJob> _job);


				/**
				 * Check if a CPage object is within the camera frustrum
				 * @param _page CPage object to check
				 * @return Whether or not that page is visible
				 */
				bool isPageVisible(const CPage& _page);

				/**
				 * Check if the camera is in a page
				 * @param _camera The camera to check
				 * @return True if the camera is in a page else False
				 */
				bool isCameraInPage(const shared_ptr<Camera>& _camera);


				/**
				 * Set up a page (position, etc.)
				 * @param _pageInstance Page Instance to setup
				 * @param _loadedPages List of loaded pages
				 * @param _offset Page offset
				 */
				void setupPage( const string _pageInstance, shared_ptr< Gnoll::DynamicObject::List > _loadedPages, const Ogre::Vector3 _offset = Ogre::Vector3());
		};

	}
}

#endif // __CSCENEMANAGER_H__
