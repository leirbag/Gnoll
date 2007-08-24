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
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../include/sourcefile.h"
#include <stddef.h>
#include <sys/types.h>
#include <dirent.h>


namespace Viracocha 
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

			DIR *dp;
			struct dirent *ep;
     
			unsigned int pathLimit = _url.find_last_of("/");
		
			string directory = m_path;
	
			// Add a trailing '/' to the path
			if (directory.size())
			{
				if (directory[directory.size()-1] != '/')
				{
					directory = directory + "/";
				}
			}

			// We want the full directory
			if (pathLimit != string::npos)
			{
				directory = directory + _url.substr(0, pathLimit);
			}


			bool result = false;
			string fileName = _url; // The file we are looking for

			// We want to extract the filename from the directory (if there is any)
			if (pathLimit != string::npos) 
			{
				fileName = _url.substr(pathLimit);
			}

			// We open the directory and go through it, looking for our file
			dp = opendir (directory.c_str());
			if (dp != NULL)
         {
           while ((ep = readdir (dp)) && (result == false))
			  {
             if (ep->d_name == fileName)
				{
					result = true;
				}
				}
           (void) closedir (dp);
         }

		 	return result;

		}
		

		void SourceFile::setOverWrite(bool _mode)
		{
			m_overWrite = _mode;
		}
	}
}

