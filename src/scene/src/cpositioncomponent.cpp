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


/*--------------------------CPositionComponent-----------------------------*\
|   This is a component able to set a position                              |
|                                                                           |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include "../../dynamicobject/include/vector3.h"
#include "../../log/include/clogmacros.h"
#include "../../graphic/include/cgraphicmodule.h"

#include "../include/cpositioncomponent.h"
#include "../../core/include/cmessage.h"
#include "../../core/messages/include/messagetype.h"
#include "../../core/messages/include/messenger.h"
#include "../../core/include/cmessagemodule.h"
#include "../../core/messages/include/listener.h"

using namespace Gnoll::DynamicObject;
using namespace Gnoll::Log;

namespace Gnoll {

	namespace Scene {

		class PositionListener : public Messages::Listener
		{
			private:
				CPositionComponent* positionComponent;

			public:

				/**
				 * This is a constructor
				 */
				PositionListener(CPositionComponent* _positionComponent) : positionComponent(_positionComponent) {}

				/**
				 * This is a destructor
				 */
				virtual ~PositionListener() {}

				/**
				 * This method is called in order to process a message
				 * @param message The message this method will have to process
				 */
				virtual void handle (shared_ptr<CMessage> message)
				{
					Ogre::Vector3 pos = message->getData<Ogre::Vector3>();
					positionComponent->setPosition(pos);
				}
		};

		CPositionComponent::CPositionComponent()
		{
			parent = NULL;
		}


		CPositionComponent::~CPositionComponent()
		{
		}


		void CPositionComponent::init(Gnoll::Scene::GObject* _parent, CPage* _page)
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
			CMessageModule*  messageModule  = CMessageModule::getInstancePtr();
			Messages::Messenger* messageManager = messageModule->getMessageManager();

			positionListener = shared_ptr<Messages::Listener> (new PositionListener(this));
			messageManager->addListener ( positionListener, "SET_POSITION_" + parent->getInstance() );


			shared_ptr< Gnoll::DynamicObject::Vector3 > zero ( new Gnoll::DynamicObject::Vector3());
			position = *(this->getAttributeOrDefault < Gnoll::DynamicObject::Vector3 > (CPositionComponent::ATTRIBUTE_POSITION(), zero));


			/**
			 * Queue the message of the init position
			 */
			Messages::MessageType messageType("SET_POSITION_" + parent->getInstance());
			shared_ptr<boost::any> data(new boost::any(position)) ;
			shared_ptr<CMessage> message = shared_ptr<CMessage>(new CMessage(messageType, data));
			messageManager->queueMessage(message);
		}


		void CPositionComponent::exit()
		{
			/**
			 * Unregister the listener
			 */
			CMessageModule*  messageModule  = CMessageModule::getInstancePtr();
			Messages::Messenger* messageManager = messageModule->getMessageManager();

			messageManager->delListener ( positionListener, "SET_POSITION_" + parent->getInstance() );

			this->setAttribute(CPositionComponent::ATTRIBUTE_POSITION(),
							   shared_ptr<Gnoll::DynamicObject::Vector3> (new Gnoll::DynamicObject::Vector3(position)));
		}


		shared_ptr<xmlpp::Document> CPositionComponent::serializeXML()
		{
			shared_ptr<xmlpp::Document> document = DynamicObject::serializeXML();
			xmlpp::Element* root = document->get_root_node();

			if (root != NULL)
			{
				root->set_name( CPositionComponent::DYNAMIC_OBJECT_NAME() );
			}

			return document;
		}

	}
}

