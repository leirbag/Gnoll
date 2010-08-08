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

#include "../include/staticgobject.h"

#include <iostream>

#include <glibmm/ustring.h>

#include "../../graphic/include/graphicmodule.h"
#include "../../log/include/logmodule.h"
#include "../../dynamicobject/include/float.h"
#include "../page/include/cpage.h"

using namespace std;
using namespace boost;
using namespace Ogre;
using namespace Gnoll::Graphic;

namespace Gnoll
{
	namespace Scene
	{
		StaticGObject::StaticGObject():
			DynamicObject::DynamicObject(Glib::ustring(""))
		{
			Gnoll::Log::LogModule::getInstancePtr()->logMessage( "StaticGObject object created");
		}

		StaticGObject::~StaticGObject()
		{
		}

		void StaticGObject::init(CPage* _parentPage)
		{
			/**
			 * A parent page must exist
			 */
			if (_parentPage == NULL)
			{
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "StaticGObject object initialized without any parent page");
				return;
			}

			string instanceNameStr = this->getInstance();

			shared_ptr< Gnoll::DynamicObject::String > meshName = this->getAttribute < Gnoll::DynamicObject::String > (StaticGObject::ATTRIBUTE_MESH());
			string meshNameStr(*meshName);

			SceneNode *parentNode = _parentPage->getPageRootNode();
			SceneNode *staticGObjectNode = parentNode->createChildSceneNode( _parentPage->getInstance() + "_" + instanceNameStr );

			std::string entName = _parentPage->getInstance() + "_" + instanceNameStr + StaticGObject::ENTITY_SUFFIX();
			Ogre::SceneManager* sm = GraphicModule::getInstancePtr()->getSceneManager();

			Ogre::Entity *ent = sm->createEntity( entName, meshNameStr );
			staticGObjectNode->attachObject( ent );

			shared_ptr< Gnoll::DynamicObject::Float > zero ( new Gnoll::DynamicObject::Float(0.0f));
			shared_ptr< Gnoll::DynamicObject::Float > one ( new Gnoll::DynamicObject::Float(1.0f));

			/**
			 * Scale the object
			 */

			shared_ptr< Gnoll::DynamicObject::Float > scaleX = this->getAttributeOrDefault < Gnoll::DynamicObject::Float > (StaticGObject::ATTRIBUTE_SCALE_X(), one);
			shared_ptr< Gnoll::DynamicObject::Float > scaleY = this->getAttributeOrDefault < Gnoll::DynamicObject::Float > (StaticGObject::ATTRIBUTE_SCALE_Y(), one);
			shared_ptr< Gnoll::DynamicObject::Float > scaleZ = this->getAttributeOrDefault < Gnoll::DynamicObject::Float > (StaticGObject::ATTRIBUTE_SCALE_Z(), one);

			staticGObjectNode->setScale(*scaleX, *scaleY, *scaleZ);

			/**
			 * Rotate the object
			 */
			shared_ptr< Gnoll::DynamicObject::Float > rotX = this->getAttributeOrDefault < Gnoll::DynamicObject::Float > (StaticGObject::ATTRIBUTE_ROTATE_X(), zero);
			shared_ptr< Gnoll::DynamicObject::Float > rotY = this->getAttributeOrDefault < Gnoll::DynamicObject::Float > (StaticGObject::ATTRIBUTE_ROTATE_Y(), zero);
			shared_ptr< Gnoll::DynamicObject::Float > rotZ = this->getAttributeOrDefault < Gnoll::DynamicObject::Float > (StaticGObject::ATTRIBUTE_ROTATE_Z(), zero);

			staticGObjectNode->rotate(Ogre::Vector3(1,0,0), Ogre::Radian(*rotX));
			staticGObjectNode->rotate(Ogre::Vector3(0,1,0), Ogre::Radian(*rotY));
			staticGObjectNode->rotate(Ogre::Vector3(0,0,1), Ogre::Radian(*rotZ));

			/**
			 * Translate the object
			 */
			shared_ptr< Gnoll::DynamicObject::Float > posX = this->getAttributeOrDefault < Gnoll::DynamicObject::Float > (StaticGObject::ATTRIBUTE_POSITION_X(), zero);
			shared_ptr< Gnoll::DynamicObject::Float > posY = this->getAttributeOrDefault < Gnoll::DynamicObject::Float > (StaticGObject::ATTRIBUTE_POSITION_Y(), zero);
			shared_ptr< Gnoll::DynamicObject::Float > posZ = this->getAttributeOrDefault < Gnoll::DynamicObject::Float > (StaticGObject::ATTRIBUTE_POSITION_Z(), zero);

			Ogre::Vector3 posGObject(*posX, *posY, *posZ);
			staticGObjectNode->setPosition(posGObject);

		}

		void StaticGObject::update()
		{
		}

		void StaticGObject::exit(CPage* _parentPage)
		{
			if (_parentPage == NULL)
			{
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "StaticGObject object exiting without any parent page");
				return;
			}

			string instanceNameStr(this->getInstance());

			std::string entName = _parentPage->getInstance() + "_" + instanceNameStr  + StaticGObject::ENTITY_SUFFIX();
			Ogre::SceneManager* sm = GraphicModule::getInstancePtr()->getSceneManager();

			SceneNode* staticGObjectNode = sm->getSceneNode( _parentPage->getInstance() + "_" + instanceNameStr );
			staticGObjectNode->detachObject( entName );

			sm->destroyEntity(entName);
			sm->destroySceneNode( _parentPage->getInstance() + "_" + instanceNameStr  );
		}

		shared_ptr<xmlpp::Document> StaticGObject::serializeXML()
		{
			shared_ptr<xmlpp::Document> document = DynamicObject::serializeXML();
			xmlpp::Element* root = document->get_root_node();

			if (root != NULL)
				root->set_name( StaticGObject::DYNAMIC_OBJECT_NAME() );

			return document;
		}
	}
}
