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

#include "../include/string.h"

namespace Gnoll 
{
	namespace DynamicObject 
	{
		String::String(string value) : 
			Scalar<string>(String::DYNAMIC_OBJECT_NAME(), value)
		{
		}

		String::~String()
		{
		}

		void String::deSerializeXML(xmlpp::Element* element)
		{
			if (element == NULL)
				return;

			xmlpp::Attribute* attr = element->get_attribute(String::ATTRIBUTE_VALUE());

			if (attr == NULL)
				return;

			this->setValue(attr->get_value());

		};

	 	Scalar<string> const & String::operator=(string rValue)
		{
			this->setValue(rValue);
			return *this;
		}

  		String::operator string()
		{
			return this->getValue();
		}

		String::operator Glib::ustring()
		{
			return this->getValue();
		}

		String::operator string() const
		{
			return this->getValue();
		}

		String::operator Glib::ustring() const
		{
			return this->getValue();
		}

		ostream &operator<<(ostream& stream, String const& str)
		{
			return  stream << str.getValue();
		}

		istream &operator>>(istream& stream, String& str)
		{
			string value;
			if (stream >> value)
			{
				str.setValue(value);
			}

			return stream;
		}
	}
}
