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


/*-----------------------------CSceneManager-------------------------------*\
|   This is the scene manager                                               |
|                                                                           |
|   Changelog :                                                             |
|               12/05/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../include/cameramanager.h"
#include "../include/cscenemanager.h"
#include "../include/cpage.h"
#include "../../time/include/ctimemodule.h"
#include "../../core/include/sourcefile.h"
#include "../../core/include/cmessage.h"
#include "../../core/include/cmessagetype.h"
#include "../../core/include/cmessagemanager.h"
#include "../../core/include/cmessagemodule.h"
#include "../../core/include/cmessagelistener.h"
#include <OgreRoot.h>
#include <OgreSceneManager.h>

#include <set>
#include "../../graphic/include/cgraphicmodule.h"
#include "../../log/include/clogmodule.h"
#include <sstream>

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
		class SceneUpdateListener : public CMessageListener
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
				virtual void handle ( shared_ptr<CMessage> message )
				{

					if (sceneManager)
					{
						Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "Updating scene..." );
						sceneManager->update();
						Gnoll::Log::CLogModule::getInstancePtr()->logMessage( " done" );
					}

				}
		};



		CSceneManager::CSceneManager(string _instanceName) : CDynamicObjectProxy(_instanceName)
		{
			// Allocate a camera manager
			CameraManager* cameraManager = CameraManager::getInstancePtr();

			if (this->hasAttribute("focusedPage") == false)
			{
				Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "No focusedPage attribute found for CSceneManager " + _instanceName );
				return;
			}


			/**
			 * A sky is defined by two attributes :
			 * - skyType which defines the type of sky (eg. dome)
			 * - skyMaterial which is the material used for drawing the sky
			 */
			if (this->hasAttribute("skyType") == true)
			{

				Ogre::Root *ogreRoot = Ogre::Root::getSingletonPtr();

				shared_ptr< Gnoll::DynamicObject::String > skyType = this->getAttribute< Gnoll::DynamicObject::String > ("skyType");
				shared_ptr< Gnoll::DynamicObject::String > skyMaterialName( new Gnoll::DynamicObject::String() );

				if (this->hasAttribute("skyMaterial") == true)
				{
					skyMaterialName = this->getAttribute< Gnoll::DynamicObject::String > ("skyMaterial");
				}

				Ogre::SceneManager* sm = ogreRoot->getSceneManager("TSM");

				sm->setSkyDome(true, *skyMaterialName);

			}

			/*
			 * Load current camera
			 */
			if(this->hasAttribute("currentCamera") == true)
			{
				// Get back the name of the instance
				shared_ptr< Gnoll::DynamicObject::String > instanceCamera = this->getAttribute< Gnoll::DynamicObject::String > ("currentCamera");

				// Load the camera about him extension
				shared_ptr<Camera> currentCamera = cameraManager->load(*instanceCamera);

				Ogre::Root *ogreRoot = Ogre::Root::getSingletonPtr();
				if (ogreRoot->getRenderSystem()->getCapabilities()->hasCapability(RSC_INFINITE_FAR_PLANE))
				{
					currentCamera->setFarValue(0);
				}

				// Get back the RenderWindow to set the camera
				Ogre::RenderWindow* renderWindow = Gnoll::Graphic::CGraphicModule::getInstancePtr()->getRenderWindow();

				// Set the camera in the viewport [Current camera]
				Viewport* vp = renderWindow->addViewport(currentCamera->getOgreCamera());
				vp->setBackgroundColour(ColourValue(0.5, 1, 0));
			}

			shared_ptr< Gnoll::DynamicObject::List > loadedPages = this->getAttribute< Gnoll::DynamicObject::List > ("loadedPages");
			if (loadedPages->size() == 0)
			{
				loadedPages = shared_ptr< Gnoll::DynamicObject::List > ( new Gnoll::DynamicObject::List() );
				this->setAttribute("loadedPages", loadedPages);
			}


			/**
			 * Set up focused Page
			 */
			shared_ptr<Gnoll::DynamicObject::String> focusedPageName = this->getAttribute<Gnoll::DynamicObject::String>("focusedPage");

			std::ostringstream tmpString;
			tmpString << "SIZE A: " << loadedPages->size() << endl;
			tmpString << "Address of loadedPages : " << loadedPages << endl;
			Gnoll::Log::CLogModule::getInstancePtr()->logMessage( tmpString.str() );
			tmpString.clear();

			this->setupPage(*focusedPageName, loadedPages);

			tmpString << "SIZE B: " << loadedPages->size() << endl;
			Gnoll::Log::CLogModule::getInstancePtr()->logMessage( tmpString.str() );

			CMessageModule* messageModule = CMessageModule::getInstancePtr();
			CTimeModule* timeModule = CTimeModule::getInstancePtr();
			CMessageManager* messageManager = messageModule->getMessageManager();
			CMessageType updateMsgType("UPDATE_SCENE_CONTENT");

			shared_ptr<boost::any> msgData (new boost::any()) ;
			shared_ptr<CMessage>  message (new CMessage(updateMsgType, msgData) );

			sceneUpdateListener = shared_ptr<CMessageListener> (new SceneUpdateListener(this));

			// Every 1 000 milliseconds a message to update scene content will be sent. But this will happen in 1 000 millisecondes
			timeModule->addPeriodicEvent(1000, message, 1000);

			messageManager->addListener ( sceneUpdateListener, updateMsgType );

		}


		CSceneManager::~CSceneManager()
		{
			CMessageModule* messageModule = CMessageModule::getInstancePtr();
			CMessageManager* messageManager = messageModule->getMessageManager();

			CMessageType updateMsgType("UPDATE_SCENE_CONTENT");

			messageManager->delListener ( sceneUpdateListener, updateMsgType );

			CameraManager::destroy();
			this->deleteAttribute("loadedPages");
		}


		void CSceneManager::update()
		{

			/**
			 * Used to keep track of visited pages
			 */
			set<string> visitedPages;

			/**
			 * List of currently visible pages
			 */
			shared_ptr< Gnoll::DynamicObject::List > visiblePages = shared_ptr< Gnoll::DynamicObject::List > ( new Gnoll::DynamicObject::List() );

			/**
			 * Pages loaded
			 */
			shared_ptr< Gnoll::DynamicObject::List > loadedPages = this->getAttribute< Gnoll::DynamicObject::List > ("loadedPages");

			std::ostringstream tmpString;
			tmpString << "Address of loadedPages : " << loadedPages << endl;
			tmpString << "Size of loadedPages : " << loadedPages->size() << endl;
			Gnoll::Log::CLogModule::getInstancePtr()->logMessage( tmpString.str() );
			tmpString.clear();

			/**
			 * Loop through all loaded pages and check if they are still visible
			 */
			for( Gnoll::DynamicObject::List::const_iterator it = loadedPages->begin(); it != loadedPages->end(); it++)
			{
				if (shared_ptr<Gnoll::DynamicObject::String> pageName = dynamic_pointer_cast<Gnoll::DynamicObject::String>(*it))
				{
					tmpString << "1 - Loaded page : " << *pageName << "\n";
					Gnoll::Log::CLogModule::getInstancePtr()->logMessage( tmpString.str() );
					tmpString.clear();

					if (visitedPages.find(*pageName) == visitedPages.end())
					{
						CPage page(*pageName);
						visitedPages.insert(*pageName);

						tmpString << "  1 - checking visibility for " <<  *pageName << endl;
						Gnoll::Log::CLogModule::getInstancePtr()->logMessage( tmpString.str() );
						tmpString.clear();

						if ( this->isPageVisible(page) )
						{
							visiblePages->push_back( pageName );

							tmpString << "    a - " <<  *pageName << " is visible "<< endl;
							if ( page.isInitialized() == false )
							{
								tmpString << "      b - " <<  *pageName << " is not initialized yet. It's going to be initialized "<< endl;
								page.init();
							} else
							{
								tmpString << "      c - " <<  *pageName << " has already been initialized"<< endl;
							}
							Gnoll::Log::CLogModule::getInstancePtr()->logMessage( tmpString.str() );
							tmpString.clear();
						}

						const char* neighbors[] = {"northLink", "southLink", "eastLink", "westLink"};

						for (unsigned int i = 0; i < 4; i++)
						{
							Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "  Checking neighbor : " + string( neighbors[i] ) );
							if (page.hasAttribute(neighbors[i]))
							{
								shared_ptr<Gnoll::DynamicObject::String> neighborInstanceName = page.getAttribute<Gnoll::DynamicObject::String>(neighbors[i]);

								Gnoll::Log::CLogModule::getInstancePtr()->logMessage( string("    neighbor found : ") + string(*neighborInstanceName) );
								if (visitedPages.find(*neighborInstanceName) == visitedPages.end())
								{
									Gnoll::Log::CLogModule::getInstancePtr()->logMessage( string("      appending : ") + string(*neighborInstanceName) );
									loadedPages->push_back( neighborInstanceName );
								}
							}
						}
					}
				}
			}

			this->setAttribute("loadedPages", visiblePages);

		}


		void CSceneManager::queueJob( shared_ptr<CJob> _job)
		{
			m_poolOfThreads.pushJob(_job);
		}


		bool CSceneManager::isPageVisible(const CPage& _page)
		{
			shared_ptr< Gnoll::DynamicObject::String > currentCameraName = this->getAttribute< Gnoll::DynamicObject::String > ("currentCamera");

			return _page.isVisibleFromCamera( currentCameraName );
			//return true;
		}


		void CSceneManager::setupPage( const string _pageInstance, shared_ptr< Gnoll::DynamicObject::List > _loadedPages, const Ogre::Vector3 _offset )
		{
			const char* neighbors[] = {"northLink", "southLink", "eastLink", "westLink"};
			shared_ptr<Gnoll::DynamicObject::String> focusedPageName = this->getAttribute<Gnoll::DynamicObject::String>("focusedPage");

			CPage page(_pageInstance);

			if ( ((string)(*focusedPageName) != _pageInstance) && (this->isPageVisible(page) == false) )
			{
				return;
			}

			Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "taking care of page " + _pageInstance );

			page.init();
			_loadedPages->push_back(shared_ptr<Gnoll::DynamicObject::String>(new Gnoll::DynamicObject::String(_pageInstance)));

			Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "page " + _pageInstance + " initialized" );
			shared_ptr<Gnoll::DynamicObject::Float> pageSize = page.getAttribute<Float>("size");


			/**
			 * Look at neighbors and translate from neighbors->length / 2.0 in opposite way
			 *
			 */
			for (unsigned int i = 0; i < 4; i++)
			{
				if (page.hasAttribute(neighbors[i]))
				{
					shared_ptr<Gnoll::DynamicObject::String> neighborInstanceName = page.getAttribute<Gnoll::DynamicObject::String>(neighbors[i]);
					Gnoll::Log::CLogModule::getInstancePtr()->logMessage( string("Loading neighbor ") + string(*neighborInstanceName) );
					CPage neighbor(*neighborInstanceName);
					Gnoll::Log::CLogModule::getInstancePtr()->logMessage( string("Neighbor ") + string(*neighborInstanceName) + " loaded" );

					if (this->isPageVisible(neighbor))
					{
						Ogre::Vector3 neighborOffset(0.0, 0.0, 0.0);

						if (strcmp(neighbors[i], "northLink") == 0)
						{
							neighborOffset = Ogre::Vector3(0.0, 0.0, *pageSize);
							this->setupPage(*neighborInstanceName, _loadedPages, _offset + neighborOffset );

						} else if (strcmp(neighbors[i], "southLink") == 0)
						{
							neighborOffset = Ogre::Vector3(0.0, 0.0, -(*pageSize));
							this->setupPage(*neighborInstanceName, _loadedPages, _offset + neighborOffset );

						} else if (strcmp(neighbors[i], "eastLink") == 0)
						{
							neighborOffset = Ogre::Vector3( -(*pageSize), 0.0, 0.0 );
							this->setupPage(*neighborInstanceName, _loadedPages, _offset + neighborOffset );

						} else if (strcmp(neighbors[i], "westLink") == 0)
						{
							neighborOffset = Ogre::Vector3( *pageSize, 0.0, 0.0 );
							this->setupPage(*neighborInstanceName, _loadedPages, _offset + neighborOffset );
						}

						Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "asking for neighbor page root node" );
						Ogre::SceneNode* neighborRootNode = neighbor.getPageRootNode();

						std::ostringstream tmpString;
						tmpString << "neighbor page root node " << neighborRootNode;
						Gnoll::Log::CLogModule::getInstancePtr()->logMessage( tmpString.str() );
						neighborRootNode->translate( _offset + neighborOffset, Ogre::Node::TS_LOCAL);

					} else {

						Gnoll::Log::CLogModule::getInstancePtr()->logMessage( string("Neighbor ") + string(*neighborInstanceName) + " not visible from " + _pageInstance );
					}
				}
			}

		}
	}
}


