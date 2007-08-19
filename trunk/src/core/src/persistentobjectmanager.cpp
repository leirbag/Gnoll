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


/*-----------------------------filestream----------------------------------*\
|   This is a stream based on files                                         |
|                                                                           |
|   Changelog :                                                             |
|               07/18/2007 - Paf - Initial release                          |
|               08/18/2007 - Paf - Update comments                          |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../include/persistentobjectmanager.h"



namespace Viracocha {

	namespace Core {

		PersistentObjectManager::PersistentObjectManager()
		{
		

		}

		PersistentObjectManager::~PersistentObjectManager()
		{
		}


		shared_ptr<PersistentObject> PersistentObjectManager::loadImpl( shared_ptr<IStream> _stream)
		{

			// First : Stream -> XML
			// The file is read and stored in a string
			// Then this string is parsed by a DOM parser

			char buffer[256];
			string content;

			while (! _stream->eof())
			{
				int nb = _stream->read(buffer, 256);
				content.append(buffer, nb);
			}
			xmlpp::DomParser parser;
			parser.parse_memory(content);

			xmlpp::Document* document = parser.get_document();

			// Second : XML -> PersistentObject
			
			xmlpp::Element* root = document->get_root_node();

			shared_ptr<PersistentObject> po (new PersistentObject("NULL"));

			po->deSerializeXML(root);

			return po;
		}

		bool PersistentObjectManager::saveImpl( shared_ptr<IStream> _stream, shared_ptr<PersistentObject> _obj)
		{
			shared_ptr<xmlpp::Document> output = _obj->serializeXML();
			string outputString = output->write_to_string_formatted();

			unsigned int bytesWritten = _stream->write( outputString.c_str() , outputString.length());

			if (bytesWritten){
				return true;
			} else {
				return false;
			}

		}

	}
}




