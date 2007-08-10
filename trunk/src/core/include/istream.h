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


/*--------------------------------istream----------------------------------*\
|   This is the interface of all the streams.                               |
|                                                                           |
|                                                                           |
|   Changelog :                                                             |
|               07/18/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/



#ifndef __ISTREAM_H__
#define __ISTREAM_H__


namespace Viracocha {

	namespace Core {


		class IStream
		{
			public:

				/**
				 * This is a constructor.
				 */
				IStream() {};


				/**
				 * This is a destructor.
				 */
				virtual ~IStream() {};


				virtual size_t read( char* _buff, size_t _size) = 0;

				virtual size_t write(const char* _buff, size_t _size) = 0;
	
				virtual void close() = 0;

				virtual bool eof() = 0;

		};

	}
}

#endif // __ISTREAM_H__
