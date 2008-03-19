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


		struct pageComparison
		{
			bool operator()(const string page1Name, const string page2Name) const
			{
				return strcmp(page1Name.c_str(), page2Name.c_str()) < 0;
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
			if (loadedPages.get() == 0)
			{
				loadedPages = shared_ptr< Gnoll::Core::List > ( new Gnoll::Core::List() );
				this->setAttribute("loadedPages", loadedPages);
			}

			/**
			 * Set up focused Page
			 */
			shared_ptr<Gnoll::Core::String> focusedPageName = this->getAttribute<Gnoll::Core::String>("focusedPage");
			this->setupPage(*focusedPageName, loadedPages);

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
			// Parcourt des sets de pages pour verifier qu'ils soient tous visibles, ainsi que leurs voisins [tant que les voisins sont visibles].
			shared_ptr<Gnoll::Core::String> focusedPageName = this->getAttribute<Gnoll::Core::String>("focusedPage");
			
			if (focusedPageName.get())
			{

				/**
				 * Currently focused page
				 */
				CPage focusedPage(*focusedPageName);

				/**
				 * Used to keep track of visited pages
				 */
				set<string, pageComparison> visitedPages;
				shared_ptr< Gnoll::Core::List > visiblePages = shared_ptr< Gnoll::Core::List > ( new Gnoll::Core::List() );

				/**
				 * Pages loaded
				 */
				shared_ptr< Gnoll::Core::List > loadedPages = this->getAttribute< Gnoll::Core::List > ("loadedPages");


				if ( isPageVisible(focusedPage) )
				{
					visitedPages.insert(*focusedPageName);
					visiblePages->push_back( focusedPageName );
				}

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
						}
					}
				}


				/**
				 * Loop through all neighbors of visiblePages and check if they are also visible
				 */

				set<string, pageComparison> visitedPagesForNeighbors;
				shared_ptr< Gnoll::Core::List > newPages = shared_ptr< Gnoll::Core::List > ( new Gnoll::Core::List() );
				for( Gnoll::Core::List::const_iterator it = visiblePages->begin(); it != visiblePages->end(); it++)
				{
					if (shared_ptr<Gnoll::Core::String> pageName = dynamic_pointer_cast<Gnoll::Core::String>(*it))
					{
						cout << "Working on " << *pageName << endl;
						if (visitedPagesForNeighbors.find(*pageName) == visitedPagesForNeighbors.end())
						{

							CPage page(*pageName);
							const char* neighbors[] = {"northLink", "southLink", "eastLink", "westLink"};

							visitedPagesForNeighbors.insert(*pageName);


							for (unsigned int i = 0; i < 4; i++)
							{
								cout << "  Checking neighbor : " << neighbors[i] << endl;
								if (page.hasAttribute(neighbors[i]))
								{
									shared_ptr<Gnoll::Core::String> neighborInstanceName = page.getAttribute<Gnoll::Core::String>(neighbors[i]);

									cout << "    neighbor found : " << neighborInstanceName << endl;
									if ( (visitedPagesForNeighbors.find(*neighborInstanceName) == visitedPagesForNeighbors.end()) &&
									     (visitedPages.find(*neighborInstanceName) == visitedPages.end()) )
									{
										cout << "    Loading neighbor " << *neighborInstanceName << endl;
										CPage neighbor(*neighborInstanceName);
										cout << "    Neighbor " << *neighborInstanceName << " loaded" << endl;

										visitedPagesForNeighbors.insert(*neighborInstanceName);

										if (this->isPageVisible(neighbor))
										{
											newPages->push_back( neighborInstanceName );
										}
									}
								}
							}
						}
					}
				}


				visiblePages->merge(*newPages);
				this->setAttribute("loadedPages", visiblePages);

			} else
			{
				cout << "No focusedPage attribute defined for CSceneManager " << this->getInstance() << endl;
			}

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


