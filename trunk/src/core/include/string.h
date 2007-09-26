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


/*---------------------------------string----------------------------------*\
|   This is a string attribute for PersistentObject                         |
|                                                                           |
|   Changelog :                                                             |
|               09/26/2007 - Paf - Initial release                          |
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
	namespace Core
	{

		/**
		 *	This is a simple attribute. 
		 */ 
		class String : public Scalar<string>
		{
			public:
		
				String(string _value = "") : Scalar<string>("string", _value) {};


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

					this->setValue(attr->get_value());
			
				};		



		};

	}
}

#endif // __FLOAT_H__
