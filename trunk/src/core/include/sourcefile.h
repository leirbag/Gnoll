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


/*-----------------------------sourcefile----------------------------------*\
|   This is a source that interface files                                   |
|                                                                           |
|   Changelog :                                                             |
|               07/11/2007 - Paf - Initial release                          |
|               08/17/2007 - Paf - Update comments                          |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include <boost/shared_ptr.hpp>
#include <libxml++/libxml++.h>

#include "isource.h"
#include "filestream.h"

#ifndef __SOURCEFILE_H__
#define __SOURCEFILE_H__

using namespace std;
using namespace boost;


namespace Viracocha
{
	namespace Core
	{

		class SourceFile : public ISource
		{
			private:

				const string m_path;
				bool m_overWrite;

			public:

				/**
				* This is a constructor.
				*/
				SourceFile( const string _path, bool _overWrite = false, unsigned int _priority = 0);


				/**
				* This is a destructor.
				*/
				virtual ~SourceFile();


				/**
				 * This loads a stream from a given URL
				 * @param _url URL to load
				 * @return Stream based on this URL
				 */ 
				virtual shared_ptr<IStream> load( const string _url);


				/**
				 *	This methods tells if a stream can be built from a given URL
				 *	@param _url URL to be tested
				 *	@param True if a stream can be built from this URL
				 */
				virtual bool isFetchable( const string _url);

				/**
				 * This methods set the overwriting mode
				 * @param _mode True means files will be overwritten. False means no file will be overwritten.
				 */
				void setOverWrite(bool _mode);
		};
	}
}

#endif // __SOURCEFILE_H__
