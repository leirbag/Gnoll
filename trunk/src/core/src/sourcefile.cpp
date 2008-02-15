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
|               08/17/2007 - Paf - Update namespace                         |
|               08/18/2007 - Paf - Implement SourceFile::isFetcheable()     |
|               09/25/2007 - Paf - Replace namespace Viracocha by Gnoll     |
|               02/15/2008 - Bruno Mahe - ISource objects provide a new     |
|                                    method to check if a source can        |
|                                    find a writable stream                 |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../include/sourcefile.h"
#include "boost/filesystem/path.hpp" 
#include "boost/filesystem/convenience.hpp"
#include "boost/filesystem/operations.hpp" 

using namespace boost::filesystem;       


namespace Gnoll 
{
	namespace Core 
	{

		SourceFile::SourceFile( const string _path, bool _overWrite, unsigned int _priority) : ISource(_priority), m_path(_path), m_overWrite(_overWrite)
		{

		}


		SourceFile::~SourceFile()
		{
		}

		shared_ptr<IStream> SourceFile::newStream(const string _url)
		{
			shared_ptr<IStream> file ( new FileStream( m_path + string("/") + _url, m_overWrite ));
			return file;
		}



		shared_ptr<IStream> SourceFile::loadStream( const string _url)
		{
			return newStream(_url);
		}

		shared_ptr<IStream> SourceFile::saveStream( const string _url)
		{
			setOverWrite(true);
			return newStream(_url);
		}

		bool SourceFile::isFetchable( const string _url)
		{

   		bool result; 
			path file( m_path);	
			file = file / _url;

			result = exists( file );

			if (m_overWrite)
			{
				return true;
			}
			return result;

		}


		bool SourceFile::isWritable( const string _url)
		{

			bool result;
			path file( m_path);
			file = file / _url;

			result = exists( file.branch_path() );

		 	return result;

		}


		void SourceFile::setOverWrite(bool _mode)
		{
			m_overWrite = _mode;
		}
	}
}

