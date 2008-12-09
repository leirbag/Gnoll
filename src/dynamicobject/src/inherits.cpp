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


/*---------------------------------inherits--------------------------------*\
|   This is an inheritance attribute for DynamicObject                      |
|                                                                           |
|   Changelog :                                                             |
|               09/27/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../include/inherits.h"
#include <iostream>


namespace Gnoll {

	namespace DynamicObject {


		Inherits::Inherits(String _parent): m_parent(_parent)
		{
		}


		Inherits::~Inherits()
		{
		}


		String Inherits::getParent()
		{
			return m_parent;
		}


		void Inherits::setParent (String _parent)
		{
			m_parent = _parent;
		}


		shared_ptr<xmlpp::Document> Inherits::serializeXML() 
		{
			shared_ptr<xmlpp::Document> document( new xmlpp::Document("1.0"));  

			xmlpp::Element* root = document->create_root_node( "inherits" );
	
			/**
			 * We are going to serialize the parent and add it as a children of the inheritance node
			 */
			shared_ptr< xmlpp::Document > docChild = m_parent.serializeXML();
			xmlpp::Element * rootChild = docChild->get_root_node();
			root->import_node(rootChild);

			return document;
		};		


		void Inherits::deSerializeXML( xmlpp::Element* _element ) 
		{
					
			if (_element == NULL)
			{
				return;
			}

			if (_element->get_name() != "inherits")
			{
				return;
			}

			// We need a handler to deserialize attributes
			Gnoll::DynamicObject::AttributeHandlerRegistry* registry = Gnoll::DynamicObject::AttributeHandlerRegistry::getInstancePtr();


			xmlpp::Node::NodeList list = _element->get_children();
			for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter)
			{
				string name= (*iter)->get_name();
				xmlpp::Element* elementChild = dynamic_cast<xmlpp::Element*>(*iter);

				if (elementChild && (name == "string"))
				{

					shared_ptr<Gnoll::DynamicObject::IAttributeHandler> handler = registry->getHandler(name);
	
					// Check if a handler is available for this attribute
					if (handler.get() != NULL)
					{
						// The handler returns an shared_pointer to a new IAttribute
						shared_ptr<IAttribute> attribute = handler->handle(elementChild);

						// This new IAttribute is added to the list
						m_parent = *(dynamic_pointer_cast<String>(attribute));
					}
				}

			}

		};		

	}
}




