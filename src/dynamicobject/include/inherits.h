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


/*-------------------------------Inherits----------------------------------*\
|   This is an inheritance attribute for DynamicObject                      |
|                                                                           |
\*-------------------------------------------------------------------------*/



#ifndef __INHERITS_H__
#define __INHERITS_H__


#include "iattribute.h"
#include "string.h"
#include "attributehandlerregistry.h"


using namespace std;
using namespace boost;

namespace Gnoll
{
	namespace DynamicObject
	{

		/**
		 *	This is an inheritance attribute for DynamicObject.
		 */
		class Inherits : public IAttribute
		{
			private:

				String m_parent;

			public:


				/**
				 * Returns Inherits's DynamicObject name
				 * @return Inherits's DynamicObject name
				 */
				inline static const char * DYNAMIC_OBJECT_NAME() {return "inherits";}


				/**
				 * This is a constructor
				 */
				Inherits(String _parent = String());


				/**
				 * This is a destructor
				 */
				~Inherits();


				/**
				 * Get the parent DynamicObject instance name
				 * @return Parent DynamicObject instance name
				 */
				String getParent();


				/**
				 * Set the parent DynamicObject instance name
				 * @param _parent Parent DynamicObject instance name
				 */
				void setParent (String _parent);


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


				virtual void applyInheritance() {}


		};

	}
}

#endif // __INHERITS_H__
