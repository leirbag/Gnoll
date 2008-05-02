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


/*----------------------------------List-----------------------------------*\
|   This is a List attribute for DynamicObject                              |
|                                                                           |
|   Changelog :                                                             |
|               09/26/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/



#ifndef __LIST_H__
#define __LIST_H__


#include <boost/shared_ptr.hpp>
#include <libxml++/libxml++.h>
#include <list>

#include "iattribute.h"
#include "attributehandlerregistry.h"


using namespace std;
using namespace boost;

namespace Gnoll
{
	namespace DynamicObject
	{

		/**
		 *	This is a list attribute for DynamicObject.
		 */
		class List : public list< shared_ptr<IAttribute> >, public IAttribute
		{
			private:


			public:

				/**
				 * This is a constructor
				 */
				List();


				/**
				 * This is a destructor
				 */
				virtual ~List();


				/**
				 * This method serialize the object. <br/>
				 * It has to be implemented by all classes that inherits from this class.
				 *
				 * @return This return the object as a XML tree
				 */
				virtual shared_ptr<xmlpp::Document> serializeXML();


				/**
				 * This method deserialize the object. <br/>
				 * This method initializes this object thanks to a XML tree given as a parameter. <br/>
				 * It has to be implemented by all classes that inherits from this class.
				 *
			  	 * @param _element This is the XML tree containing the state of this object
				 */
				virtual void deSerializeXML( xmlpp::Element* _element );
		};

	}
}

#endif // __LIST_H__
