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

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>

#include "../messages/include/messagetype.h"

using namespace std;
using namespace boost;
using namespace Gnoll::Core::Messages;
using boost::any_cast;

namespace Gnoll
{
	namespace Core
	{
		typedef shared_ptr<boost::any> Any_ptr;
		const Messages::MessageType MSG_ANYTYPE("*");

		/**
		 *	A message.
		 */
		class Message
		{
			public:
				/**
				 * This is a constructor
				 */
				Message(MessageType msgtype = MSG_ANYTYPE, Any_ptr data = Any_ptr()) :
					m_type(msgtype), m_userdata(data) {}

				/**
				 * This is a destructor
				 */
				~Message() {}

				/**
				 * This returns the type of this message
				 *	@return Messages::MessageType	:	Its type
				 */
				MessageType getType() { return m_type; }

				/**
				 * This returns the data contained in this message
				 *	@return shared_ptr<boost::any>	:	Its data
				 */
				template<class T>
				T getData() { return T (any_cast<T>(*m_userdata)); }

			private:
				MessageType m_type;
				shared_ptr<boost::any> m_userdata;
		};
	};
};
#endif
