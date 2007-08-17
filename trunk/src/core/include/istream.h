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
|               08/16/2007 - Paf - Update comments                          |
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

				/**
				 * This methods reads at most _size bytes and store them in the buffer _buff
				 * @param _buff Buffer where data will be stored
				 * @param _size Maximum number of bytes to read
				 * @return Number of bytes actually read
				 */
				virtual size_t read( char* _buff, size_t _size) = 0;

				/**
				 * This methods writes at most _size bytes from the buffer _buff
				 * @param _buff Buffer from where data will be read
				 * @param _size Maximum number of bytes to written
				 * @return Number of bytes actually written
				 */
				virtual size_t write(const char* _buff, size_t _size) = 0;
	
				/**
				 * This method closes the stream
				 */
				virtual void close() = 0;

				/**
				 * This method tells you if it's the end of the stream
				 * @return True if it's the end of the stream. False otherwise
				 */
				virtual bool eof() = 0;

		};

	}
}

#endif // __ISTREAM_H__
