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


/*--------------------------------CPage------------------------------------*\
|   This is a page                                                          |
|                                                                           |
|   Changelog :                                                             |
|               12/04/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include "../../graphic/include/cgraphicmodule.h"
#include "../../core/include/persistentobjectmanager.h"
#include "../../core/include/float.h"
#include "../include/cpage.h"
#include "../include/ipagerenderer.h"
#include <OgreCamera.h>

#include <iostream>

using namespace std;
using namespace boost;
using namespace Gnoll::Graphic;

namespace Gnoll
{
	namespace Scene
	{


		CPage::CPage(string _instanceName): CPersistentObjectProxy(_instanceName)
		{

		}

		void CPage::init()
		{

			cout << "!!!!!!!!!!!!!!!! INIT OF PAGE " << this->getInstance() <<  endl;

			Ogre::SceneManager* sm = CGraphicModule::getInstancePtr()->getSceneManager();
			SceneNode* root = sm->getRootSceneNode();

			// Creating root page node
			root->createChildSceneNode( this->getInstance() );

			if (this->hasAttribute("PageRenderer"))
			{
				cout << "From " << this->getInstance() << " // Initializing PageRenderer" << endl;
				shared_ptr<IPageRenderer> pRenderer = this->getAttribute<IPageRenderer>("PageRenderer");
				pRenderer->init(this);
			}

		}

		void CPage::unInit()
		{
			cout << "!!!!!!!!!!!!!!!! DE-INIT OF PAGE " << this->getInstance() <<  endl;

			/**
			 * The way to check if a page has been initialized, is to check if the Page Root Node has
			 * been created.
			 */
			if (this->getPageRootNode())
			{
				Ogre::SceneManager* sm = CGraphicModule::getInstancePtr()->getSceneManager();


				if (this->hasAttribute("PageRenderer"))
				{
					shared_ptr<IPageRenderer> pRenderer = this->getAttribute<IPageRenderer>("PageRenderer");
					pRenderer->exit();
				}

				SceneNode* node = sm->getSceneNode( this->getInstance() );
				node->removeAndDestroyAllChildren();

				//SceneNode* root = sm->getRootSceneNode();
				//root->removeAndDestroyChild(this->getInstance());
			}
		}


		CPage::~CPage()
		{
		}


		Ogre::SceneNode * CPage::getPageRootNode() const
		{
			Ogre::SceneManager* sm = CGraphicModule::getInstancePtr()->getSceneManager();
			SceneNode* node = NULL;

			if ( sm->hasSceneNode(this->getInstance()) )
			{
				node = sm->getSceneNode( this->getInstance() );
			}

			return node;
		}

		bool CPage::isInitialized()
		{
			bool result = false;

			if (this->getPageRootNode())
			{
				result = true;
			}

			return result;
		}


		bool CPage::isVisibleFromCamera(shared_ptr< Gnoll::Core::String > _cameraName) const
		{
			// XXX TODO
			// Get page root node coordinates
			// get page length
			// Compute AABB box
			// Check with the current camera if the AABB box is inside the view frustrum

			cout << "       Checking visibility from camera : " << *_cameraName << endl;
			Ogre::Camera* ogreCamera = CGraphicModule::getInstancePtr()->getSceneManager()->getCamera(*_cameraName);
			Ogre::SceneNode* rootNode = this->getPageRootNode();

			bool result = false;

			if (rootNode)
			{
				cout << "        root node available (" <<  this->getInstance() << ")" << endl;
				cout << "         Number of children : " << rootNode->numChildren() << endl;

				//const Ogre::AxisAlignedBox aabb = rootNode->_getWorldAABB();
				Ogre::AxisAlignedBox aabb;


				//////////////////////////////////////////////////

				/**
				 * The OctreeSceneManager doesn't take children nodes in account
				 * when computing AABB box even though this is taken in account by
				 * Ogre::SceneNode.
				 * So this is computed by hand like does Ogre::SceneNode.
				 */


				// Update bounds from own attached objects
				SceneNode::ObjectIterator oi = rootNode->getAttachedObjectIterator();
				while (oi.hasMoreElements())
				{
					MovableObject *m = oi.getNext();

					// Merge world bounds of each object
					aabb.merge(m->getWorldBoundingBox(true));
				}

				// Merge with children
				Node::ChildNodeIterator oit = rootNode->getChildIterator();
				while (oit.hasMoreElements())
				{
					SceneNode* sceneChild = static_cast<SceneNode*>(oit.getNext());
					aabb.merge(sceneChild->_getWorldAABB());
				}

				////////////////////////////////////////

				Ogre::Vector3 min = aabb.getMinimum();
				Ogre::Vector3 max = aabb.getMaximum();


				cout << "         AABB Min : " <<  min  << endl;
				cout << "         AABB Max : " <<  max  << endl;

				cout << "         AABB : " << aabb << endl;

				if (aabb.isNull())
				{
					cout << "         AABB IS NULL !!!" << endl;
				}
				cout << "         Camera : " << *ogreCamera << endl;

				result = ogreCamera->isVisible(aabb);


			} else
			{
				cout << "        root node unavailable (" <<  this->getInstance() << ")" << endl;
				cout << "        looking at neighbors" << endl;

				// Find an initialized neighbor (return false if no neighbor is found)
				// Get its world position
				// Add offset to get current page world position
				// Construct an Ogre::AxisAlignedBox
				// Check if visible from ogrecamera
				char* neighbors[] = {"northLink", "southLink", "eastLink", "westLink"};
				PersistentObjectManager *pom = PersistentObjectManager::getInstancePtr();


				shared_ptr< Gnoll::Core::String > loadedNeighbor;

				for (unsigned int i = 0; (i < 4) && (loadedNeighbor.get() == NULL); i++)
				{

					cout << "                Looking at neighbor from " << neighbors[i] << endl;

					if (this->hasAttribute( neighbors[i] ))
					{

						shared_ptr< Gnoll::Core::String > neighbor = this->getAttribute< Gnoll::Core::String >( neighbors[i] );
						cout << "                Looking at neighbor " << *neighbor << " from " << neighbors[i] << endl;

						if (pom->isInstanceInCache (*neighbor) )
						{
							CPage neighborPage(*neighbor);
							if (neighborPage.isInitialized())
							{
								loadedNeighbor = neighbor;
								cout << "Youpi !" << endl;
							}
						}
					}
				}

				if (loadedNeighbor)
				{
					CPage neighborPage(*loadedNeighbor);
					cout << "        neighbor found : " <<  neighborPage.getInstance() << endl;
					const char* neighborStr = string(*loadedNeighbor).c_str();

					Ogre::SceneNode* rootNode = neighborPage.getPageRootNode();

					//const Ogre::AxisAlignedBox aabbOrig = rootNode->_getWorldAABB();
					Ogre::AxisAlignedBox aabbOrig;

				//////////////////////////////////////////////////

				/**
				 * The OctreeSceneManager doesn't take children nodes in account
				 * when computing AABB box even though this is taken in account by
				 * Ogre::SceneNode.
				 * So this is computed by hand like does Ogre::SceneNode.
				 */


				// Update bounds from own attached objects
				SceneNode::ObjectIterator oi = rootNode->getAttachedObjectIterator();
				while (oi.hasMoreElements())
				{
					MovableObject *m = oi.getNext();

					// Merge world bounds of each object
					aabbOrig.merge(m->getWorldBoundingBox(true));
				}

				// Merge with children
				Node::ChildNodeIterator oit = rootNode->getChildIterator();
				while (oit.hasMoreElements())
				{
					SceneNode* sceneChild = static_cast<SceneNode*>(oit.getNext());
					aabbOrig.merge(sceneChild->_getWorldAABB());
				}

				////////////////////////////////////////


					Ogre::Vector3 min = aabbOrig.getMinimum();
					Ogre::Vector3 max = aabbOrig.getMaximum();

				cout << "         AABB Min : " <<  min  << endl;
				cout << "         AABB Max : " <<  max  << endl;

					shared_ptr<Gnoll::Core::Float> pageSize = neighborPage.getAttribute<Float>("size");

					Ogre::Vector3 neighborOffset;

					if (strcmp( neighborStr, string(*(this->getAttribute< Gnoll::Core::String >( "northLink"))).c_str()) == 0)
					{
						neighborOffset = Ogre::Vector3(0.0, 0.0, -*pageSize) / 2.0f;

					} else if (strcmp( neighborStr, string(*(this->getAttribute< Gnoll::Core::String >( "southLink"))).c_str()) == 0)
					{
						neighborOffset = Ogre::Vector3(0.0, 0.0, *pageSize) / 2.0f;

					} else if (strcmp( neighborStr, string(*(this->getAttribute< Gnoll::Core::String >( "eastLink"))).c_str()) == 0)
					{
						neighborOffset = Ogre::Vector3( *pageSize, 0.0, 0.0 ) / 2.0f;

					} else if (strcmp( neighborStr, string(*(this->getAttribute< Gnoll::Core::String >( "westLink"))).c_str()) == 0)
					{
						neighborOffset = Ogre::Vector3( -(*pageSize), 0.0, 0.0 ) / 2.0f;
					}

					Ogre::AxisAlignedBox aabb(
														min.x + neighborOffset.x,
														min.y + neighborOffset.y,
														min.z + neighborOffset.z,
														max.x + neighborOffset.x,
														max.y + neighborOffset.y,
														max.z + neighborOffset.z
														);

					result = ogreCamera->isVisible(aabb);

				}
			}

					cout << "             Is " << this->getInstance() << " visible : " << result << endl;
			return result;
		}
	}
}


