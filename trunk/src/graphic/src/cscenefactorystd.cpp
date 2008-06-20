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


/*-------------------------cscenefactorystd.cpp----------------------------*\
|   This is the scene factory standard implementation                       |
|                                                                           |
|   Changelog :                                                             |
|               06/24/2006 - Paf - Initial release                          |
|               13/07/2007 - Yellow - Change Glib::ustring to std::string   |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../include/cscenefactorystd.h"
#include "../include/csceneparser.h"
#include "../../log/include/clogmodule.h"



CSceneFactoryStd::CSceneFactoryStd(const std::string filename, Ogre::Root *root) :
																	 m_filename(filename),
																	 m_root(root)
{

}


CSceneFactoryStd::~CSceneFactoryStd()
{

}


void CSceneFactoryStd::loadScene()
{
	// Parse the entire document in one go:
	try
	{
		CSceneParser parser(m_root);
		parser.set_substitute_entities(true); //
		parser.parse_file(m_filename);
	}
	catch(const xmlpp::exception& ex)
	{
		std::ostringstream tmpString;
		tmpString << "libxml++ exception: " << ex.what();
		Gnoll::Log::CLogModule::getInstancePtr()->logMessage( tmpString.str() );
	}
}
