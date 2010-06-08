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

#include "../include/dynamicobjectmanager.h"

#include "../include/attributehandlerregistry.h"
#include "../../log/include/clogmacros.h"

namespace Gnoll 
{
	namespace DynamicObject 
	{
		DynamicObjectManager::DynamicObjectManager()
		{
		}

		DynamicObjectManager::~DynamicObjectManager()
		{
			GNOLL_LOG().logMessage("Deleting DynamicObjectManager");
		}

		shared_ptr<DynamicObject> DynamicObjectManager::loadImpl(shared_ptr<AbstractStream> stream, 
																 string instance)
		{
			// First : Stream -> XML
			// The file is read and stored in a string
			// Then this string is parsed by a DOM parser

			char buffer[256];
			string content;

			while(!stream->eof())
			{
				int nb = stream->read(buffer, 256);
				content.append(buffer, nb);
			}

			xmlpp::Document* document;
			shared_ptr<DynamicObject> po(new DynamicObject(instance));

			// Second : XML -> DynamicObject
			// Only if some data are available
			if(content.length() > 0)
			{
				xmlpp::DomParser parser;
				parser.parse_memory(content);

				document = parser.get_document();

				xmlpp::Element* root = document->get_root_node();

				po->deSerializeXML(root);
			}

			return po;
		}

		bool DynamicObjectManager::saveImpl(shared_ptr<AbstractStream> stream, 
											shared_ptr<DynamicObject> obj, 
											string instance)
		{
			shared_ptr<xmlpp::Document> output = obj->serializeXML();
			string outputString = output->write_to_string_formatted();

			unsigned int bytesWritten = stream->write(outputString.c_str() , outputString.length());

			if (bytesWritten)
				return true;
			else
				return false;
		}
	}
}
