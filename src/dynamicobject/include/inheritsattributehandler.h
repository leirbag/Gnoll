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

#ifndef __INHERITSATTRIBUTEHANDLER_H__
#define __INHERITSATTRIBUTEHANDLER_H__

#include <boost/shared_ptr.hpp>
#include <libxml++/libxml++.h>

#include "dynamicobject.h"
#include "abstractattributehandler.h"

using namespace boost;
using namespace Gnoll::Core;

namespace Gnoll
{
	namespace DynamicObject
	{
		/**
		*	This is the interface of an attribute handler.</br>
		*	This make sure each Attribute will be (de)serializable.</br>
		*	This works as long as T inherits from AbstractAttribute.
		*/
		class InheritsAttributeHandler : public AbstractAttributeHandler
		{
			public:
				typedef shared_ptr<AbstractAttribute> AbstractAttribute_ptr;

				/**
				* This is a constructor.
				*/
				InheritsAttributeHandler();

				/**
				* This is a destructor.
				*/
				~InheritsAttributeHandler();

				/**
				 * This is the handler method
				 * @param _node Node to process and extract the AbstractAttribute smart pointer from
				 * @return Smart pointer to an AbstractAttribute
				 */
				virtual AbstractAttribute_ptr handle(xmlpp::Element* node, 
													 DynamicObject* po = NULL);
		};
	}
}

#endif
