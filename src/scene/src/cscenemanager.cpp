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

#include <set>
#include <sstream>
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include "../include/ogrecamerawrapper.h"
#include "../include/cameramanager.h"
#include "../include/cscenemanager.h"
#include "../include/cpage.h"
#include "../../time/include/timemodule.h"
#include "../../graphic/include/graphicmodule.h"
#include "../../core/include/sourcefile.h"
#include "../../core/include/message.h"
#include "../../core/messages/include/messenger.h"
#include "../../core/include/messagemodule.h"
#include "../../core/messages/include/listener.h"
#include "../../log/include/logmacros.h"
#include "../include/cameramanager.h"
#include "../include/cscenemanager.h"
#include "../include/cpage.h"


using namespace std;
using namespace boost;
using namespace Gnoll::Core;
using namespace Gnoll::DynamicObject;
using namespace Gnoll::Time;

namespace Gnoll
{
	namespace Scene
	{

		/**
		 * Listener which will update scene content
		 */
		class SceneUpdateListener : public Messages::Listener
		{
			private:

				CSceneManager* sceneManager;

			public:

				/**
				 * This is a constructor
				 */
				SceneUpdateListener(CSceneManager* _sceneManager) : sceneManager(_sceneManager) {}

				/**
				 * This is a destructor
				 */
				virtual ~SceneUpdateListener() {}

				/**
				 * This method is called in order to process a message
				 * @param message The message this method will have to process
				 */
				virtual void handle ( shared_ptr<Message> message )
				{

					if (sceneManager)
					{
						GNOLL_LOG() << "Updating scene...\n";
						sceneManager->update();
						GNOLL_LOG() << " done\n";
					}

				}
		};


		void CSceneManager::setupSky()
		{

			/**
			 * A sky is defined by two attributes :
			 * - skyType which defines the type of sky (eg. dome)
			 * - skyMaterial which is the material used for drawing the sky
			 */
			if (this->hasAttribute( CSceneManager::ATTRIBUTE_SKY_TYPE() ) == true)
			{

				Ogre::Root *ogreRoot = Ogre::Root::getSingletonPtr();

				shared_ptr< Gnoll::DynamicObject::String > skyType = this->getAttribute< Gnoll::DynamicObject::String > ( CSceneManager::ATTRIBUTE_SKY_TYPE() );
				shared_ptr< Gnoll::DynamicObject::String > skyMaterialName( new Gnoll::DynamicObject::String() );

				if (this->hasAttribute( CSceneManager::ATTRIBUTE_SKY_MATERIAL() ) == true)
				{
					skyMaterialName = this->getAttribute< Gnoll::DynamicObject::String > ( CSceneManager::ATTRIBUTE_SKY_MATERIAL() );
				}

				Ogre::SceneManager* sm = ogreRoot->getSceneManager( Gnoll::Graphic::GraphicModule::DEFAULT_OGRE_SCENE_MANAGER_NAME() );


				if (skyType->getValue() == "dome")
				{
					sm->setSkyDome(true, *skyMaterialName);
				}
				else if (skyType->getValue() == "box")
				{
					sm->setSkyBox(true, *skyMaterialName);
				}
				else
				{
					GNOLL_LOG() << "Cannot setup a sky of type " << *skyType << " (unknown).\n";
				}
			}
		}


		void CSceneManager::setupCamera()
		{
			/**
			 * Allocate a camera manager
			 */
			CameraManager* cameraManager = CameraManager::getInstancePtr();


			/*
			 * Load current camera
			 */
			if(this->hasAttribute( CSceneManager::ATTRIBUTE_CURRENT_CAMERA() ) == true)
			{
				// Get back the name of the instance
				shared_ptr< Gnoll::DynamicObject::String > instanceCamera = this->getAttribute< Gnoll::DynamicObject::String > ( CSceneManager::ATTRIBUTE_CURRENT_CAMERA() );

				// Load the camera
				shared_ptr<Camera> currentCamera = cameraManager->load(*instanceCamera);
			} else
			{
				GNOLL_LOG() << "No currentCamera attribute found for scene manager " << this->getInstance() << "\n";
			}

		}


		void CSceneManager::setupLoadedPages()
		{

			shared_ptr< Gnoll::DynamicObject::List > loadedPages = this->getAttribute< Gnoll::DynamicObject::List > ( CSceneManager::ATTRIBUTE_LOADED_PAGES() );
			if (loadedPages->size() == 0)
			{
				loadedPages = shared_ptr< Gnoll::DynamicObject::List > ( new Gnoll::DynamicObject::List() );
				this->setAttribute( CSceneManager::ATTRIBUTE_LOADED_PAGES(), loadedPages);
			}

			GNOLL_LOG() << "Number of loaded pages : " << loadedPages->size() << "\n";
		}


		void CSceneManager::setupMessages()
		{

			MessageModule* messageModule = MessageModule::getInstancePtr();
			TimeModule* timeModule = TimeModule::getInstancePtr();
			Messages::Messenger* messageManager = messageModule->getMessageManager();

			/**
			 * Message type indicating scene manager has to update scene
			 */
			Messages::MessageType updateMsgType( UPDATE_MSG_TYPE() );


			/**
			 * Message for updating scene has no data
			 */
			shared_ptr<boost::any> msgData (new boost::any()) ;
			shared_ptr<Message>  message (new Message(updateMsgType, msgData) );


			/**
			 * Create a timer for updating the scene
			 * Every 1 000 milliseconds a message to update scene content will be sent.
			 * But this will happen in 1 000 millisecondes
			 */
			timeModule->addPeriodicEvent(500, message, 500);


			/**
			 * Create the listener which will update the scene whenever
			 * an UPDATE_SCENE_CONTENT message is received
			 */
			sceneUpdateListener = shared_ptr<Messages::Listener> (new SceneUpdateListener(this));
			messageManager->addListener ( sceneUpdateListener, updateMsgType );

		}


		CSceneManager::CSceneManager(string _instanceName) : DynamicObjectProxy(_instanceName)
		{

			/**
			 * A focusedPage attribute MUST be defined
			 */
			if (this->hasAttribute( CSceneManager::ATTRIBUTE_FOCUSED_PAGE() ) == false)
			{
				GNOLL_LOG() << "No focusedPage attribute found for scene manager " + _instanceName << "\n";
				return;
			}


			/**
			 * First we setup the sky
			 */
			this->setupSky();


			/**
			 * Load and setup the camera
			 */
			this->setupCamera();


			/**
			 * Setup and initialize the loadedPages attribute
			 */
			this->setupLoadedPages();


			/**
			 * Set up focused Page
			 */
			shared_ptr<Gnoll::DynamicObject::String> focusedPageName = this->getAttribute<Gnoll::DynamicObject::String>( CSceneManager::ATTRIBUTE_FOCUSED_PAGE() );
			shared_ptr<Gnoll::DynamicObject::List> loadedPages = this->getAttribute<Gnoll::DynamicObject::List>( CSceneManager::ATTRIBUTE_LOADED_PAGES() );

			this->setupPage(*focusedPageName, loadedPages);

			GNOLL_LOG() << "Number of loadedPages after setup: " << loadedPages->size() << "\n";



			/**
			 * Setup message listeners and such
			 */
			this->setupMessages();
		}


		CSceneManager::~CSceneManager()
		{
			MessageModule* messageModule = MessageModule::getInstancePtr();
			Messages::Messenger* messageManager = messageModule->getMessageManager();

			messageManager->delListener ( sceneUpdateListener, UPDATE_MSG_TYPE() );

			CameraManager::destroy();
			this->deleteAttribute( CSceneManager::ATTRIBUTE_LOADED_PAGES() );
		}


		void CSceneManager::update()
		{

		}

		void CSceneManager::translateVisiblePages(const  Ogre::Vector3 _translate)
		{
			/**
			 * Loaded pages
			 */
			shared_ptr< Gnoll::DynamicObject::List > loadedPages = this->getAttribute< Gnoll::DynamicObject::List > ( CSceneManager::ATTRIBUTE_LOADED_PAGES() );


			/**
			 * Loop through all loaded pages and translate them
			 */
			for( Gnoll::DynamicObject::List::const_iterator it = loadedPages->begin(); it != loadedPages->end(); it++)
			{
				shared_ptr<Gnoll::DynamicObject::String> pageName = dynamic_pointer_cast<Gnoll::DynamicObject::String>(*it);
				CPage page(*pageName);

				if (page.isInitialized())
				{
					Ogre::SceneNode* sceneNode = page.getPageRootNode();
					sceneNode->translate(_translate);
				}
			}

		}

		void CSceneManager::queueJob( shared_ptr<Job> _job)
		{
			m_poolOfThreads.pushJob(_job);
		}


		bool CSceneManager::isPageVisible(const CPage& _page)
		{
			shared_ptr< Gnoll::DynamicObject::String > currentCameraName = this->getAttribute< Gnoll::DynamicObject::String > ( CSceneManager::ATTRIBUTE_CURRENT_CAMERA() );

			return _page.isVisibleFromCamera( currentCameraName );
		}


		void CSceneManager::setupPage( const string _pageInstance, shared_ptr< Gnoll::DynamicObject::List > _loadedPages, const Ogre::Vector3 _offset )
		{
			const char* neighbors[] =   {
											CPage::ATTRIBUTE_NORTH_LINK(),
											CPage::ATTRIBUTE_SOUTH_LINK(),
											CPage::ATTRIBUTE_EAST_LINK(),
											CPage::ATTRIBUTE_WEST_LINK()
										};
			shared_ptr<Gnoll::DynamicObject::String> focusedPageName = this->getAttribute<Gnoll::DynamicObject::String>( CSceneManager::ATTRIBUTE_FOCUSED_PAGE() );

			CPage page(_pageInstance);

			if (page.isInitialized())
			{
				return;
			}

			GNOLL_LOG() << "taking care of page " << _pageInstance << "\n";

			page.init();
			page.initPosition();
			_loadedPages->push_back(shared_ptr<Gnoll::DynamicObject::String>(new Gnoll::DynamicObject::String(_pageInstance)));

			GNOLL_LOG() << "page " << _pageInstance << " initialized\n";
			shared_ptr<Gnoll::DynamicObject::Float> pageSize = page.getAttribute<Float>( CPage::ATTRIBUTE_SIZE() );


			/**
			 * Look at neighbors and translate from neighbors->length / 2.0 in opposite way
			 *
			 */
			for (unsigned int i = 0; i < 4; i++)
			{
				if (page.hasAttribute(neighbors[i]))
				{
					shared_ptr<Gnoll::DynamicObject::String> neighborInstanceName = page.getAttribute<Gnoll::DynamicObject::String>(neighbors[i]);
					GNOLL_LOG() << "Loading neighbor " << string(*neighborInstanceName) << "\n";
					CPage neighbor(*neighborInstanceName);
                    GNOLL_LOG() << "Neighbor " << string(*neighborInstanceName) << " loaded\n";

                    if (strcmp(neighbors[i], CPage::ATTRIBUTE_NORTH_LINK()) == 0)
                    {
                        this->setupPage(*neighborInstanceName, _loadedPages, _offset);

                    } else if (strcmp(neighbors[i], CPage::ATTRIBUTE_SOUTH_LINK()) == 0)
                    {
                        this->setupPage(*neighborInstanceName, _loadedPages, _offset);

                    } else if (strcmp(neighbors[i], CPage::ATTRIBUTE_EAST_LINK()) == 0)
                    {
                        this->setupPage(*neighborInstanceName, _loadedPages, _offset);

                    } else if (strcmp(neighbors[i], CPage::ATTRIBUTE_WEST_LINK()) == 0)
                    {
                        this->setupPage(*neighborInstanceName, _loadedPages, _offset);
                    }
                }
            }
        }

        bool CSceneManager::isCameraInPage(const shared_ptr<Camera>& _camera)
        {
            //
            // Check all loaded page
            //
            /*shared_ptr< Gnoll::DynamicObject::List > loadedPages = this->getAttribute< Gnoll::DynamicObject::List >
              ( CSceneManager::ATTRIBUTE_LOADED_PAGES() );

              for( Gnoll::DynamicObject::List::const_iterator it = loadedPages->begin(); it != loadedPages->end(); it++)
              {
              shared_ptr<Gnoll::DynamicObject::String> pageName = dynamic_pointer_cast<Gnoll::DynamicObject::String>(*it);
              CPage page(*pageName);
              shared_ptr<Gnoll::DynamicObject::Float> pageSize = page->getAttribute<Gnoll::DynamicObject::Float>(CPage::ATTRIBUTE_SIZE());

              if (page.isInitialized())
              {
              Ogre::SceneNode* sceneNode = page.getPageRootNode();
              if(sceneNode->getPosition().x + pageSize < cameraPosition.x &&
              sceneNode->getPosition().x > cameraPosition.x)
              {
              if(sceneNode->getPosition().z + pageSize > cameraPosition.z &&
              sceneNode->getPosition().z < cameraPosition.z)
              {
              return true;
              }
              }
              }
              }*/

            return false;
        }
    }
}


