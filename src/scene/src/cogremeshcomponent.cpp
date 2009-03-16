/***************************************************************************
 *   Copyright (C) 2009 by Bruno Mahe                                      *
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


/*---------------------------COgreMeshComponent----------------------------*\
|   This is a component able to display Ogre models                         |
|                                                                           |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include "../../dynamicobject/include/float.h"
#include "../../graphic/include/cgraphicmodule.h"
#include "../../log/include/clogmacros.h"
#include "../../graphic/include/cgraphicmodule.h"

#include "../include/cogremeshcomponent.h"
#include "../include/gobject.h"

using namespace Gnoll::DynamicObject;
using namespace Gnoll::Log;

namespace Gnoll {

	namespace Scene {

		COgreMeshComponent::COgreMeshComponent()
		{
			this->m_parentPage = NULL;
			this->m_parent = NULL;
		}


		COgreMeshComponent::~COgreMeshComponent()
		{
		}


		Gnoll::Scene::GObject * COgreMeshComponent::getParent() const
		{
			return m_parent;
		}


		void COgreMeshComponent::setParent(Gnoll::Scene::GObject *m_parent)
		{
			this->m_parent = m_parent;
		}


		Gnoll::Scene::CPage * COgreMeshComponent::getParentPage() const
		{
			return m_parentPage;
		}


		void COgreMeshComponent::setParentPage(Gnoll::Scene::CPage *m_parentPage)
		{
			this->m_parentPage = m_parentPage;
		}


		void COgreMeshComponent::init(Gnoll::Scene::GObject* parent, CPage* page)
		{

			/**
			 * A parent must exist
			 */
			if (parent == NULL)
			{
				GNOLL_LOG() << this->getInstance() << " object initialized without any parent";
				return;
			}

			/**
			 * A parent page must exist
			 */
			if (page == NULL)
			{
				GNOLL_LOG() << parent->getInstance() << ":" << this->getInstance() << " object initialized without any parent page" << "\n";
				return;
			}

			this->m_parent = parent;
			this->m_parentPage = page;


			shared_ptr< Gnoll::DynamicObject::String > meshName = this->getAttribute < Gnoll::DynamicObject::String > (COgreMeshComponent::ATTRIBUTE_MESH());
			string meshNameStr(*meshName);

			SceneNode *parentNode = m_parentPage->getPageRootNode();
			SceneNode *meshNode = parentNode->createChildSceneNode( m_parentPage->getInstance() + "_" + this->getInstance() );

			std::string entName = m_parentPage->getInstance() + "_" + this->getInstance() + COgreMeshComponent::ENTITY_SUFFIX();
			Ogre::SceneManager* sm = Gnoll::Graphic::CGraphicModule::getInstancePtr()->getSceneManager();

			Ogre::Entity *ent = sm->createEntity( entName, meshNameStr );
			meshNode->attachObject( ent );


			shared_ptr< Gnoll::DynamicObject::Float > zero ( new Gnoll::DynamicObject::Float(0.0f));
			shared_ptr< Gnoll::DynamicObject::Float > one ( new Gnoll::DynamicObject::Float(1.0f));



			/**
			 * Scale the object
			 */

			shared_ptr< Gnoll::DynamicObject::Float > scaleX = this->getAttributeOrDefault < Gnoll::DynamicObject::Float > (COgreMeshComponent::ATTRIBUTE_SCALE_X(), one);
			shared_ptr< Gnoll::DynamicObject::Float > scaleY = this->getAttributeOrDefault < Gnoll::DynamicObject::Float > (COgreMeshComponent::ATTRIBUTE_SCALE_Y(), one);
			shared_ptr< Gnoll::DynamicObject::Float > scaleZ = this->getAttributeOrDefault < Gnoll::DynamicObject::Float > (COgreMeshComponent::ATTRIBUTE_SCALE_Z(), one);


			meshNode->setScale(*scaleX, *scaleY, *scaleZ);


			/**
			 * Rotate the object
			 */

			shared_ptr< Gnoll::DynamicObject::Float > rotX = this->getAttributeOrDefault < Gnoll::DynamicObject::Float > (COgreMeshComponent::ATTRIBUTE_ROTATE_X(), zero);
			shared_ptr< Gnoll::DynamicObject::Float > rotY = this->getAttributeOrDefault < Gnoll::DynamicObject::Float > (COgreMeshComponent::ATTRIBUTE_ROTATE_Y(), zero);
			shared_ptr< Gnoll::DynamicObject::Float > rotZ = this->getAttributeOrDefault < Gnoll::DynamicObject::Float > (COgreMeshComponent::ATTRIBUTE_ROTATE_Z(), zero);

			meshNode->rotate(Ogre::Vector3(1,0,0), Ogre::Radian(*rotX));
			meshNode->rotate(Ogre::Vector3(0,1,0), Ogre::Radian(*rotY));
			meshNode->rotate(Ogre::Vector3(0,0,1), Ogre::Radian(*rotZ));


			/**
			 * Translate the object
			 */
			shared_ptr< Gnoll::DynamicObject::Float > posX = this->getAttributeOrDefault < Gnoll::DynamicObject::Float > (COgreMeshComponent::ATTRIBUTE_POSITION_X(), zero);
			shared_ptr< Gnoll::DynamicObject::Float > posY = this->getAttributeOrDefault < Gnoll::DynamicObject::Float > (COgreMeshComponent::ATTRIBUTE_POSITION_Y(), zero);
			shared_ptr< Gnoll::DynamicObject::Float > posZ = this->getAttributeOrDefault < Gnoll::DynamicObject::Float > (COgreMeshComponent::ATTRIBUTE_POSITION_Z(), zero);

			Ogre::Vector3 posGObject(*posX, *posY, *posZ);
			meshNode->translate( posGObject, Ogre::Node::TS_LOCAL);



		}


		 void COgreMeshComponent::exit()
		 {
			GNOLL_LOG() << "Gobject " << this->m_parent->getInstance() << " is DEinitializing its component named " << this->getInstance() << "\n";


			if (this->m_parentPage == NULL)
			{

				GNOLL_LOG() << m_parent->getInstance() << ":" << this->getInstance() << " object cannot be DEinitialized without a parent page" << "\n";
				return;
			}

			string instanceNameStr(this->getInstance());

			std::string entName = m_parentPage->getInstance() + "_" + instanceNameStr  + COgreMeshComponent::ENTITY_SUFFIX();
			Ogre::SceneManager* sm = Gnoll::Graphic::CGraphicModule::getInstancePtr()->getSceneManager();

			SceneNode* meshNode = sm->getSceneNode( m_parentPage->getInstance() + "_" + instanceNameStr );
			meshNode->detachObject( entName );

			sm->destroyEntity(entName);
			sm->destroySceneNode( m_parentPage->getInstance() + "_" + instanceNameStr  );

		}


		shared_ptr<xmlpp::Document> COgreMeshComponent::serializeXML()
		{
			shared_ptr<xmlpp::Document> document = DynamicObject::serializeXML();
			xmlpp::Element* root = document->get_root_node();

			if (root != NULL)
			{
				root->set_name( COgreMeshComponent::DYNAMIC_OBJECT_NAME() );
			}

			return document;
		}

	}
}
