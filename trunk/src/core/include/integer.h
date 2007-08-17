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


/*---------------------------------integer---------------------------------*\
|   This is a basic container supposed to an integer basic type             |
|                                                                           |
|   Changelog :                                                             |
|               08/04/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "iattribute.h" 
#include <sstream>

#ifndef __INTEGER_H__
#define __INTEGER_H__

using namespace std;
using namespace boost;


namespace Viracocha
{

	namespace Core 
	{

		/**
		*	This is a simple attribute. </br> 
		*	This hold a signed integer.
		*/ 
		class Integer : public IAttribute
		{
			private:

				/**
				 * The actual value hold by one instance of this class
				 */
				int m_element;

			public:

				/**
				 * This is a constructor
				 * @param _value Value to hold
				 */
				Integer(int _value = 0) : m_element(_value) {};


				/**
				 * This operator gives a convenient way to get the value hold by an instance of this class
				 */
				int operator() (void) { return m_element;};

				/**
			  	 * This method serialize the object. <br/>
				 * It has to be implemented by all classes that inherits from this class.
				 *
				 * @return This return the object as a XML tree 
				 */

				virtual shared_ptr<xmlpp::Document> serializeXML() 
				{
					shared_ptr<xmlpp::Document> document( new xmlpp::Document("1.0"));  

					xmlpp::Element* root = document->create_root_node("integer");

					// std::stringstream is not really friendly with non-ascii
					// characters.
					std::stringstream ss;
					string finalString;

					ss << m_element;
					finalString = ss.str();

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

					std::istringstream ss(attr->get_value());
					ss >> m_element;
				};		
		};
	};
};

#endif // __INTEGER_H__
