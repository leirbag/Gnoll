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


/*------------------------------csceneparser.h------------------------------*\
|   This is the scene factory interface                                      |
|                                                                            |
|   Changelog :                                                              |
|               01/07/2007 - Paf - Initial release                           |
|               13/07/2007 - Yellow - Change Glib::ustring to std::string    |
|                                                                            |
\*--------------------------------------------------------------------------*/


#include <boost/shared_ptr.hpp>
#include <Ogre.h>
#include <OgreLogManager.h>
#include <OgrePlatformManager.h>
#include <libxml++/libxml++.h>
#include <stack>

#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif


using namespace std;

#ifndef __CSCENEPARSER_H__
#define __CSCENEPARSER_H__


/**
 * The Scene Factory standard implementation
 */ 
class  CSceneParser : public xmlpp::SaxParser
{
	public:
		CSceneParser(Ogre::Root *root);
		virtual ~CSceneParser();

	protected:
		//overrides:
		virtual void on_start_document();
		virtual void on_end_document();
		virtual void on_start_element(const std::string& name,
												const AttributeList& properties);
		virtual void on_end_element(const std::string& name);
		virtual void on_characters(const std::string& characters);
		virtual void on_comment(const std::string& text);
		virtual void on_warning(const std::string& text);
		virtual void on_error(const std::string& text);
		virtual void on_fatal_error(const std::string& text);

	private:

		Ogre::Root *m_root;

		stack<std::string> m_currentnodetype;
		stack<Ogre::SceneNode*> m_nodes;
		Ogre::SceneNode *m_parentnode;
};

#endif // __CSCENEPARSER_H__
