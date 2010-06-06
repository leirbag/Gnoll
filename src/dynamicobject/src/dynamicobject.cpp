/***************************************************************************
 *   Copyright (C) 2006 by Puzzle Team                                     *
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

#include "../include/dynamicobject.h"

#include "../../log/include/clogmacros.h"

namespace Gnoll
{
	namespace DynamicObject
	{
		typedef shared_ptr<AbstractAttribute> AbstractAttribute_ptr;
		typedef shared_ptr<AbstractAttributeHandler> AbstractAttributeHandler_ptr;

		DynamicObject::DynamicObject(Glib::ustring instance) : m_instance(instance)
		{
		}

		DynamicObject::~DynamicObject()
		{
		}

		void DynamicObject::setAttribute( const Glib::ustring name, shared_ptr<AbstractAttribute> value )
		{
			m_attributes[name] = value;
		}

		bool DynamicObject::hasAttribute(const Glib::ustring name) const
		{
			mapAttributes::const_iterator iter = m_attributes.find(name);
			return iter != m_attributes.end() ;
		}

		void DynamicObject::deleteAttribute(Glib::ustring name)
		{
			mapAttributes::iterator iter = m_attributes.find(name);

			if(iter == m_attributes.end())
				return;

			m_attributes.erase(iter);
		}

		void DynamicObject::deleteAllAttributes()
		{
			m_attributes.clear();
		}

		List DynamicObject::getAttributesNames()
		{
			List attrNames;
			for(mapAttributes::const_iterator iter = m_attributes.begin(); 
				iter != m_attributes.end(); iter++)
			{
				shared_ptr<String> attrName(new String((*iter).first));
				attrNames.push_back(attrName);
			}

			return attrNames;
		}

		DynamicObject::mapAttributes::const_iterator DynamicObject::begin() const
		{
			return m_attributes.begin();
		}

		DynamicObject::mapAttributes::const_iterator DynamicObject::end() const
		{
			return m_attributes.end();
		}

		string DynamicObject::getInstance()
		{
			return m_instance;
		}

		shared_ptr<xmlpp::Document> DynamicObject::serializeXML()
		{
			// First a new document is created
			shared_ptr<xmlpp::Document> document(new xmlpp::Document("1.0"));

			// The root node will contain the whole object
			xmlpp::Element* root = document->create_root_node("dynamicobject");
			root->set_attribute("instance", this->getInstance());


			// Each attribute will be serialiazed and attached to the root node
			mapAttributes::iterator iter;
			for(iter = m_attributes.begin(); iter != m_attributes.end(); iter++) {

				// First we set up a node with the informations about the attribute
				// ex : instance name
				xmlpp::Element* child = root->add_child("attribute");
				child->set_attribute("name", iter->first);

				// Then the content of the serialized attribute is attached
				// as a child to the attribute node
				// So we will be able to add some information about the attribute
				// without being worried of messing around
				shared_ptr<AbstractAttribute> attr = iter->second;
				shared_ptr<xmlpp::Document> serializedAttr = attr->serializeXML();
				child->import_node(serializedAttr->get_root_node());
			}

			return document;
		}

		/**
		 * Example of a serialized DynamicObject :
		 *
		 * <dynamicobject instance="zelda">
		 *   <attribute name="age">
		 *     <integer value="345"/>
		 *   </attribute>
		 *   <attribute name="phrase">
		 *     <string value="c'etait un cafard qui vivait dans le noir"/>
		 *   </attribute>
		 *   <attribute name="pi">
		 *     <float value="3.145"/>
		 *   </attribute>
		 * </dynamicobject>
		 *
		 */
		void DynamicObject::deSerializeXML(xmlpp::Element* element)
		{
			if (element	== NULL)
				return;

			xmlpp::Attribute* instanceName = element->get_attribute("instance");
			if(instanceName)
				this->m_instance = instanceName->get_value();

			xmlpp::Node::NodeList nodeList = element->get_children("attribute");

			// Looping through all attributes
			for(xmlpp::Node::NodeList::iterator it = nodeList.begin(); it != nodeList.end(); it++)
			{
				xmlpp::Attribute* attr = NULL;

				xmlpp::Element* tmpElement = dynamic_cast<xmlpp::Element*>(*it);
				if (tmpElement)
					attr = tmpElement->get_attribute("name");

				// There must be an attribute name
				if (attr != NULL)
				{
					string attrName = attr->get_value();

					xmlpp::Node::NodeList childrenList = (*it)->get_children();

					// Looping through all attributes' children
					// Only the last child will be taken in account and associated to the attribute name
					for(xmlpp::Node::NodeList::iterator itChildren = childrenList.begin(); 
						itChildren != childrenList.end(); 
						itChildren++)
					{
						// Get the type of the attribute
						string name= (*itChildren)->get_name();

						xmlpp::Element* elementChild = dynamic_cast<xmlpp::Element*>(*itChildren);

						if(elementChild)
						{
							// We need a handler for this attribute
							AttributeHandlerRegistry* registry;
							registry = AttributeHandlerRegistry::getInstancePtr();

							GNOLL_LOG().logMessage("DO: Looking for a handler for attribute " + name);
							AbstractAttributeHandler_ptr handler = registry->getHandler(name);

							// Check if a handler is available for this attribute
							if(handler.get() != NULL)
							{
								GNOLL_LOG().logMessage("DO: Handler found for attribute " + name);
								// The handler returns an shared_pointer to a new AbstractAttribute
								AbstractAttribute_ptr attribute = handler->handle(elementChild, this);

								// This new AbstractAttribute is associated to the attribute name
								this->setAttribute(attrName, attribute);
							}
						}
					}
				}
			}
		}
	}
}
