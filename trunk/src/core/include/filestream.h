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


/*-------------------------------filestream--------------------------------*\
|   This is can stream files.                                               |
|                                                                           |
|                                                                           |
|   Changelog :                                                             |
|               07/18/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include <string>
#include <fstream>

#include "istream.h"

#ifndef __FILESTREAM_H__
#define __FILESTREAM_H__


using namespace std;

namespace Viracocha {

	namespace Core {


		class FileStream : public IStream
		{
			private:
				string m_filename;
				fstream m_stream;
				bool m_isOpen;


			public:

				/**
				 * This is a constructor.
				 */
				FileStream( string _filename, bool _overWrite = false);


				/**
				 * This is a destructor.
				 */
				virtual ~FileStream();


				virtual size_t read( char* _buff, size_t _size);

				virtual size_t write( const char* _buff, size_t _size);
	
				virtual void close();

				virtual bool eof();

		};

	}
}

#endif // __FILESTREAM_H__
