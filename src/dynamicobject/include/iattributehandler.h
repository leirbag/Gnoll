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


/*----------------------------iattributehandler----------------------------*\
|   This is the interface of all the attributes handlers.                   |
|                                                                           |
|   Changelog :                                                             |
|               08/03/2007 - Paf - Initial release                          |
|               08/14/2007 - Paf - Update comments                          |
|               08/25/2007 - Paf - Replace namespace Viraoccha by Gnoll     |
|               10/12/2007 - Paf - Add a new parameter to handle()          |
|                                                                           |
\*-------------------------------------------------------------------------*/



#ifndef __IATTRIBUTEHANDLER_H__
#define __IATTRIBUTEHANDLER_H__

#include <boost/shared_ptr.hpp>
#include <libxml++/libxml++.h>

#include "iattribute.h"


using namespace boost;


namespace Gnoll 
{

	namespace DynamicObject
	{

		/**
		 * Forward declaration
		 */
		class DynamicObject;


		/**
		*	This is the interface of an attribute handler.
		*	This make sure each Attribute will be deserializable
		*/ 
		class IAttributeHandler
		{

			public:

				/**
				* This is a constructor.
				*/
				IAttributeHandler() {}


				/**
				* This is a destructor.
				*/
				virtual ~IAttributeHandler() {}


				/**
				 * This is the handler
				 * @param _node The attribute passed as a xml tree
				 * @return Smart pointer to the deserialized IAttribute
				 */
				virtual shared_ptr<IAttribute> handle (xmlpp::Element* _node, DynamicObject* _po = NULL) = 0;

		};

	}
}

#endif // __IATTRIBUTEHANDLER_H__
