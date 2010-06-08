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

#ifndef __FILESTREAM_H__
#define __FILESTREAM_H__

#include <string>
#include <fstream>

#include "abstractstream.h"

using namespace std;

namespace Gnoll
{
	namespace Core
	{
		/**
		 * This is a stream based on files
		 */
		class FileStream : public AbstractStream
		{
			public:
				/**
				 * This is a constructor.
				 * @param _filename File name
				 * @param _overWrite Will this stream overwrite a file if it already exists
				 */
				FileStream(string _filename, bool _overWrite = false);

				/**
				 * This is a destructor.
				 */
				virtual ~FileStream();

				/**
				 * Read some bytes from the stream
				 * @param _buff Address of the buffer where the data will be stored
				 * @param _size Size of the buffer _buff
				 * @return Returns the number of bytes read
				 */
				virtual size_t read(char* _buff, size_t _size);

				/**
				 * Read some bytes to the stream
				 * @param _buff Address of the buffer where the data to write are be stored
				 * @param _size Size of the buffer _buff
				 * @return Returns the number of bytes written
				 */
				virtual size_t write(const char* _buff, size_t _size);

				/**
				 * Closes the stream
				 */
				virtual void close();

				/**
				 * Tell if the end of the stream has been reached
				 * @return Returns true if the end of the stream has been reached. False otherwise.
				 */
				virtual bool eof();

			private:
				/**
				 * Filename
				 */
				string m_filename;

				/**
				 * iostream to the file
				 */
				fstream m_stream;

				/**
				 * Boolean to tell if the stream is open or not
				 */
				bool m_isOpen;
		};

	}
}

#endif // __FILESTREAM_H__
