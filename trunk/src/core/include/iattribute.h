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


/*-----------------------------iattribute----------------------------------*\
|   This is the interface of all the attributes. Each Attribute has to be   |
|     (de)serializable                                                      |
|                                                                           |
|   Changelog :                                                             |
|               07/09/2007 - Paf - Initial release                          |
|               07/10/2007 - Paf - Add some comments                        |
|                          - Paf - serializeXML() now returns a             |
|                                    shared_ptr<xmlpp::Document>            |
|               08/14/2007 - Paf - Update comments                          |
|               04/10/2006 - Gabriel - Add namespace Gnoll and Core         |
|                                                                           |
\*-------------------------------------------------------------------------*/



#ifndef __IATTRIBUTE_H__
#define __IATTRIBUTE_H__


#include <boost/shared_ptr.hpp>
#include <libxml++/libxml++.h>


using namespace boost;

namespace Gnoll
{
	namespace Core
	{
		/**
		 *	This is the interface of an attribute.
		 *	This make sure each Attribute will be (de)serializable
		 */
		class IAttribute
		{
			public:

				/**
				 * This is a constructor.
				 */
				IAttribute() {};


				/**
				 * This is a destructor.
				 */
				virtual ~IAttribute() {};


				/**
				 * This method serialize the object. <br/>
				 * It has to be implemented by all classes that inherits from this class.
				 *
				 * @return This return the object as a XML tree
				 */
				virtual shared_ptr<xmlpp::Document> serializeXML() = 0;


				/**
				 * This method deserialize the object. <br/>
				 * This method initializes this object thanks to a XML tree given as a parameter. <br/>
				 * It has to be implemented by all classes that inherits from this class.
				 *
				 * @param _element This is the XML tree containing the state of this object
				 */
				virtual void deSerializeXML( xmlpp::Element* _element ) = 0;

		};
	};
};

#endif // __IATTRIBUTE_H__
