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

/*--------------------------COgreAnimatedMeshComponent---------------------*\
|   This is a component able to display Ogre models that are animated       |
|                                                                           |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include "../../dynamicobject/include/float.h"
#include "../../dynamicobject/include/string.h"
#include "../../graphic/include/cgraphicmodule.h"
#include "../../log/include/clogmacros.h"
#include "../../graphic/include/cgraphicmodule.h"
#include "../../stats/include/statsmodule.h"
#include "../../core/messages/include/messagetype.h"
#include "../../core/include/cmessagemodule.h"
#include "../include/cogreanimatedmeshcomponent.h"
#include "../include/gobject.h"

using namespace Gnoll::DynamicObject;
using namespace Gnoll::Log;

namespace Gnoll {

	namespace Scene {

		class OgreAnimatedMeshListener : public Messages::Listener
		{
			private:
				COgreAnimatedMeshComponent* component;

			public:

				/**
				 * This is a constructor
				 */
				OgreAnimatedMeshListener(COgreAnimatedMeshComponent* _component) : component(_component) {}

				/**
				 * This is a destructor
				 */
				virtual ~OgreAnimatedMeshListener() {}

				/**
				 * This method is called in order to process a message
				 * @param message The message this method will have to process
				 */
				virtual void handle (shared_ptr<CMessage> message)
				{
					float lasttime = Gnoll::Stats::StatsModule::getInstancePtr()->getRenderTime();
					component->update(lasttime/1000.0f);
				}
		};

		class OgreAnimatedMeshPositionListener : public Messages::Listener
		{
			private:
				COgreAnimatedMeshComponent* component;

			public:

				/**
				 * This is a constructor
				 */
				OgreAnimatedMeshPositionListener(COgreAnimatedMeshComponent* _component) : component(_component) {}

				/**
				 * This is a destructor
				 */
				virtual ~OgreAnimatedMeshPositionListener() {}

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


		class OgreAnimatedMeshScalingListener : public Messages::Listener
		{
			private:
				COgreAnimatedMeshComponent* component;

			public:

				/**
				 * This is a constructor
				 */
				OgreAnimatedMeshScalingListener(COgreAnimatedMeshComponent* _component) : component(_component) {}

				/**
				 * This is a destructor
				 */
				virtual ~OgreAnimatedMeshScalingListener() {}

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


		class OgreAnimatedMeshRotationListener : public Messages::Listener
		{
			private:
				COgreAnimatedMeshComponent* component;

			public:

				/**
				 * This is a constructor
				 */
				OgreAnimatedMeshRotationListener(COgreAnimatedMeshComponent* _component) : component(_component) {}

				/**
				 * This is a destructor
				 */
				virtual ~OgreAnimatedMeshRotationListener() {}

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


		class OgreAnimatedMeshAnimationStateListener : public Messages::Listener
		{
			private:
				COgreAnimatedMeshComponent* component;

			public:

				/**
				 * This is a constructor
				 */
				OgreAnimatedMeshAnimationStateListener(COgreAnimatedMeshComponent* _component) : component(_component) {}

				/**
				 * This is a destructor
				 */
				virtual ~OgreAnimatedMeshAnimationStateListener() {}

				/**
				 * This method is called in order to process a message
				 * @param message The message this method will have to process
				 */
				virtual void handle (shared_ptr<CMessage> message)
				{
					std::string name = message->getData<std::string>();
					component->setCurrentAnimationState(name);
				}
		};


		COgreAnimatedMeshComponent::COgreAnimatedMeshComponent()
		{
			this->m_currentAnimation = "";
			this->m_parentPageName = "";
			this->m_parent = NULL;
		}


		COgreAnimatedMeshComponent::~COgreAnimatedMeshComponent()
		{
		}


		Gnoll::Scene::GObject * COgreAnimatedMeshComponent::getParent() const
		{
			return m_parent;
		}


		void COgreAnimatedMeshComponent::setParent(Gnoll::Scene::GObject *m_parent)
		{
			this->m_parent = m_parent;
		}


		const std::string& COgreAnimatedMeshComponent::getParentPageName() const
		{
			return m_parentPageName;
		}


		void COgreAnimatedMeshComponent::setParentPage(const std::string& m_parentPageName)
		{
			this->m_parentPageName = m_parentPageName;
		}


		void COgreAnimatedMeshComponent::setPosition(const Ogre::Vector3& position)
		{
			string instanceNameStr(this->getInstance());
			Ogre::SceneManager* sm = Gnoll::Graphic::CGraphicModule::getInstancePtr()->getSceneManager();

			SceneNode* meshNode = sm->getSceneNode( m_parentPageName + "_" + instanceNameStr );
			meshNode->translate(position, Ogre::Node::TS_LOCAL);
		}


		void COgreAnimatedMeshComponent::setScaling(const Ogre::Vector3& scale)
		{
			string instanceNameStr(this->getInstance());
			Ogre::SceneManager* sm = Gnoll::Graphic::CGraphicModule::getInstancePtr()->getSceneManager();

			SceneNode* meshNode = sm->getSceneNode( m_parentPageName + "_" + instanceNameStr );
			meshNode->setScale(scale.x, scale.y, scale.z);
		}


		void COgreAnimatedMeshComponent::setRotation(const Ogre::Vector3& rotation)
		{
			string instanceNameStr(this->getInstance());
			Ogre::SceneManager* sm = Gnoll::Graphic::CGraphicModule::getInstancePtr()->getSceneManager();

			SceneNode* meshNode = sm->getSceneNode( m_parentPageName + "_" + instanceNameStr );
			meshNode->rotate(Ogre::Vector3(1,0,0), Ogre::Radian(rotation.x));
			meshNode->rotate(Ogre::Vector3(0,1,0), Ogre::Radian(rotation.x));
			meshNode->rotate(Ogre::Vector3(0,0,1), Ogre::Radian(rotation.x));
		}


		void COgreAnimatedMeshComponent::setCurrentAnimationState(const std::string& name)
		{
			/**
			 * Check if the animation exist
			 */
			if(m_mapAnimationStates.find(name) == m_mapAnimationStates.end())
			{
				GNOLL_LOG() << name << " animation does not exit !\n";
				return;
			}


			/**
			 * Stop the previous animation
			 */
			if(m_currentAnimation != "")
			{
				m_mapAnimationStates[m_currentAnimation]->setEnabled(false);
				m_mapAnimationStates[m_currentAnimation]->setLoop(false);
			}
			m_currentAnimation = name;

			m_mapAnimationStates[m_currentAnimation]->setEnabled(true);
			m_mapAnimationStates[m_currentAnimation]->setLoop(true);
		}


		void COgreAnimatedMeshComponent::init(Gnoll::Scene::GObject* parent, CPage* page)
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

			this->m_parent         = parent;
			this->m_parentPageName = page->getInstance();

			shared_ptr< Gnoll::DynamicObject::String > meshName = this->getAttribute < Gnoll::DynamicObject::String > (COgreAnimatedMeshComponent::ATTRIBUTE_MESH());
			string meshNameStr(*meshName);

			SceneNode *parentNode = page->getPageRootNode();
			SceneNode *meshNode   = parentNode->createChildSceneNode( m_parentPageName + "_" + this->getInstance() );

			std::string entName = m_parentPageName + "_" + this->getInstance() + COgreAnimatedMeshComponent::ENTITY_SUFFIX();
			Ogre::SceneManager* sm = Gnoll::Graphic::CGraphicModule::getInstancePtr()->getSceneManager();

			Ogre::Entity *ent = sm->createEntity( entName, meshNameStr );
			meshNode->attachObject( ent );


			/**
			 * Check if its an animated mesh and get back the set of AnimationState
			 */
			AnimationStateSet *animationStateSet = ent->getAllAnimationStates();
			if(animationStateSet == 0)
			{
				GNOLL_LOG() << parent->getInstance() << ":" << this->getInstance() << " is not a animated mesh !" << "\n";
			}

			/**
			 * Select the first by default
			 */
			GNOLL_LOG() << this->getInstance() << " : Load animation states : " << "\n";
			AnimationStateIterator iter = animationStateSet->getAnimationStateIterator();
			while(iter.hasMoreElements())
			{
				AnimationState* animationState = iter.getNext();
				m_mapAnimationStates[animationState->getAnimationName()] = animationState;
				GNOLL_LOG() << this->getInstance() << " : - " << animationState->getAnimationName() << "\n";
			}


			/**
			 * Restore the default animation state, if none so no animation by default
			 */
			shared_ptr< Gnoll::DynamicObject::String > default_animationState ( new Gnoll::DynamicObject::String());
			m_currentAnimation = *(this->getAttributeOrDefault < Gnoll::DynamicObject::String > (COgreAnimatedMeshComponent::ATTRIBUTE_ANIMATIONSTATE(), default_animationState));

			if(animationStateSet->hasAnimationState(m_currentAnimation))
			{
				setCurrentAnimationState(m_currentAnimation);
			}
			else
			{
				m_currentAnimation = "";
			}

			/**
			 * Register the listener
			 */
			CMessageModule*  messageModule  = CMessageModule::getInstancePtr();
			Messages::Messenger* messageManager = messageModule->getMessageManager();

			componentListener = shared_ptr<Messages::Listener> (new OgreAnimatedMeshListener(this));
			componentPositionListener = shared_ptr<Messages::Listener> (new OgreAnimatedMeshPositionListener(this));
			componentScalingListener = shared_ptr<Messages::Listener> (new OgreAnimatedMeshScalingListener(this));
			componentRotationListener = shared_ptr<Messages::Listener> (new OgreAnimatedMeshRotationListener(this));
			componentAnimationStateListener = shared_ptr<Messages::Listener> (new OgreAnimatedMeshAnimationStateListener(this));

			messageManager->addListener ( componentListener, Gnoll::Core::Messages::MessageType("GRAPHIC_FRAME_RENDERED") );
			messageManager->addListener ( componentPositionListener, "SET_POSITION_" + m_parent->getInstance() );
			messageManager->addListener ( componentScalingListener, "SET_SCALING_" + m_parent->getInstance() );
			messageManager->addListener ( componentRotationListener, "SET_ROTATION_" + m_parent->getInstance() );
			messageManager->addListener ( componentAnimationStateListener, "SET_ANIMATIONSTATE_" + m_parent->getInstance() );
		}


		 void COgreAnimatedMeshComponent::exit()
		 {
			GNOLL_LOG() << "Gobject " << this->m_parent->getInstance() << " is DEinitializing its component named " << this->getInstance() << "\n";
			if (this->m_parentPageName == "")
			{

				GNOLL_LOG() << m_parent->getInstance() << ":" << this->getInstance() << " object cannot be DEinitialized without a parent page" << "\n";
				return;
			}

			string instanceNameStr(this->getInstance());

			std::string entName = m_parentPageName + "_" + instanceNameStr  + COgreAnimatedMeshComponent::ENTITY_SUFFIX();
			Ogre::SceneManager* sm = Gnoll::Graphic::CGraphicModule::getInstancePtr()->getSceneManager();

			SceneNode* meshNode = sm->getSceneNode( m_parentPageName + "_" + instanceNameStr );
			meshNode->detachObject( entName );

			sm->destroyEntity(entName);
			sm->destroySceneNode( m_parentPageName + "_" + instanceNameStr  );

			shared_ptr<Gnoll::DynamicObject::String> attribute_AnimationState = shared_ptr<Gnoll::DynamicObject::String>(
																				new Gnoll::DynamicObject::String(m_currentAnimation));

			this->setAttribute(COgreAnimatedMeshComponent::ATTRIBUTE_ANIMATIONSTATE(), attribute_AnimationState);

			/**
			 * Unregister the listener
			 */
			CMessageModule*  messageModule  = CMessageModule::getInstancePtr();
			Messages::Messenger* messageManager = messageModule->getMessageManager();

			messageManager->delListener ( componentListener, Gnoll::Core::Messages::MessageType("GRAPHIC_FRAME_RENDERED") );
			messageManager->delListener ( componentPositionListener, "SET_POSITION_" + m_parent->getInstance() );
			messageManager->delListener ( componentScalingListener, "SET_SCALING_" + m_parent->getInstance() );
			messageManager->delListener ( componentRotationListener, "SET_ROTATION_" + m_parent->getInstance() );
			messageManager->delListener ( componentAnimationStateListener, "SET_ANIMATIONSTATE_" + m_parent->getInstance() );
		}

		void COgreAnimatedMeshComponent::update(float time)
		{
			/**
			 * Update all animation that are enabled
			 */
			if(m_currentAnimation != "")
				m_mapAnimationStates[m_currentAnimation]->addTime(time);
		}

		shared_ptr<xmlpp::Document> COgreAnimatedMeshComponent::serializeXML()
		{
			shared_ptr<xmlpp::Document> document = DynamicObject::serializeXML();
			xmlpp::Element* root = document->get_root_node();

			if (root != NULL)
			{
				root->set_name( COgreAnimatedMeshComponent::DYNAMIC_OBJECT_NAME() );
			}

			return document;
		}

	}
}
