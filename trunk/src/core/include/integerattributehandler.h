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


/*-------------------------integerattributehandler-------------------------*\
|   This attribute handler can deserialize Integer attributes               |
|                                                                           |
|   Changelog :                                                             |
|               08/03/2007 - Paf - Initial release                          |
|               08/14/2007 - Paf - Update comments                          |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include <boost/shared_ptr.hpp>
#include <libxml++/libxml++.h>

#include "../include/iattributehandler.h"

#ifndef __INTEGERATTRIBUTEHANDLER_H__
#define __INTEGERATTRIBUTEHANDLER_H__

using namespace boost;


namespace Viracocha 
{

	namespace Core
	{

		/**
		*	This attribute handler deserialize Integer attributes.
		*/ 
		class IntegerAttributeHandler : public IAttributeHandler
		{

			public:

				/**
				* This is a constructor.
				*/
				IntegerAttributeHandler() {]


				/**
				* This is a destructor.
				*/
				~IntegerAttributeHandler() {}


				/**
				 * This is the handler
				 * @param _node The attribute passed as a xml tree
				 * @return Smart pointer to the deserialized IAttribute
				 */
				virtual shared_ptr<IAttribute> handle (xmlpp::Element* _node) 
				{
					shared_ptr<Integer> integer = new Integer();
					integer->deSerialize(_node);

					return dynamic_pointer_cast<IAttribute>(integer);
				}

		};

	}
}

#endif // __INTEGERATTRIBUTEHANDLER_H__
