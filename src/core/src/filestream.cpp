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

#include "../include/filestream.h"

#include <boost/filesystem/operations.hpp>

#include "../../log/include/logmacros.h"

using namespace boost::filesystem;

namespace Gnoll
{
	namespace Core
	{
		FileStream::FileStream(string filename, bool overWrite) : 
			m_filename(filename)
		{
			ios::openmode mode = ios::in | ios::binary;
			m_isOpen = false;

			if(is_directory(filename))
			{
				GNOLL_LOG().logMessage(filename + " is a directory");
				return;
			}

			if(overWrite)
				mode = mode | ios::trunc | ios::out;

			m_stream.open (m_filename.c_str(), mode);
			if(m_stream.good())
			{
				m_isOpen = true;
				GNOLL_LOG().logMessage(m_filename + " succesfully opened");
			} 
			else 
				GNOLL_LOG().logMessage(m_filename + " UNsuccesfully opened");
		}

		FileStream::~FileStream()
		{
			if(m_isOpen)
			{
				this->close();
				GNOLL_LOG().logMessage("Closing file " + m_filename);
			}
		}

		size_t FileStream::read(char* buff, size_t size)
		{
			GNOLL_LOG().logMessage( "Reading " + m_filename );
			if(m_isOpen)
			{
				m_stream.read(buff, size);
				return m_stream.gcount();
			} 
			else
			{
				GNOLL_LOG().logMessage( m_filename + "was not opened" );
				return 0;
			}
		}

		size_t FileStream::write(const char* buff, size_t size)
		{
			if (m_isOpen)
			{
				unsigned int before = m_stream.tellp();
				m_stream.write(buff, size);
				return (unsigned int)m_stream.tellp() - before;
			} 
			else
				return 0;
		}

		void FileStream::close()
		{
			if(m_isOpen)
			{
				m_stream.flush();
				m_stream.close();
			}
		}

		bool FileStream::eof()
		{
			if(m_isOpen)
				return m_stream.eof();
			else
			{
				GNOLL_LOG().logMessage("Trying to get an EOF on a closed file: " + m_filename);
				return 1;
			}
		}
	}
}
