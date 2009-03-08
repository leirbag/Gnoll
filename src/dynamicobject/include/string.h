/***************************************************************************
 *   Copyright (C) 2009 by Bruno Mahe                                      *
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


/*---------------------------------string----------------------------------*\
|   This is a string attribute for DynamicObject                            |
|                                                                           |
\*-------------------------------------------------------------------------*/



#ifndef __STRING_H__
#define __STRING_H__


#include "iattribute.h"
#include "scalar.h"
#include <string>

using namespace std;
using namespace boost;

namespace Gnoll
{
	namespace DynamicObject
	{

		/**
		 *	This is a simple attribute.
		 */
		class String : public Scalar<string>
		{
			public:

				/**
				 * Returns String's DynamicObject name
				 * @return String's DynamicObject name
				 */
				inline static const char * DYNAMIC_OBJECT_NAME() {return "string";}


				/**
				 * Returns attribute name "value".<br/>
				 * This attribute contains the value of a string
				 * @return The attribute name "value"
				 */
				inline static const char * ATTRIBUTE_VALUE() {return "value";}


				/**
				 * Constructor
				 * @param _value Initialiazion of the String value
				 */
				String(string _value = "");


				/**
				 * Destructor
				 */
				virtual ~String();


				/**
				 * This method deserialize the object. <br/>
				 * This method initializes this object thanks to a XML tree given as a parameter. <br/>
				 * It has to be implemented by all classes that inherits from this class.
				 *
			  	 * @param _element This is the XML tree containing the state of this object
				 */
				virtual void deSerializeXML( xmlpp::Element* _element );


				/**
				 * Overloading of the conversion string -> String operator
				 * @param _rValue Value to convert from
				 * @return Converted value
				 */
			 	virtual Scalar<string> const &operator=(string _rValue);


				/**
				 * Overloading of the conversion String -> string operator
				 * @return Converted value
				 */
		  		virtual operator string();


				/**
				 * Overloading of the conversion String -> Glib::ustring operator
				 * @return Converted value
				 */
				virtual operator Glib::ustring();


				/**
				 * Overloading of the conversion String -> const string operator
				 * @return Converted value
				 */
				virtual operator string() const;


				/**
				 * Overloading of the conversion String -> const Glib::ustring operator
				 * @return Converted value
				 */
				virtual operator Glib::ustring() const;




		};


		/**
 		 * Overloading of the inserting operator
		 * @param _stream Stream to insert the value to
		 * @return Stream
		 */
		ostream &operator<<(ostream & _stream, String const & _str);


		/**
		 * Overloading of the extraction operator
		 * @param _stream Streaim to extract the value from
		 * @return Stream
		 */
		istream &operator>>(istream &_stream, String &_str);

	}
}

#endif // __STRING_H__
