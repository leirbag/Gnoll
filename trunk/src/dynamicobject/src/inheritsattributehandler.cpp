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


/*--------------------------InheritsAttributeHandler-----------------------*\
|   This is an attribute handler for Inherits attribute.                    |
|                                                                           |
|                                                                           |
|   Changelog :                                                             |
|               10/15/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../include/inheritsattributehandler.h"

#include "../include/persistentobjectmanager.h"
#include "../include/inherits.h"


using namespace boost;
using namespace Gnoll::Core;


namespace Gnoll
{

	namespace Core
	{


		InheritsAttributeHandler::InheritsAttributeHandler()
		{
		}


		InheritsAttributeHandler::~InheritsAttributeHandler() 
		{
		}


		shared_ptr<IAttribute> InheritsAttributeHandler::handle (xmlpp::Element* _node, PersistentObject* _po) 
		{
			/**
			 * Deserialization
			 */
			shared_ptr<Inherits> attribute( new Inherits() );
			attribute->deSerializeXML(_node);

			/**
			 * We only apply inheritance if the target PersistentObject is provided
			 */
			if (_po != NULL)
			{

				/**
				 * The first step is to load the parent object
				 */

				String parentName = attribute->getParent();


				PersistentObjectManager *pom = PersistentObjectManager::getInstancePtr();
			

				/**
				 * We release the parent if it's in the cache. So the child and the parent will not share any attribute.
				 * We want a copy of these attributes, not a reference to their attributes.
				 * Onw way to simulate a copy is to save and release first and then copy the references of each attribute, and 
				 * finally release the parent again.
				 */
				if (pom->isInstanceInCache(parentName)) 
				{
					pom->save(parentName);
					pom->release(parentName);
				}


				shared_ptr<PersistentObject> parent = pom->load(parentName);


				/**
				 * The second step is to copy all the parent's attributes if they don't exists in the child object
				 */

				for (PersistentObject::mapAttributes::const_iterator it = parent->begin(); it != parent->end(); it++)
				{
					string attrName = (*it).first;

					shared_ptr<IAttribute> attr = (*it).second;

					if ( _po->hasAttribute(attrName) == false)
					{
						_po->setAttribute(attrName, attr);
					}
				}

				/**
				 * Release of the parent, so no one will share the same attributes
				 */
				pom->release(parentName);

			}


			return dynamic_pointer_cast<IAttribute>(attribute);
		}

	}
}

