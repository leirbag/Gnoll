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
using namespace Viracocha::Core;


class SourceFile : public ISource
{
	private:

		const string m_path;
		bool m_overWrite;

	public:

		/**
		 * This is a constructor.
		 */
		SourceFile( const string _path, bool _overWrite = false);


		/**
		 * This is a destructor.
		 */
		virtual ~SourceFile();


		virtual shared_ptr<IStream> load( const string _url);
		virtual bool isFetchable( const string _url);

		void setOverWrite(bool _mode);
};

#endif // __SOURCEFILE_H__
