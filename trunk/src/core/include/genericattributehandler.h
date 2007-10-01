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


/*--------------------------genericattributehandler------------------------*\
|   This is a generic attribute handler.                                    |
|                                                                           |
|                                                                           |
|   Changelog :                                                             |
|               08/03/2007 - Paf - Initial release                          |
|               08/14/2007 - Paf - Update comments                          |
|               09/25/2007 - Paf - Replace namespace Gnoll by Gnoll     |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include <boost/shared_ptr.hpp>
#include <libxml++/libxml++.h>

#include "../include/iattributehandler.h"

#ifndef __GENERICATTRIBUTEHANDLER_H__
#define __GENERICATTRIBUTEHANDLER_H__

using namespace boost;


namespace Gnoll
{

	namespace Core
	{

		/**
		*	This is the interface of an attribute handler.</br>
		*	This make sure each Attribute will be (de)serializable.</br>
		*	This works as long as T inherits from IAttribute.
		*/ 
		template <class T> class GenericAttributeHandler : public IAttributeHandler
		{

			public:

				/**
				* This is a constructor.
				*/
				GenericAttributeHandler() {}


				/**
				* This is a destructor.
				*/
				~GenericAttributeHandler() {}


				/**
				 * This is the handler method
				 * @param _node Node to process and extract the IAttribute smart pointer from
				 * @return Smart pointer to an IAttribute
				 */		
				virtual shared_ptr<IAttribute> handle (xmlpp::Element* _node) 
				{
					shared_ptr<T> attribute( new T() );
					attribute->deSerializeXML(_node);

					return dynamic_pointer_cast<IAttribute>(attribute);
				}

		};

	}
}

#endif // __GENERICATTRIBUTEHANDLER_H__
