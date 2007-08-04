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


/*-----------------------------cmessagetype--------------------------------*\
|   This is a type of a message                                             |
|                                                                           |
|   Changelog :                                                             |
|               05/15/2006 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include <string>


#ifndef __CMESSAGETYPE_H__
#define __CMESSAGETYPE_H__


using namespace std;



/**
 *	A message's type.
 */ 
class CMessageType
{
	private:

		/**
		 * A string which contain the message type
		 */
		string m_msgtype;

	public:
		
		/**
		 * This is a constructor
		 */
		CMessageType(string _msgtype): m_msgtype(_msgtype) {}

		/**
		 * This is a destructor
		 */
		~CMessageType() {}

		/**
		 * Operator ==
		 * @param op A message's type to compare to
		 * @return bool : The result :)
		 */
		bool operator== (CMessageType const & op); 

		/**
		 * Operator !=
		 * @param op A message's type to compare to
		 * @return bool : The result :)
		 */
		bool operator!= (CMessageType const & op); 

		/**
		 * Operator <
		 * @param op A message's type to compare to
		 * @return bool : The result :)
		 */
		bool operator< ( CMessageType const & op ) const;

		/**
		 * Returns its type
		 * @return string : The type 
		 */
		string getTypeStr() {return m_msgtype;}

};

#endif // __CMESSAGETYPE_H__
