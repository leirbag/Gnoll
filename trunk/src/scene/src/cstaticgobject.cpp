/***************************************************************************
 *   Copyright (C) 2008 by Bruno Mahe                                      *
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


/*-----------------------------CStaticGObject------------------------------*\
|   This is a static game object                                            |
|                                                                           |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include "../include/cstaticgobject.h"
#include "../../graphic/include/cgraphicmodule.h"
#include "../../log/include/clogmodule.h"
#include "../include/cpage.h"
#include <glibmm/ustring.h>

#include <iostream>
using namespace std;
using namespace boost;
using namespace Ogre;
using namespace Gnoll::Graphic;

namespace Gnoll
{
	namespace Scene
	{


		CStaticGObject::CStaticGObject():
														DynamicObject::DynamicObject(Glib::ustring(""))
		{

			Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "CStaticGObject object created");
		}


		CStaticGObject::~CStaticGObject()
		{
		}


		void CStaticGObject::init(CPage* _parentPage)
		{

			if (_parentPage == NULL) 
			{
				Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "CStaticGObject object initialized without any parent page");
				return;
			}

			shared_ptr< Gnoll::DynamicObject::String > instanceName = this->getAttribute < Gnoll::DynamicObject::String > ("name");
			string instanceNameStr(*instanceName);

			shared_ptr< Gnoll::DynamicObject::String > meshName = this->getAttribute < Gnoll::DynamicObject::String > ("mesh");
			string meshNameStr(*meshName);

			SceneNode *parentNode = _parentPage->getPageRootNode();
			SceneNode *staticGObjectNode = parentNode->createChildSceneNode( _parentPage->getInstance() + "_" + instanceNameStr );

			std::string entName = _parentPage->getInstance() + "_" + instanceNameStr + "_entity";
			Ogre::SceneManager* sm = CGraphicModule::getInstancePtr()->getSceneManager();
			
			Ogre::Entity *ent = sm->createEntity( entName, meshNameStr );
			staticGObjectNode->attachObject( ent );

		}


		void CStaticGObject::update()
		{
		}


		void CStaticGObject::exit(CPage* _parentPage)
		{
			if (_parentPage == NULL) 
			{
				Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "CStaticGObject object exiting without any parent page");
				return;
			}

			shared_ptr< Gnoll::DynamicObject::String > instanceName = this->getAttribute < Gnoll::DynamicObject::String > ("name");
			string instanceNameStr(*instanceName);

			std::string entName = _parentPage->getInstance() + "_" + instanceNameStr  + "_entity";
			Ogre::SceneManager* sm = CGraphicModule::getInstancePtr()->getSceneManager();
			
			SceneNode* staticGObjectNode = sm->getSceneNode( _parentPage->getInstance() + "_" + instanceNameStr );
			staticGObjectNode->detachObject( entName );

			sm->destroyEntity(entName);
			sm->destroySceneNode( _parentPage->getInstance() + "_" + instanceNameStr  );
		}


		shared_ptr<xmlpp::Document> CStaticGObject::serializeXML()
		{
			shared_ptr<xmlpp::Document> document = DynamicObject::serializeXML();
			xmlpp::Element* root = document->get_root_node();

			if (root != NULL)
			{
				root->set_name( "cstaticgobject" );
			}

			return document;
		}
	}
}


