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

#ifndef __SCALAR_H__
#define __SCALAR_H__

#include <string>

#include <boost/lexical_cast.hpp>

#include "abstractattribute.h" 

using namespace std;
using namespace boost;

namespace Gnoll
{
	namespace DynamicObject
	{
		/**
		 *	This is a simple attribute. 
		 */ 
		template <class T > 
		class Scalar : public AbstractAttribute
		{
			public:
				/**
				 * This is a constructor
				 * @param _attributeType Type name of the value to hold
				 * @param _value Value to hold
				 */
				Scalar(string attributeType = "FixMe: No attributeType", T value = T()) : 
					melement(value), 
					m_attributeType(attributeType) 
				{}

				/**
				 * This is a constructor
				 * @param _attributeType Type name of the value to hold
				 */
				Scalar(string attributeType) : m_attributeType(attributeType) {}

				/**
				 * This operator gives a convenient way to get the value hold by an instance of this class
				 */
				T operator() () { return melement; }

				/**
				 * This operator gives a convenient way to get the value hold by an instance of this class
				 */
				T getValue() const { return melement; }

				/**
				 * This operator gives a convenient way to set the value hold by an instance of this class
				 */
				void setValue(T elem) { melement = elem; }

				/**
				 * This operator gives a convenient way to get the attribute type 
				 */
				string getAttrType() const { return m_attributeType; }

				/**
				 * This operator gives a convenient way to set the attribute type
				 */
				void setAttrType(string type) { m_attributeType = type; }

				/**
				 * This method serialize the object. <br/>
				 * It has to be implemented by all classes that inherits from this class.
				 *
				 * @return This return the object as a XML tree 
				 */
				virtual shared_ptr<xmlpp::Document> serializeXML() 
				{
					shared_ptr<xmlpp::Document> document(new xmlpp::Document("1.0"));  

					xmlpp::Element* root = document->create_root_node(m_attributeType );

					string finalString = lexical_cast<string>(melement);

					root->set_attribute("value", finalString);

					return document;
				};		

				/**
				 * This method deserialize the object. <br/>
				 * This method initializes this object thanks to a XML tree given as a parameter. <br/>
				 * It has to be implemented by all classes that inherits from this class.
				 *
				 * @param element This is the XML tree containing the state of this object
				 */
				virtual void deSerializeXML(xmlpp::Element* element) 
				{
					if (element == NULL)
						return;

					xmlpp::Attribute* attr = element->get_attribute("value");

					if (attr == NULL)
						return;

					string attrValue = attr->get_value();
					melement = lexical_cast<T>(attrValue);
				};	

				/**
				 * Overloading the inserting operator
				 * @param _stream Stream to insert the value to
				 * @return Stream
				 */
				virtual ostream& operator<<(ostream& stream)
				{
					return  stream << this->getValue();
				}

				/**
				 * Overloading the extraction operator
				 * @param _stream Streaim to extract the value from
				 * @return Stream
				 */
				virtual istream& operator>>(istream& stream)
				{
					T value;
					if(stream >> value)    
						this->setValue(value);

					return stream;
				}

				/**
				 * Overloading the conversion T -> Scalar<T> operator
				 * @param rValue Value to convert from
				 * @return Converted value 
				 */
				virtual Scalar<T> const& operator=(const T rValue)
				{
					this->setValue(rValue);
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

			private:
				/**
				 * The actual value hold by one instance of this class
				 */
				T melement;

				/**
				 * The name of the node it will use to store melement (usually its type)
				 */
				string m_attributeType;
		};

		/**
		 * Overloading operator +
		 * @param lValue Left value 
		 * @param rValue Right value 
		 * @return Result of the operation 
		 */
		template <typename T> 
		Scalar<T> const operator+(Scalar<T> const& lValue, Scalar<T> const& rValue)
		{
			return Scalar<T>(lValue.getAttrType(), lValue.getValue() + rValue.getValue());
		}

		/**
		 * Overloading operator -
		 * @param lValue Left value 
		 * @param rValue Right value 
		 * @return Result of the operation 
		 */
		template <typename T> 
		Scalar<T> const operator-(Scalar<T> const& lValue, Scalar<T> const &rValue)
		{
			return Scalar<T>(lValue.getAttrType(), lValue.getValue() - rValue.getValue());
		}

		/**
		 * Overloading operator *
		 * @param lValue Left value 
		 * @param rValue Right value 
		 * @return Result of the operation 
		 */
		template <typename T> 
		Scalar<T> const operator*(Scalar<T> const& lValue, Scalar<T> const& rValue)
		{
			return Scalar<T>(lValue.getAttrType(), lValue.getValue() * rValue.getValue());
		}

		/**
		 * Overloading operator /
		 * @param lValue Left value 
		 * @param rValue Right value 
		 * @return Result of the operation 
		 */
		template <typename T> 
		Scalar<T> const operator/(Scalar<T> const& lValue, Scalar<T> const& rValue)
		{
			return Scalar<T>(lValue.getAttrType(), lValue.getValue() / rValue.getValue());
		}

		/**
		 * Overloading operator ^
		 * @param lValue Left value 
		 * @param rValue Right value 
		 * @return Result of the operation 
		 */
		template <typename T> 
		Scalar<T> const operator^(Scalar<T> const& lValue, Scalar<T> const& rValue)
		{
			return Scalar<T>(lValue.getAttrType(), lValue.getValue() ^ rValue.getValue());
		}

		/**
		 * Overloading operator ==
		 * @param lValue Left value
		 * @param rValue Right value
		 * @return Result of the operation
		 */
		template <typename T> 
		bool const operator==(Scalar<T> const& lValue, Scalar<T> const& rValue)
		{
			return (lValue.getValue() == rValue.getValue());
		}

		/**
		 * Overloading operator !=
		 * @param lValue Left value
		 * @param rValue Right value
		 * @return Result of the operation
		 */
		template <typename T> 
		bool const operator!=(Scalar<T> const& lValue, Scalar<T> const& rValue)
		{
			return (lValue.getValue() != rValue.getValue());
		}

		/**
		 * Overloading operator <
		 * @param lValue Left value
		 * @param rValue Right value
		 * @return Result of the operation
		 */
		template <typename T> 
		bool const operator<(Scalar<T> const& lValue, Scalar<T> const& rValue)
		{
			return (lValue.getValue() < rValue.getValue());
		}

		/**
		 * Overloading operator >
		 * @param lValue Left value
		 * @param rValue Right value
		 * @return Result of the operation
		 */
		template <typename T> 
		bool const operator>(Scalar<T> const& lValue, Scalar<T> const& rValue)
		{
			return (lValue.getValue() > rValue.getValue());
		}
	}
}

#endif
