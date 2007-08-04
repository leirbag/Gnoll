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


/*-------------------------------cmessage----------------------------------*\
|   This is a message                                                       |
|                                                                           |
|   Changelog :                                                             |
|               07/09/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../include/persistentobject.h"

PersistentObject::PersistentObject(Glib::ustring _instance) : m_instance(_instance)
{
}


PersistentObject::~PersistentObject() 
{
}

void PersistentObject::setAttribute( const Glib::ustring _name, shared_ptr<IAttribute> _value )
{
	m_attributes[_name] = _value;
}

bool PersistentObject::hasAttribute ( Glib::ustring _name )
{
	mapAttributes::iterator iter = m_attributes.find(_name);
	
	return iter != m_attributes.end() ;
}


void PersistentObject::deleteAttribute ( Glib::ustring _name )
{
	mapAttributes::iterator iter = m_attributes.find(_name);
	
	if( iter == m_attributes.end() ) {
		return;
	}
	
	m_attributes.erase(iter);
}

void PersistentObject::deleteAllAttributes ( void ) 
{
	m_attributes.clear();
}		  

string PersistentObject::getInstance()
{
	return m_instance;
}


shared_ptr<xmlpp::Document> PersistentObject::serializeXML() 
{
	// First a new document is created
	shared_ptr<xmlpp::Document> document( new xmlpp::Document("1.0"));  

	// The root node will contain the whole object
	xmlpp::Element* root = document->create_root_node("persistentobject");
	root->set_attribute("instance", this->getInstance());


	// Each attribute will be serialiazed and attached to the root node
	mapAttributes::iterator iter;   
	for( iter = m_attributes.begin(); iter != m_attributes.end(); iter++ ) {

		// First we set up a node with the informations about the attribute
		// ex : instance name
		xmlpp::Element* child = root->add_child("attribute");
		child->set_attribute("name", iter->first);

		// Then the content of the serialized attribute is attached
		// as a child to the attribute node
		// So we will be able to add some information about the attribute 
		// without being worried of messing around
		shared_ptr<IAttribute> attr = iter->second;
		shared_ptr<xmlpp::Document> serializedAttr = attr->serializeXML();
		child->import_node(serializedAttr->get_root_node());
	}


	return document;
}		


