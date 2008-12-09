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


/*---------------------------cmessagetype.cpp------------------------------*\
|   This is a type of a message                                             |
|                                                                           |
|   Changelog :                                                             |
|               05/15/2006 - Paf - Initial release                          |
|               04/10/2006 - Gabriel - Add namespace Gnoll and Core         |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../include/cmessagetype.h"

namespace Gnoll
{
	namespace Core
	{
		bool CMessageType::operator== (CMessageType const & op)
		{
			return (this->m_msgtype == op.m_msgtype);

		}


		bool CMessageType::operator!= (CMessageType const & op)
		{
			return !(*this == op);
		}


		bool CMessageType::operator< (CMessageType const & op) const
		{
			return ( this->m_msgtype < op.m_msgtype );
		}
	};
};
