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

#ifndef __ATTRIBUTEHANDLERREGISTRY_H__
#define __ATTRIBUTEHANDLERREGISTRY_H__

#include <string>

#include <boost/shared_ptr.hpp>

#include "abstractattributehandler.h"
#include "../../core/include/singleton.h"

using namespace boost;
using namespace std;
using namespace Gnoll::Core;

namespace Gnoll
{
	namespace DynamicObject
	{
		/**
		 * This is where an attribute has its handler registered.</br>
		 * This is a Singleton.
		 */ 
		class AttributeHandlerRegistry : public Singleton<AttributeHandlerRegistry>
		{
			public:
				/**
				* This is a constructor.
				*/
				AttributeHandlerRegistry();

				/**
				* This is a destructor.
				*/
				~AttributeHandlerRegistry();

				/**
				 * This method provides a way to register an attribute handler
				 * @param _attributeType The attribute to associate the attribute handler with
				 * @param _handler The attributehHandler
				 */	
				void registerHandler(string _attributeType, shared_ptr<AbstractAttributeHandler> _handler);

				/**
				 *	This method unregisters an attribute handler
				 *	@param _attributeType The attribute that is going to be unregistered
				 */
				void unregisterHandler(string _attributeType);

				/**
				 * This method returns the attribute handler associated to an attribute type
				 * @param _attributeType The attribute type
				 */
				shared_ptr<AbstractAttributeHandler> getHandler(string _attributeType);

			private:
				/**
				 * This the map that associates an attribute handler to its attribute
				 */
				map<string, shared_ptr<AbstractAttributeHandler> > m_handlersMap;
		};
	}
}

#endif // __IATTRIBUTEHANDLERREGISTRY_H__
