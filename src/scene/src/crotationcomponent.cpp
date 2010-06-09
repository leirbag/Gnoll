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

#include "../../dynamicobject/include/vector3.h"
#include "../../log/include/logmacros.h"
#include "../../graphic/include/cgraphicmodule.h"

#include "../include/crotationcomponent.h"
#include "../../core/include/message.h"
#include "../../core/messages/include/messagetype.h"
#include "../../core/messages/include/messenger.h"
#include "../../core/include/messagemodule.h"
#include "../../core/messages/include/listener.h"

using namespace Gnoll::DynamicObject;
using namespace Gnoll::Log;

namespace Gnoll {

	namespace Scene {

		class RotationListener : public Messages::Listener
		{
			private:
				CRotationComponent* rotationComponent;

			public:

				/**
				 * This is a constructor
				 */
				RotationListener(CRotationComponent* _rotationComponent) : rotationComponent(_rotationComponent) {}

				/**
				 * This is a destructor
				 */
				virtual ~RotationListener() {}

				/**
				 * This method is called in order to process a message
				 * @param message The message this method will have to process
				 */
				virtual void handle (shared_ptr<Message> message)
				{
					Ogre::Vector3 pos = message->getData<Ogre::Vector3>();
					rotationComponent->setRotation(pos);
				}
		};

		CRotationComponent::CRotationComponent()
		{
			parent = NULL;
		}


		CRotationComponent::~CRotationComponent()
		{
		}


		void CRotationComponent::init(Gnoll::Scene::GObject* _parent, CPage* _page)
		{
			parent = _parent;

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
			if (_page == NULL)
			{
				GNOLL_LOG() << parent->getInstance() << ":" << this->getInstance() << " object initialized without any parent page" << "\n";
				return;
			}


			/**
			 * Register the listener
			 */
			MessageModule*  messageModule  = MessageModule::getInstancePtr();
			Messages::Messenger* messageManager = messageModule->getMessageManager();

			rotationListener = shared_ptr<Messages::Listener> (new RotationListener(this));
			messageManager->addListener ( rotationListener, "SET_ROTATION_" + parent->getInstance() );


			shared_ptr< Gnoll::DynamicObject::Vector3 > zero ( new Gnoll::DynamicObject::Vector3());
			rotation = *(this->getAttributeOrDefault < Gnoll::DynamicObject::Vector3 > (CRotationComponent::ATTRIBUTE_ROTATION(), zero));


			/**
			 * Queue the message of the init rotation
			 */
			Messages::MessageType messageType("SET_ROTATION_" + parent->getInstance());
			shared_ptr<boost::any> data(new boost::any(rotation)) ;
			shared_ptr<Message> message = shared_ptr<Message>(new Message(messageType, data));
			messageManager->queueMessage(message);
		}


		void CRotationComponent::exit()
		{
			/**
			 * Unregister the listener
			 */
			MessageModule*  messageModule  = MessageModule::getInstancePtr();
			Messages::Messenger* messageManager = messageModule->getMessageManager();

			messageManager->delListener ( rotationListener, "SET_ROTATION_" + parent->getInstance() );

			this->setAttribute(CRotationComponent::ATTRIBUTE_ROTATION(),
							   shared_ptr<Gnoll::DynamicObject::Vector3> (new Gnoll::DynamicObject::Vector3(rotation)));
		}


		shared_ptr<xmlpp::Document> CRotationComponent::serializeXML()
		{
			shared_ptr<xmlpp::Document> document = DynamicObject::serializeXML();
			xmlpp::Element* root = document->get_root_node();

			if (root != NULL)
			{
				root->set_name( CRotationComponent::DYNAMIC_OBJECT_NAME() );
			}

			return document;
		}

	}
}


