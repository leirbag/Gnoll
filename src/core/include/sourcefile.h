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

#ifndef __SOURCEFILE_H__
#define __SOURCEFILE_H__

#include <boost/shared_ptr.hpp>
#include <libxml++/libxml++.h>

#include "abstractsource.h"
#include "filestream.h"

using namespace std;
using namespace boost;

namespace  Gnoll
{
	namespace Core
	{
		typedef unsigned int uint;

		class SourceFile : public AbstractSource
		{
			public:

				/**
				* This is a constructor.
				*/
				SourceFile(const string _path, bool _overWrite = false, uint _priority = 0);

				/**
				* This is a destructor.
				*/
				virtual ~SourceFile();

				/**
				 * This loads a stream from a given URL
				 * @param _url URL to load
				 * @return Stream based on this URL
				 */
				virtual shared_ptr<AbstractStream> loadStream(const string _url);

				/**
				 * This loads a stream with writing permissions to a given URL
				 * @param _url URL to save to
				 * @return True if the operation is successful. False otherwise
				 */
				virtual shared_ptr<AbstractStream> saveStream(const string _url);

				/**
				 *	This methods tells if a stream can be built from a given URL
				 *	@param _url URL to be tested
				 *	@param True if a stream can be built from this URL
				 */
				virtual bool isFetchable(const string _url);

				/**
				 *	This methods tells if a writable stream can be built from a given URL
				 *	@param _url URL to be tested
				 *	@param True if a writable stream can be built from this URL
				 */
				virtual bool isWritable(const string _url);

				/**
				 * This methods set the overwriting mode
				 * @param _mode True means files will be overwritten. False means no file will be overwritten.
				 */
				void setOverWrite(bool _mode);

			private:
				/**
				 * This creates a stream from a given URL
				 * @param _url URL to load
				 * @return Stream based on this URL
				 */
				shared_ptr<AbstractStream> newStream(const string _url);

			private:
				const string m_path;
				bool m_overWrite;
		};
	}
}

#endif // __SOURCEFILE_H__
