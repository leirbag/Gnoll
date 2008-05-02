/***************************************************************************
 *   Copyright (C) 2007 by Paf                                             *
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


/*---------------------------------String----------------------------------*\
|   This is a string attribute for DynamicObject                            |
|                                                                           |
|   Changelog :                                                             |
|               10/03/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../include/string.h"



namespace Gnoll {

	namespace DynamicObject {

		/**
		 * Constructor
		 * @param _value Initialiazion of the String value
		 */
		String::String(string _value) : Scalar<string>("string", _value)
		{
		}


		/**
		 * Destructor
		 */
		String::~String()
		{
		}


		/**
		 * This method deserialize the object. <br/>
		 * This method initializes this object thanks to a XML tree given as a parameter. <br/>
		 * It has to be implemented by all classes that inherits from this class.
		 *
	  	 * @param _element This is the XML tree containing the state of this object
		 */
		void String::deSerializeXML( xmlpp::Element* _element ) 
		{
				
			if (_element == NULL)
			{
				return;
			}

			xmlpp::Attribute* attr = _element->get_attribute("value");

			if (attr == NULL)
			{
				return;
			}

			this->setValue(attr->get_value());
	
		};		


		/**
		 * Overloading of the conversion string -> String operator
		 * @param _rValue Value to convert from
		 * @return Converted value 
		 */
	 	Scalar<string> const & String::operator=(string _rValue)
		{
			this->setValue(_rValue);
			return *this;
		}


		/**
		 * Overloading of the conversion String -> string operator
		 * @return Converted value 
		 */
  		String::operator string()
		{
			return this->getValue();
		}


		/**
 		 * Overloading of the inserting operator
		 * @param _stream Stream to insert the value to
		 * @return Stream
		 */
		ostream &operator<<(ostream & _stream, String const & _str)
		{
			return  _stream << _str.getValue();
		}


		/**
		 * Overloading of the extraction operator
		 * @param _stream Streaim to extract the value from
		 * @return Stream
		 */
		istream &operator>>(istream &_stream, String &_str)
		{

			string value;
			if (_stream >> value)    
			{
				_str.setValue(value);
			}

			return _stream;
		}


	}
}




