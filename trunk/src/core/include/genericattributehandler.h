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


/*-------------------------------cmessage----------------------------------*\
|   This is the interface of all the attributes. Each Attribute has to be   |
|     (de)serializable                                                      |
|                                                                           |
|   Changelog :                                                             |
|               08/03/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include <boost/shared_ptr.hpp>
#include <libxml++/libxml++.h>

#include "../include/iattributehandler.h"

#ifndef __GENERICATTRIBUTEHANDLER_H__
#define __GENERICATTRIBUTEHANDLER_H__

using namespace boost;


namespace Viracocha 
{

	namespace Core
	{

		/**
		*	This is the interface of an attribute handler.
		*	This make sure each Attribute will be (de)serializable
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
