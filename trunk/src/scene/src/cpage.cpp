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
#include "../include/cpage.h"
#include "../include/ipagerenderer.h"

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

			Ogre::SceneManager* sm = CGraphicModule::getInstancePtr()->getSceneManager();
			SceneNode* root = sm->getRootSceneNode();

			// Creating root page node
			root->createChildSceneNode( this->getInstance() );
			
			if (this->hasAttribute("PageRenderer"))
			{
				cout << "Initializing PageRenderer" << endl;
				shared_ptr<IPageRenderer> pRenderer = this->getAttribute<IPageRenderer>("PageRenderer");
				pRenderer->init(this);
			}

		}

		void CPage::unInit()
		{
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


		Ogre::SceneNode * CPage::getPageRootNode()
		{
			Ogre::SceneManager* sm = CGraphicModule::getInstancePtr()->getSceneManager();
			SceneNode* node = sm->getSceneNode( this->getInstance() );
			return node;
		}


	}
}


