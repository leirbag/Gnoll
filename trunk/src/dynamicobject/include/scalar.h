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
|               09/25/2007 - Paf - Implements Scalar::deSerializeXML()      |
|                                - Add method Scalar::getValue()            |
|                                - Enclose Scalar in namespace Gnoll::Core  |
|                                - Add more comments                        |
|               09/26/2007 - Paf - Replace sstream by boost::lexical_cast   |
|                                    for lexical conversions                |
|               10/02/2007 - Paf - Add conversion operators                 |
|                                - Add arithmetic operators                 |
|                                - Add insertion and extraction operators   |
|                                - getValue() method is now const           |
|                                - Add attribute type getter and setter     |
|               12/17/2007 - Paf - Add default values to constructor        |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "iattribute.h" 
#include <string>
#include <boost/lexical_cast.hpp>


#ifndef __SCALAR_H__
#define __SCALAR_H__

using namespace std;
using namespace boost;


namespace Gnoll
{

	namespace DynamicObject
	{

		/**
		 *	This is a simple attribute. 
		 */ 
		template <class T > class Scalar : public IAttribute
		{
			private:

				/**
				 * The actual value hold by one instance of this class
				 */
				T m_element;
	
				/**
				 * The name of the node it will use to store m_element (usually its type)
				 */
				string m_attributeType;

			public:

				/**
				 * This is a constructor
				 * @param _attributeType Type name of the value to hold
				 * @param _value Value to hold
				 */
				Scalar(string _attributeType = "FixMe: No attributeType", T _value = T()) : m_element(_value), m_attributeType(_attributeType) {};


				/**
				 * This is a constructor
				 * @param _attributeType Type name of the value to hold
				 */
				Scalar(string _attributeType ) : m_attributeType(_attributeType) {};



				/**
				 * This operator gives a convenient way to get the value hold by an instance of this class
				 */
				T operator() (void) { return m_element;};
	

				/**
				 * This operator gives a convenient way to get the value hold by an instance of this class
				 */
				T getValue() const {return m_element;};


				/**
				 * This operator gives a convenient way to set the value hold by an instance of this class
				 */
				void setValue( T _elem) { m_element = _elem;};


				/**
				 * This operator gives a convenient way to get the attribute type 
				 */
				string getAttrType() const {return m_attributeType;};


				/**
				 * This operator gives a convenient way to set the attribute type
				 */
				void setAttrType( string _type) { m_attributeType = _type;};


				/**
				 * This method serialize the object. <br/>
				 * It has to be implemented by all classes that inherits from this class.
				 *
				 * @return This return the object as a XML tree 
				 */
				virtual shared_ptr<xmlpp::Document> serializeXML() 
				{
					shared_ptr<xmlpp::Document> document( new xmlpp::Document("1.0"));  

					xmlpp::Element* root = document->create_root_node( m_attributeType );
	
					string finalString = lexical_cast<string> (m_element);

					root->set_attribute("value", finalString);
		
					return document;
				};		


				/**
				 * This method deserialize the object. <br/>
				 * This method initializes this object thanks to a XML tree given as a parameter. <br/>
				 * It has to be implemented by all classes that inherits from this class.
				 *
			  	 * @param _element This is the XML tree containing the state of this object
				 */
				virtual void deSerializeXML( xmlpp::Element* _element ) 
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

					string attrValue = attr->get_value();

					m_element = lexical_cast<T>(attrValue);
			
				};	

				/**
				 * Overloading the inserting operator
				 * @param _stream Stream to insert the value to
				 * @return Stream
				 */
				virtual ostream &operator<<(ostream &_stream)
				{
				 	return  _stream << this->getValue();
				}


				/**
				 * Overloading the extraction operator
				 * @param _stream Streaim to extract the value from
				 * @return Stream
				 */
				virtual istream &operator>>(istream &_stream)
				{

					T value;
					if (_stream >> value)    
					{
						this->setValue(value);
					}

					return _stream;
				}


				/**
				 * Overloading the conversion T -> Scalar<T> operator
				 * @param _rValue Value to convert from
				 * @return Converted value 
				 */
			 	virtual Scalar<T> const &operator=(const T _rValue)
				{
					this->setValue(_rValue);
					return *this;
				}


				/**
				 * Overloading the conversion Scalar<T> -> T operator
				 * @return Converted value 
				 */
		  		virtual operator T() const
				{
					return getValue();
				}


		};


		/**
		 * Overloading operator +
		 * @param _lValue Left value 
		 * @param _rValue Right value 
		 * @return Result of the operation 
		 */
		template <typename T> Scalar<T> const operator+(Scalar<T> const &_lValue, Scalar<T> const &_rValue)
		{
			return Scalar<T>( _lValue.getAttrType, _lValue.getValue() + _rValue.getValue() );
		}


		/**
		 * Overloading operator -
		 * @param _lValue Left value 
		 * @param _rValue Right value 
		 * @return Result of the operation 
		 */
		template <typename T> Scalar<T> const operator-(Scalar<T> const &_lValue, Scalar<T> const &_rValue)
		{
			return Scalar<T>( _lValue.getAttrType(),  _lValue.getValue() - _rValue.getValue() );
		}


		/**
		 * Overloading operator *
		 * @param _lValue Left value 
		 * @param _rValue Right value 
		 * @return Result of the operation 
		 */
		template <typename T> Scalar<T> const operator*(Scalar<T> const &_lValue, Scalar<T> const &_rValue)
		{
			return Scalar<T>( _lValue.getAttrType(), _lValue.getValue() * _rValue.getValue() );
		}


		/**
		 * Overloading operator /
		 * @param _lValue Left value 
		 * @param _rValue Right value 
		 * @return Result of the operation 
		 */
		template <typename T> Scalar<T> const operator/(Scalar<T> const &_lValue, Scalar<T> const &_rValue)
		{
			return Scalar<T>( _lValue.getAttrType(), _lValue.getValue() / _rValue.getValue() );
		}


		/**
		 * Overloading operator ^
		 * @param _lValue Left value 
		 * @param _rValue Right value 
		 * @return Result of the operation 
		 */
		template <typename T> Scalar<T> const operator^(Scalar<T> const &_lValue, Scalar<T> const &_rValue)
		{
			return Scalar<T>( _lValue.getAttrType(), _lValue.getValue() ^ _rValue.getValue() );
		}


		/**
		 * Overloading operator ==
		 * @param _lValue Left value
		 * @param _rValue Right value
		 * @return Result of the operation
		 */
		template <typename T> bool const operator==(Scalar<T> const &_lValue, Scalar<T> const &_rValue)
		{
			return (_lValue.getValue() == _rValue.getValue() );
		}


		/**
		 * Overloading operator !=
		 * @param _lValue Left value
		 * @param _rValue Right value
		 * @return Result of the operation
		 */
		template <typename T> bool const operator!=(Scalar<T> const &_lValue, Scalar<T> const &_rValue)
		{
			return (_lValue.getValue() != _rValue.getValue() );
		}


		/**
		 * Overloading operator <
		 * @param _lValue Left value
		 * @param _rValue Right value
		 * @return Result of the operation
		 */
		template <typename T> bool const operator<(Scalar<T> const &_lValue, Scalar<T> const &_rValue)
		{
			return (_lValue.getValue() < _rValue.getValue() );
		}


		/**
		 * Overloading operator >
		 * @param _lValue Left value
		 * @param _rValue Right value
		 * @return Result of the operation
		 */
		template <typename T> bool const operator>(Scalar<T> const &_lValue, Scalar<T> const &_rValue)
		{
			return (_lValue.getValue() > _rValue.getValue() );
		}

	}

}

#endif // __SCALAR_H__
