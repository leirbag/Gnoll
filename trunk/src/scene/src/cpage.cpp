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
#include "../../log/include/clogmodule.h"
#include "../../dynamicobject/include/dynamicobjectmanager.h"
#include "../../dynamicobject/include/float.h"
#include "../include/cpage.h"
#include "../include/ipagerenderer.h"
#include "../include/cstaticgobject.h"
#include <OgreCamera.h>

#include <sstream>

using namespace std;
using namespace boost;
using namespace Gnoll::Graphic;

namespace Gnoll
{
	namespace Scene
	{


		CPage::CPage(string _instanceName): CDynamicObjectProxy(_instanceName)
		{

		}

		void CPage::init()
		{

			Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "!!!!!!!!!!!!!!!! INIT OF PAGE " + this->getInstance() );

			Ogre::SceneManager* sm = CGraphicModule::getInstancePtr()->getSceneManager();
			SceneNode* root = sm->getRootSceneNode();

			// Creating root page node
			root->createChildSceneNode( this->getInstance() );


			/**
			 * Load the page renderer
			 */
			if (this->hasAttribute("PageRenderer"))
			{
				Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "From " + this->getInstance() + " // Initializing PageRenderer" );
				shared_ptr<IPageRenderer> pRenderer = this->getAttribute<IPageRenderer>("PageRenderer");
				pRenderer->init(this);
			}


			/**
			 * Load static objects
			 */
			if (this->hasAttribute("staticObjects"))
			{
				shared_ptr< Gnoll::DynamicObject::List > listStaticObjects = this->getAttribute < Gnoll::DynamicObject::List > ("staticObjects");

				typedef list< shared_ptr<Gnoll::DynamicObject::IAttribute> >::iterator ListIterator;

				for( ListIterator it = listStaticObjects->begin(); it != listStaticObjects->end(); it++)
				{
					if (shared_ptr<Gnoll::Scene::CStaticGObject> staticGObject = dynamic_pointer_cast<Gnoll::Scene::CStaticGObject>(*it))
					{
						staticGObject->init(this);
					}
				}
			}
		}

		void CPage::unInit()
		{
			Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "!!!!!!!!!!!!!!!! DE-INIT OF PAGE " + this->getInstance() );

			if (this->isInitialized())
			{
				Ogre::SceneManager* sm = CGraphicModule::getInstancePtr()->getSceneManager();

				/**
				 * Unload static objects
				 */
				if (this->hasAttribute("staticObjects"))
				{
					shared_ptr< Gnoll::DynamicObject::List > listStaticObjects = this->getAttribute < Gnoll::DynamicObject::List > ("staticObjects");

					typedef list< shared_ptr<Gnoll::DynamicObject::IAttribute> >::iterator ListIterator;

					for( ListIterator it = listStaticObjects->begin(); it != listStaticObjects->end(); it++)
					{
						if (shared_ptr<Gnoll::Scene::CStaticGObject> staticGObject = dynamic_pointer_cast<Gnoll::Scene::CStaticGObject>(*it))
						{
							staticGObject->exit(this);
						}
					}
				}


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

			/**
			 * The way to check if a page has been initialized, is to check if the Page Root Node has
			 * been created.
			 */
			if (this->getPageRootNode())
			{
				result = true;
			}

			return result;
		}


		bool CPage::isVisibleFromCamera(shared_ptr< Gnoll::DynamicObject::String > _cameraName) const
		{
			// XXX TODO
			// Get page root node coordinates
			// get page length
			// Compute AABB box
			// Check with the current camera if the AABB box is inside the view frustrum

			Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "       Checking visibility from camera : " + string(*_cameraName) + " for page " + this->getInstance());
			Ogre::Camera* ogreCamera = CGraphicModule::getInstancePtr()->getSceneManager()->getCamera(*_cameraName);
			Ogre::SceneNode* rootNode = this->getPageRootNode();

			bool result = false;

			if (rootNode)
			{
				std::ostringstream tmpString;
				tmpString << "        root node available (" <<  this->getInstance() << ")" << endl;
				tmpString << "         Number of children : " << rootNode->numChildren() << endl;
				Gnoll::Log::CLogModule::getInstancePtr()->logMessage( tmpString.str() );
				tmpString.clear();

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
					aabb.merge(m->getWorldBoundingBox());
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


				tmpString << "         AABB Min : " <<  min  << endl;
				tmpString << "         AABB Max : " <<  max  << endl;

				tmpString << "         AABB : " << aabb << endl;

				if (aabb.isNull())
				{
					tmpString << "         AABB IS NULL !!!" << endl;
				}
				tmpString << "         Camera : " << *ogreCamera << endl;

				Gnoll::Log::CLogModule::getInstancePtr()->logMessage( tmpString.str() );

				result = ogreCamera->isVisible(aabb);


			} else
			{
				Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "        root node unavailable (" + this->getInstance() + ")" );;
				Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "        looking at neighbors" );

				// Find an initialized neighbor (return false if no neighbor is found)
				// Get its world position
				// Add offset to get current page world position
				// Construct an Ogre::AxisAlignedBox
				// Check if visible from ogrecamera
				char* neighbors[] = {"northLink", "southLink", "eastLink", "westLink"};
				DynamicObjectManager *pom = DynamicObjectManager::getInstancePtr();


				shared_ptr< Gnoll::DynamicObject::String > loadedNeighbor;

				std::ostringstream tmpString;
				for (unsigned int i = 0; (i < 4) && (loadedNeighbor.get() == NULL); i++)
				{

					Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "                Looking at neighbor from " + string(neighbors[i]) );

					if (this->hasAttribute( neighbors[i] ))
					{

						shared_ptr< Gnoll::DynamicObject::String > neighbor = this->getAttribute< Gnoll::DynamicObject::String >( neighbors[i] );

						tmpString << "                Looking at neighbor " << *neighbor << " from " << neighbors[i] << endl;
						Gnoll::Log::CLogModule::getInstancePtr()->logMessage( tmpString.str() );
						tmpString.clear();

						if (pom->isInstanceInCache (*neighbor) )
						{
							CPage neighborPage(*neighbor);
							if (neighborPage.isInitialized())
							{
								loadedNeighbor = neighbor;
								Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "Youpi !" );
							}
						}
					}
				}

				if (loadedNeighbor)
				{
					CPage neighborPage(*loadedNeighbor);
					Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "        neighbor found : " +  neighborPage.getInstance() );
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

					std::ostringstream tmpString;
					tmpString << "         AABB Min : " << min << endl;
					tmpString << "         AABB Max : " << max;
					Gnoll::Log::CLogModule::getInstancePtr()->logMessage( tmpString.str() );

					shared_ptr<Gnoll::DynamicObject::Float> pageSize = neighborPage.getAttribute<Float>("size");

					Ogre::Vector3 neighborOffset;

					if (strcmp( neighborStr, string(*(this->getAttribute< Gnoll::DynamicObject::String >( "northLink"))).c_str()) == 0)
					{
						neighborOffset = Ogre::Vector3(0.0, 0.0, -*pageSize) / 2.0f;

					} else if (strcmp( neighborStr, string(*(this->getAttribute< Gnoll::DynamicObject::String >( "southLink"))).c_str()) == 0)
					{
						neighborOffset = Ogre::Vector3(0.0, 0.0, *pageSize) / 2.0f;

					} else if (strcmp( neighborStr, string(*(this->getAttribute< Gnoll::DynamicObject::String >( "eastLink"))).c_str()) == 0)
					{
						neighborOffset = Ogre::Vector3( *pageSize, 0.0, 0.0 ) / 2.0f;

					} else if (strcmp( neighborStr, string(*(this->getAttribute< Gnoll::DynamicObject::String >( "westLink"))).c_str()) == 0)
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

			std::ostringstream tmpString;
			tmpString << "             Is " << this->getInstance() << " visible : " << result;
			Gnoll::Log::CLogModule::getInstancePtr()->logMessage( tmpString.str() );
			return result;
		}
	}
}


