/***************************************************************************
 *   Copyright (C) 2008 by Bruno Mahe                                      *
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

#ifndef __SET_H__
#define __SET_H__

#include <set>

#include <boost/shared_ptr.hpp>
#include <libxml++/libxml++.h>

#include "abstractattribute.h"
#include "attributehandlerregistry.h"

using namespace std;
using namespace boost;

namespace Gnoll
{
	namespace DynamicObject
	{
		bool operator<(AbstractAttribute const& a, AbstractAttribute const& b);

		struct AbstractAttributeComparison
		{
			bool operator()(shared_ptr<AbstractAttribute> ia1, shared_ptr<AbstractAttribute> ia2) const
			{
				return (*ia1) < (*ia2);
			}
		};

		/**
		 *	This is a set attribute for DynamicObject.
		 */
		class Set : public set< shared_ptr<AbstractAttribute>, AbstractAttributeComparison >, 
					public AbstractAttribute
		{
			public:
				/**
				 * Returns Set's DynamicObject name
				 * @return Set's DynamicObject name
				 */
				inline static const char* DYNAMIC_OBJECT_NAME() { return "set"; }

				/**
				 * This is a constructor
				 */
				Set();

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
				virtual void deSerializeXML(xmlpp::Element* element);
		};
	}
}

#endif 
