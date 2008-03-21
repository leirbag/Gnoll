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



#include "../include/cscenemanager.h"
#include "../include/cpage.h"
#include "../../time/include/ctimemodule.h"
#include "../../core/include/cmessage.h"
#include "../../core/include/cmessagetype.h"
#include "../../core/include/cmessagemanager.h"
#include "../../core/include/cmessagemodule.h"
#include "../../core/include/cmessagelistener.h"

#include <set>

#include <iostream>


using namespace std;
using namespace boost;
using namespace Gnoll::Core;
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
						cout << "Updating scene..." << endl;
						sceneManager->update();
						cout << " done" << endl;
					}

				}
		};



		CSceneManager::CSceneManager(string _instanceName) : CPersistentObjectProxy(_instanceName)
		{

			if (this->hasAttribute("focusedPage") == false)
			{
				cout << "No focusedPage attribute found for CSceneManager " << _instanceName << endl;
				return;
			}


			shared_ptr< Gnoll::Core::List > loadedPages = this->getAttribute< Gnoll::Core::List > ("loadedPages");
			if (loadedPages->size() == 0)
			{
				loadedPages = shared_ptr< Gnoll::Core::List > ( new Gnoll::Core::List() );
				this->setAttribute("loadedPages", loadedPages);
			}


			/**
			 * Set up focused Page
			 */
			shared_ptr<Gnoll::Core::String> focusedPageName = this->getAttribute<Gnoll::Core::String>("focusedPage");
			cout << "SIZE A: " << loadedPages->size() << endl;
			cout << "Address of loadedPages : " << loadedPages << endl;
			this->setupPage(*focusedPageName, loadedPages);
			cout << "SIZE B: " << loadedPages->size() << endl;

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
			shared_ptr< Gnoll::Core::List > visiblePages = shared_ptr< Gnoll::Core::List > ( new Gnoll::Core::List() );

			/**
			 * Pages loaded
			 */
			shared_ptr< Gnoll::Core::List > loadedPages = this->getAttribute< Gnoll::Core::List > ("loadedPages");

			cout << "Address of loadedPages : " << loadedPages << endl;
			cout << "Size of loadedPages : " << loadedPages->size() << endl;

			/**
			 * Loop through all loaded pages and check if they are still visible
			 */
			for( Gnoll::Core::List::const_iterator it = loadedPages->begin(); it != loadedPages->end(); it++)
			{
				if (shared_ptr<Gnoll::Core::String> pageName = dynamic_pointer_cast<Gnoll::Core::String>(*it))
				{
					cout << "1 - Loaded page : " << *pageName << endl;

					if (visitedPages.find(*pageName) == visitedPages.end())
					{
						CPage page(*pageName);
						visitedPages.insert(*pageName);
						cout << "  1 - checking visibility" << endl;
						if ( this->isPageVisible(page) )
						{
							visiblePages->push_back( pageName );
						}

						const char* neighbors[] = {"northLink", "southLink", "eastLink", "westLink"};

						for (unsigned int i = 0; i < 4; i++)
						{
							cout << "  Checking neighbor : " << neighbors[i] << endl;
							if (page.hasAttribute(neighbors[i]))
							{
								shared_ptr<Gnoll::Core::String> neighborInstanceName = page.getAttribute<Gnoll::Core::String>(neighbors[i]);

								cout << "    neighbor found : " << neighborInstanceName << endl;
								if (visitedPages.find(*neighborInstanceName) == visitedPages.end())
								{
									cout << "      appending : " << neighborInstanceName << endl;
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
			shared_ptr< Gnoll::Core::String > currentCameraName = this->getAttribute< Gnoll::Core::String > ("currentCamera");

			// XXX TODO
			// Get page root node coordinates
			// get page length
			// Compute AABB box
			// Check with the current camera if the AABB box is inside the view frustrum
			return true;
		}


		void CSceneManager::setupPage( const string _pageInstance, shared_ptr< Gnoll::Core::List > _loadedPages, const Ogre::Vector3 _offset )
		{
			char* neighbors[] = {"northLink", "southLink", "eastLink", "westLink"};

			CPage page(_pageInstance);

			if (this->isPageVisible(page) == false)
			{
				return;
			}

			cout << "taking care of page " << _pageInstance << endl;

			page.init();
			_loadedPages->push_back(shared_ptr<Gnoll::Core::String>(new Gnoll::Core::String(_pageInstance)));

			cout << "page " << _pageInstance << "initialized" << endl;
			shared_ptr<Gnoll::Core::Float> pageSize = page.getAttribute<Float>("size");


			/**
			 * Look at neighbors and translate from neighbors->length / 2.0 in opposite way
			 *
			 */
			for (unsigned int i = 0; i < 4; i++)
			{
				if (page.hasAttribute(neighbors[i]))
				{
					shared_ptr<Gnoll::Core::String> neighborInstanceName = page.getAttribute<Gnoll::Core::String>(neighbors[i]);
					cout << "Loading neighbor " << *neighborInstanceName << endl;
					CPage neighbor(*neighborInstanceName);
					cout << "Neighbor " << *neighborInstanceName << " loaded" << endl;

					if (this->isPageVisible(neighbor))
					{
						Ogre::Vector3 neighborOffset(0.0, 0.0, 0.0);

						if (strcmp(neighbors[i], "northLink") == 0)
						{
							neighborOffset = Ogre::Vector3(0.0, 0.0, *pageSize) / 2.0f;
							this->setupPage(*neighborInstanceName, _loadedPages, _offset + neighborOffset );

						} else if (strcmp(neighbors[i], "southLink") == 0)
						{
							neighborOffset = Ogre::Vector3(0.0, 0.0, -(*pageSize)) / 2.0f;
							this->setupPage(*neighborInstanceName, _loadedPages, _offset + neighborOffset );

						} else if (strcmp(neighbors[i], "eastLink") == 0)
						{
							neighborOffset = Ogre::Vector3( -(*pageSize), 0.0, 0.0 ) / 2.0f;
							this->setupPage(*neighborInstanceName, _loadedPages, _offset + neighborOffset );

						} else if (strcmp(neighbors[i], "westLink") == 0)
						{
							neighborOffset = Ogre::Vector3( *pageSize, 0.0, 0.0 ) / 2.0f;
							this->setupPage(*neighborInstanceName, _loadedPages, _offset + neighborOffset );
						}

						cout << "asking for neighbor page root node" << endl;
						Ogre::SceneNode* neighborRootNode = neighbor.getPageRootNode();
						cout << "neighbor page root node " << neighborRootNode << endl;
						neighborRootNode->translate( _offset + neighborOffset, Ogre::Node::TS_LOCAL);
					}
				}
			}

		}
	}
}


