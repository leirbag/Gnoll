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


/*---------------------------------list------------------------------------*\
|   This is a list attribute for PersistentObject                           |
|                                                                           |
|   Changelog :                                                             |
|               09/27/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../include/list.h"



namespace Gnoll {

	namespace Core {


		List::List()
		{
		}


		shared_ptr<xmlpp::Document> List::serializeXML() 
		{
			shared_ptr<xmlpp::Document> document( new xmlpp::Document("1.0"));  

			xmlpp::Element* root = document->create_root_node( "list" );
	
			/**
			 * We are going to serialize each child and add them as children of the list node
			 */
			for (list< shared_ptr<IAttribute> >::iterator it = this->begin(); it != this->end(); it++)
			{
				shared_ptr< xmlpp::Document > docChild = (*it)->serializeXML();
				xmlpp::Element * rootChild = docChild->get_root_node();
				root->import_node(rootChild);
			}

			return document;
		};		


		void List::deSerializeXML( xmlpp::Element* _element ) 
		{
					
			if (_element == NULL)
			{
				return;
			}

			if (_element->get_name() != "list")
			{
				return;
			}

			// We need a handler to deserialize attributes
			Gnoll::Core::AttributeHandlerRegistry registry = Gnoll::Core::AttributeHandlerRegistry::getInstance();


			xmlpp::Node::NodeList list = _element->get_children();
			for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter)
			{
				string name= (*iter)->get_name();

				xmlpp::Element* elementChild = dynamic_cast<xmlpp::Element*>(*iter);

				if (elementChild)
				{

					shared_ptr<Gnoll::Core::IAttributeHandler> handler = registry.getHandler(name);
	
					// Check if a handler is available for this attribute
					if (handler.get() != NULL)
					{
						// The handler returns an shared_pointer to a new IAttribute
						shared_ptr<IAttribute> attribute = handler->handle(elementChild);

						// This new IAttribute is added to the list
						this->push_back(attribute);
					}
				}

			}

		};		

	}
}




