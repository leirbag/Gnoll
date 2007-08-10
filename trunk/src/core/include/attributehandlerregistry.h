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
|               07/09/2007 - Paf - Initial release                          |
|               07/10/2007 - Paf - Add some comments                        |
|                          - Paf - serializeXML() now returns a             |
|                                    shared_ptr<xmlpp::Document>            |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include <boost/shared_ptr.hpp>
#include <libxml++/libxml++.h>
#include <string>

#include "iattributehandler.h"
#include "singleton.h"

// Handlers
#include "genericattributehandler.h"
#include "integer.h"


#ifndef __ATTRIBUTEHANDLERREGISTRY_H__
#define __ATTRIBUTEHANDLERREGISTRY_H__

using namespace boost;
using namespace std;


namespace Viracocha 
{

	namespace Core
	{

		/**
		*	This is the interface of an attribute.
		*	This make sure each Attribute will be (de)serializable
		*/ 
		class AttributeHandlerRegistry: public Singleton<AttributeHandlerRegistry>
		{

			private:

				map<string, shared_ptr<IAttributeHandler> > m_handlersMap;

			public:

				/**
				* This is a constructor.
				*/
				AttributeHandlerRegistry();


				/**
				* This is a destructor.
				*/
				~AttributeHandlerRegistry();


		
				void registerHandler(string _attributeType, shared_ptr<IAttributeHandler> _handler);


				void unregisterHandler(string _attributeType);


				shared_ptr<IAttributeHandler> getHandler(string _attributeType);

		};

	}
}

#endif // __IATTRIBUTEHANDLERREGISTRY_H__
