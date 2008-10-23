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


/*-----------------------------filestream----------------------------------*\
|   This is a stream based on files                                         |
|                                                                           |
|   Changelog :                                                             |
|               07/18/2007 - Paf - Initial release                          |
|               08/18/2007 - Paf - Update comments                          |
|               09/25/2007 - Paf - Replace namespace Viracocha by Gnoll     |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../include/filestream.h"
#include "../../log/include/clogmodule.h"



namespace Gnoll {

	namespace Core {


		FileStream::FileStream( string _filename, bool _overWrite) : m_filename(_filename)
		{
			ios::openmode mode = ios::in | ios::binary;
			m_isOpen = false;

			if (_overWrite)
			{
				mode = mode | ios::trunc | ios::out;
			}

			m_stream.open (m_filename.c_str(), mode); 

			if (m_stream.good())
			{
				m_isOpen = true;
				Gnoll::Log::CLogModule::getInstancePtr()->logMessage( m_filename + " succesfully opened" );
			} else {
				Gnoll::Log::CLogModule::getInstancePtr()->logMessage( m_filename + " UNsuccesfully opened" );
			}
		}


		FileStream::~FileStream()
		{
			if (m_isOpen)
			{
				this->close();
				Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "Closing file " + m_filename );
			}
		}


		size_t FileStream::read( char* _buff, size_t _size)
		{
			Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "Reading " + m_filename );
			if (m_isOpen)
			{
				m_stream.read(_buff, _size);
				return m_stream.gcount();
			} else 
			{
				Gnoll::Log::CLogModule::getInstancePtr()->logMessage( m_filename + "was not opened" );
				return 0;
			}
		}

		size_t FileStream::write( const char* _buff, size_t _size)
		{
			if (m_isOpen)
			{
				unsigned int before = m_stream.tellp();
				m_stream.write(_buff, _size);
				return (unsigned int)m_stream.tellp() - before;
			} else 
			{
				return 0;
			}
		}
	
		void FileStream::close()
		{
			if (m_isOpen)
			{
				m_stream.flush();
				m_stream.close();
			}
		}

		bool FileStream::eof()
		{
			if (m_isOpen)
			{
				return m_stream.eof();
			} else 
			{ 
				Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "Trying to get an EOF on a closed file: " + m_filename );
				return 1;		  
			}
		}
	}
}




