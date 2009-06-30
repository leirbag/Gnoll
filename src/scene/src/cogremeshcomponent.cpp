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

#include "../../core/include/cmessagetype.h"
#include "../../core/include/cmessagemodule.h"
#include "../include/cogremeshcomponent.h"
#include "../include/gobject.h"

using namespace Gnoll::DynamicObject;
using namespace Gnoll::Log;

namespace Gnoll {

	namespace Scene {

		class OgreMeshPositionListener : public CMessageListener
		{
			private:
				COgreMeshComponent* component;

			public:

				/**
				 * This is a constructor
				 */
				OgreMeshPositionListener(COgreMeshComponent* _component) : component(_component) {}

				/**
				 * This is a destructor
				 */
				virtual ~OgreMeshPositionListener() {}

				/**
				 * This method is called in order to process a message
				 * @param message The message this method will have to process
				 */
				virtual void handle (shared_ptr<CMessage> message)
				{
					Ogre::Vector3 position = message->getData<Ogre::Vector3>();
					component->setPosition(position);
				}
		};


		class OgreMeshScalingListener : public CMessageListener
		{
			private:
				COgreMeshComponent* component;

			public:

				/**
				 * This is a constructor
				 */
				OgreMeshScalingListener(COgreMeshComponent* _component) : component(_component) {}

				/**
				 * This is a destructor
				 */
				virtual ~OgreMeshScalingListener() {}

				/**
				 * This method is called in order to process a message
				 * @param message The message this method will have to process
				 */
				virtual void handle (shared_ptr<CMessage> message)
				{
					Ogre::Vector3 scale = message->getData<Ogre::Vector3>();
					component->setScaling(scale);
				}
		};


		class OgreMeshRotationListener : public CMessageListener
		{
			private:
				COgreMeshComponent* component;

			public:

				/**
				 * This is a constructor
				 */
				OgreMeshRotationListener(COgreMeshComponent* _component) : component(_component) {}

				/**
				 * This is a destructor
				 */
				virtual ~OgreMeshRotationListener() {}

				/**
				 * This method is called in order to process a message
				 * @param message The message this method will have to process
				 */
				virtual void handle (shared_ptr<CMessage> message)
				{
					Ogre::Vector3 rotation = message->getData<Ogre::Vector3>();
					component->setRotation(rotation);
				}
		};

		COgreMeshComponent::COgreMeshComponent()
		{
			this->m_parentPageName = "";
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


		const std::string& COgreMeshComponent::getParentPageName() const
		{
			return m_parentPageName;
		}


		void COgreMeshComponent::setParentPageName(const std::string& m_parentPageName)
		{
			this->m_parentPageName = m_parentPageName;
		}


		void COgreMeshComponent::setPosition(const Ogre::Vector3& position)
		{
			string instanceNameStr(this->getInstance());
			Ogre::SceneManager* sm = Gnoll::Graphic::CGraphicModule::getInstancePtr()->getSceneManager();

			SceneNode* meshNode = sm->getSceneNode( m_parentPageName + "_" + instanceNameStr );
			meshNode->translate(position, Ogre::Node::TS_LOCAL);
		}


		void COgreMeshComponent::setScaling(const Ogre::Vector3& scale)
		{
			string instanceNameStr(this->getInstance());
			Ogre::SceneManager* sm = Gnoll::Graphic::CGraphicModule::getInstancePtr()->getSceneManager();

			SceneNode* meshNode = sm->getSceneNode( m_parentPageName + "_" + instanceNameStr );
			meshNode->setScale(scale.x, scale.y, scale.z);
		}


		void COgreMeshComponent::setRotation(const Ogre::Vector3& rotation)
		{
			string instanceNameStr(this->getInstance());
			Ogre::SceneManager* sm = Gnoll::Graphic::CGraphicModule::getInstancePtr()->getSceneManager();

			SceneNode* meshNode = sm->getSceneNode( m_parentPageName + "_" + instanceNameStr );
			meshNode->rotate(Ogre::Vector3(1,0,0), Ogre::Radian(rotation.x));
			meshNode->rotate(Ogre::Vector3(0,1,0), Ogre::Radian(rotation.x));
			meshNode->rotate(Ogre::Vector3(0,0,1), Ogre::Radian(rotation.x));
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
			this->m_parentPageName = page->getInstance();


			shared_ptr< Gnoll::DynamicObject::String > meshName = this->getAttribute < Gnoll::DynamicObject::String > (COgreMeshComponent::ATTRIBUTE_MESH());
			string meshNameStr(*meshName);

			SceneNode *parentNode = page->getPageRootNode();
			SceneNode *meshNode = parentNode->createChildSceneNode( getParentPageName() + "_" + this->getInstance() );

			std::string entName = getParentPageName() + "_" + this->getInstance() + COgreMeshComponent::ENTITY_SUFFIX();
			Ogre::SceneManager* sm = Gnoll::Graphic::CGraphicModule::getInstancePtr()->getSceneManager();

			Ogre::Entity *ent = sm->createEntity( entName, meshNameStr );
			meshNode->attachObject( ent );


			/**
			 * Register the listener
			 */
			CMessageModule*  messageModule  = CMessageModule::getInstancePtr();
			CMessageManager* messageManager = messageModule->getMessageManager();

			componentPositionListener = shared_ptr<CMessageListener> (new OgreMeshPositionListener(this));
			componentScalingListener = shared_ptr<CMessageListener> (new OgreMeshScalingListener(this));
			componentRotationListener = shared_ptr<CMessageListener> (new OgreMeshRotationListener(this));
			messageManager->addListener ( componentPositionListener, "SET_POSITION_" + m_parent->getInstance() );
			messageManager->addListener ( componentScalingListener, "SET_SCALING_" + m_parent->getInstance() );
			messageManager->addListener ( componentRotationListener, "SET_ROTATION_" + m_parent->getInstance() );

		}


		 void COgreMeshComponent::exit()
		 {
			GNOLL_LOG() << "Gobject " << this->m_parent->getInstance() << " is DEinitializing its component named " << this->getInstance() << "\n";


			if (this->m_parentPageName == "")
			{

				GNOLL_LOG() << m_parent->getInstance() << ":" << this->getInstance() << " object cannot be DEinitialized without a parent page" << "\n";
				return;
			}

			string instanceNameStr(this->getInstance());

			std::string entName = getParentPageName() + "_" + instanceNameStr  + COgreMeshComponent::ENTITY_SUFFIX();
			Ogre::SceneManager* sm = Gnoll::Graphic::CGraphicModule::getInstancePtr()->getSceneManager();

			SceneNode* meshNode = sm->getSceneNode( getParentPageName() + "_" + instanceNameStr );
			meshNode->detachObject( entName );

			sm->destroyEntity(entName);
			sm->destroySceneNode( getParentPageName() + "_" + instanceNameStr  );

			/**
			 * Unregister the listener
			 */
			CMessageModule*  messageModule  = CMessageModule::getInstancePtr();
			CMessageManager* messageManager = messageModule->getMessageManager();

			messageManager->delListener ( componentPositionListener, "SET_POSITION_" + m_parent->getInstance() );
			messageManager->delListener ( componentScalingListener, "SET_SCALING_" + m_parent->getInstance() );
			messageManager->delListener ( componentRotationListener, "SET_ROTATION_" + m_parent->getInstance() );
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
