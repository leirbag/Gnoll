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


/*--------------------------------isource----------------------------------*\
|   This is the interface of all the attributes. Each Attribute has to be   |
|     (de)serializable                                                      |
|                                                                           |
|   Changelog :                                                             |
|               07/11/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../include/sourcefile.h"


SourceFile::SourceFile( const string _path, bool _overWrite) : m_path(_path), m_overWrite(_overWrite)
{

}


SourceFile::~SourceFile()
{
}


shared_ptr<IStream> SourceFile::load( const string _url)
{
	shared_ptr<IStream> file ( new FileStream( m_path + string("/") + _url, m_overWrite ));
	return file;
}


bool SourceFile::isFetchable( const string _url)
{
	if ( ! _url.find(m_path))
	{
			  return false;
	}

	return 1; //Glib::file_test(_url, Glib::FILE_TEST_EXISTS);
}


void SourceFile::setOverWrite(bool _mode)
{
	m_overWrite = _mode;
}

