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
#include "../../log/include/clogmacros.h"
#include "../../graphic/include/cgraphicmodule.h"

#include "../include/cscalingcomponent.h"
#include "../../core/include/cmessage.h"
#include "../../core/messages/include/messagetype.h"
#include "../../core/messages/include/messenger.h"
#include "../../core/include/cmessagemodule.h"
#include "../../core/messages/include/listener.h"

using namespace Gnoll::DynamicObject;
using namespace Gnoll::Log;

namespace Gnoll {

	namespace Scene {

		class ScalingListener : public Messages::Listener
		{
			private:
				CScalingComponent* scaleComponent;

			public:

				/**
				 * This is a constructor
				 */
				ScalingListener(CScalingComponent* _scaleComponent) : scaleComponent(_scaleComponent) {}

				/**
				 * This is a destructor
				 */
				virtual ~ScalingListener() {}

				/**
				 * This method is called in order to process a message
				 * @param message The message this method will have to process
				 */
				virtual void handle (shared_ptr<CMessage> message)
				{
					Ogre::Vector3 scale = message->getData<Ogre::Vector3>();
					scaleComponent->setScaling(scale);
				}
		};

		CScalingComponent::CScalingComponent()
		{
			parent = NULL;
		}


		CScalingComponent::~CScalingComponent()
		{
		}


		void CScalingComponent::init(Gnoll::Scene::GObject* _parent, CPage* _page)
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

			scalingListener = shared_ptr<Messages::Listener> (new ScalingListener(this));
			messageManager->addListener ( scalingListener, "SET_SCALING_" + parent->getInstance() );


			shared_ptr< Gnoll::DynamicObject::Vector3 > one ( new Gnoll::DynamicObject::Vector3(Ogre::Vector3(1, 1, 1)));
			scale = *(this->getAttributeOrDefault < Gnoll::DynamicObject::Vector3 > (CScalingComponent::ATTRIBUTE_SCALE(), one));


			/**
			 * Queue the message of the init position
			 */
			Messages::MessageType messageType("SET_SCALING_" + parent->getInstance());
			shared_ptr<boost::any> data(new boost::any(scale)) ;
			shared_ptr<CMessage> message = shared_ptr<CMessage>(new CMessage(messageType, data));
			messageManager->queueMessage(message);
		}


		void CScalingComponent::exit()
		{
			/**
			 * Unregister the listener
			 */
			CMessageModule*  messageModule  = CMessageModule::getInstancePtr();
			Messages::Messenger* messageManager = messageModule->getMessageManager();

			messageManager->delListener ( scalingListener, "SET_SCALING_" + parent->getInstance() );

			this->setAttribute(CScalingComponent::ATTRIBUTE_SCALE(),
							   shared_ptr<Gnoll::DynamicObject::Vector3> (new Gnoll::DynamicObject::Vector3(scale)));
		}


		shared_ptr<xmlpp::Document> CScalingComponent::serializeXML()
		{
			shared_ptr<xmlpp::Document> document = DynamicObject::serializeXML();
			xmlpp::Element* root = document->get_root_node();

			if (root != NULL)
			{
				root->set_name( CScalingComponent::DYNAMIC_OBJECT_NAME() );
			}

			return document;
		}

	}
}


