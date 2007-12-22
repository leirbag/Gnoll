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
|               07/13/2007 - Yellow - Change Glib::ustring to std::string    |
|               12/21/2007 - Paf - Revert Yellow's previous change, because  |
|                                    that was breaking all the parsing steps |
|                                    since on_start_element() signature      |
|                                    has changed                             |
|                                                                            |
\*--------------------------------------------------------------------------*/


#include <boost/shared_ptr.hpp>
#include <Ogre.h>
#include <OgreLogManager.h>
#include <OgrePlatform.h>
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
		virtual void on_start_element(const Glib::ustring& name,
												const AttributeList& properties);
		virtual void on_end_element(const Glib::ustring& name);
		virtual void on_characters(const Glib::ustring& characters);
		virtual void on_comment(const Glib::ustring& text);
		virtual void on_warning(const Glib::ustring& text);
		virtual void on_error(const Glib::ustring& text);
		virtual void on_fatal_error(const Glib::ustring& text);

	private:

		Ogre::Root *m_root;

		stack<Glib::ustring> m_currentnodetype;
		stack<Ogre::SceneNode*> m_nodes;
		Ogre::SceneNode *m_parentnode;
};

#endif // __CSCENEPARSER_H__
