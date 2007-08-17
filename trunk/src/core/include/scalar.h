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


/*---------------------------------scalar----------------------------------*\
|   This is a basic container supposed to hold any basic type (char, int...)|
|                                                                           |
|   Changelog :                                                             |
|               07/09/2007 - Paf - Initial release                          |
|               07/10/2007 - Paf - Implements Scalar::serializeXML()        |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "iattribute.h" 
#include <sstream>
#include <string>

#ifndef __SCALAR_H__
#define __SCALAR_H__

using namespace std;
using namespace boost;


/**
 *	This is a simple attribute. 
 */ 
template <class T > class Scalar : public IAttribute
{
	private:
		T m_element;

	public:

		Scalar(T _value) : m_element(_value) {};


		T operator() (void) { return m_element;};

		virtual shared_ptr<xmlpp::Document> serializeXML() 
		{
			shared_ptr<xmlpp::Document> document( new xmlpp::Document("1.0"));  

			xmlpp::Element* root = document->create_root_node("scalar");

			// std::stringstream is not really friendly with non-ascii
			// characters.
			std::stringstream ss;
			string finalString;

			ss << m_element;
			finalString = ss.str();

			root->set_attribute("value", finalString);

			return document;
		};		

		// XXX TODO : Implement this method
		virtual void deSerializeXML( xmlpp::Element* _element ) {};		
};

#endif // __SCALAR_H__
