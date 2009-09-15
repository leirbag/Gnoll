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


/*-------------------------------cmessage----------------------------------*\
|   This is a message                                                       |
|                                                                           |
|   Changelog :                                                             |
|               05/15/2006 - Paf - Initial release                          |
|               05/09/2007 - Paf - Change m_userdata type to                |
|                                     shared_ptr< boost::any >              |
|               04/10/2006 - Gabriel - Add namespace Gnoll and Core         |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>

#include "../messages/include/messagetype.h"

#ifndef __CMESSAGE_H__
#define __CMESSAGE_H__

using namespace std;
using namespace boost;
using boost::any_cast;

namespace Gnoll
{
	namespace Core
	{
		const Messages::MessageType MSG_ANYTYPE("*");

		/**
		 *	A message.
		 */
		class CMessage
		{
			private:

				Messages::MessageType m_type;
				shared_ptr<boost::any> m_userdata;

			public:

				/**
				 * This is a constructor
				 */
				CMessage(Messages::MessageType _msgtype = MSG_ANYTYPE, shared_ptr<boost::any> _data = shared_ptr<boost::any>() ): m_type(_msgtype), m_userdata(_data) {}

				/**
				 * This is a destructor
				 */
				~CMessage() {}

				/**
				 * This returns the type of this message
				 *	@return Messages::MessageType	:	Its type
				 */
				Messages::MessageType getType() {return m_type;}

				/**
				 * This returns the data contained in this message
				 *	@return shared_ptr<boost::any>	:	Its data
				 */
				template <class T> T getData() {return  T (any_cast<T>(*m_userdata)) ;}

		};
	};
};
#endif // __CMESSAGE_H__
