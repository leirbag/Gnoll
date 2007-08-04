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


/*---------------------------csceneparser.cpp------------------------------*\
|   This is the scene parser                                                |
|                                                                           |
|   Changelog :                                                             |
|               01/07/2007 - Paf - Initial release                          |
|               03/25/2007 - Paf - Add "visible" attribute to "mesh" node   |
|               13/07/2007 - Yellow - Change Glib::ustring to std::string   |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../include/csceneparser.h"

#include <iostream>
#include <stdlib.h>


CSceneParser::CSceneParser(Ogre::Root *root)
  : xmlpp::SaxParser(), m_root(root)
{
	if (m_root)
	{
		m_parentnode = m_root->getSceneManager("TSM")->getRootSceneNode();
	}
}

CSceneParser::~CSceneParser()
{
}

void CSceneParser::on_start_document()
{
	Ogre::LogManager::getSingleton().logMessage("Begin to parse the scene..." );
}

void CSceneParser::on_end_document()
{
	Ogre::LogManager::getSingleton().logMessage("Finished to parse the scene..." );
}

void CSceneParser::on_start_element(const std::string& name,
                                   const AttributeList& attributes)
{
  std::cout << "node name=" << name << std::endl;

  // Print attributes:
  for(xmlpp::SaxParser::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
  {
    std::cout << "  Attribute " << iter->name << " = " << iter->value << std::endl;
  }


	m_currentnodetype.push(name);


	if (name == "node")
	{
		std::string nodename;

		for(xmlpp::SaxParser::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
		{
			if (iter->name == "name")
				nodename = iter->value;			
		}

		m_parentnode = m_parentnode->createChildSceneNode(nodename);
		m_nodes.push(m_parentnode);

	} else if (name == "position")
	{
		float x, y, z;

		for(xmlpp::SaxParser::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
		{
			if (iter->name == "x")
				x = atof((iter->value).c_str());			
			if (iter->name == "y")
				y = atof((iter->value).c_str());			
			if (iter->name == "z")
				z = atof((iter->value).c_str());			
		}
		
		Ogre::SceneNode * parent = m_nodes.top();
		parent->translate(x, y, z);

	} else if (name == "rotation")
	{
		float x, y, z;

		for(xmlpp::SaxParser::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
		{
			if (iter->name == "x")
				x = atof((iter->value).c_str());			
			if (iter->name == "y")
				y = atof((iter->value).c_str());			
			if (iter->name == "z")
				z = atof((iter->value).c_str());			
		}

		Ogre::SceneNode * parent = m_nodes.top();
		
		parent->rotate(Ogre::Vector3(1,0,0), Ogre::Radian(x));
		parent->rotate(Ogre::Vector3(0,1,0), Ogre::Radian(y));
		parent->rotate(Ogre::Vector3(0,0,1), Ogre::Radian(z));

	} else if (name == "scale")
	{
		float x, y, z;

		for(xmlpp::SaxParser::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
		{
			if (iter->name == "x")
				x = atof((iter->value).c_str());			
			if (iter->name == "y")
				y = atof((iter->value).c_str());			
			if (iter->name == "z")
				z = atof((iter->value).c_str());			
		}

		Ogre::SceneNode * parent = m_nodes.top();

		parent->setScale(x,y ,z);

	} else if (name == "mesh")
	{
		std::string filename;
		std::string entname;
		bool visible = true;

		for(xmlpp::SaxParser::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
		{
			if (iter->name == "name")
				entname = iter->value;			
			if (iter->name == "filename")
				filename = iter->value;			

			if (iter->name == "visible") {
				if (iter->value == "false") {
					visible = false;			
					Ogre::LogManager::getSingleton().logMessage("Invisible !" );

				}
			}
		}

		Ogre::SceneNode * parent = m_nodes.top();

		std::string entnodename = entname + "_node";
		Ogre::SceneNode* entnode = parent->createChildSceneNode(entnodename);
		Ogre::SceneManager* sm = m_root->getSceneManager("TSM");
		if (visible)
		{
			Ogre::Entity *ent = sm->createEntity( entname, filename );
			entnode->attachObject( ent );
		}
		m_nodes.push(entnode);

	}


}

void CSceneParser::on_end_element(const std::string& name)
{
	m_currentnodetype.pop();

	if (name == "node")
		m_nodes.pop();

	if (name == "mesh") 
		m_nodes.pop();

}

void CSceneParser::on_characters(const std::string& text)
{
}

void CSceneParser::on_comment(const std::string& text)
{
}

void CSceneParser::on_warning(const std::string& text)
{
	Ogre::LogManager::getSingleton().logMessage("Warning : " + std::string(text) );
}

void CSceneParser::on_error(const std::string& text)
{
	Ogre::LogManager::getSingleton().logMessage("Error : " + std::string(text) );
}

void CSceneParser::on_fatal_error(const std::string& text)
{
	Ogre::LogManager::getSingleton().logMessage("Fatal error : " + std::string(text) );
}
