/***************************************************************************
 *   Copyright (C) 2009 by Bruno Mahe                                      *
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


/*-------------------------------GObject-----------------------------------*\
|   This is a game object                                                   |
|                                                                           |
|                                                                           |
\*-------------------------------------------------------------------------*/


#ifndef __GOBJECT_H__
#define __GOBJECT_H__


#include "../../dynamicobject/include/dynamicobject.h"
#include "cpage.h"
#include "icomponent.h"

using namespace std;
using namespace boost;
using namespace Gnoll::Core;
using namespace Gnoll::DynamicObject;
using namespace Ogre;


namespace Gnoll
{
	namespace Scene
	{

		/**
		 * This is a game object
		 */
		class GObject : public Gnoll::DynamicObject::DynamicObject
		{

			public:

				/**
				 * Returns name a GObject DynamicObject
				 * @return The name a GObject DynamicObject
				 */
				inline static const char * DYNAMIC_OBJECT_NAME() {return "gobject";}

				/**
				 * Returns attribute name "components".<br/>
				 * This attribute contains the list of all components belonging to the GObject
				 * @return The attribute name "components"
				 */
				inline static const char * ATTRIBUTE_COMPONENTS() {return "components";}



				/**
				 * Constructor
				 */
				GObject();


				/**
				 * Destructor
				 */
				virtual ~GObject();


				/**
				 * This method serialize the GObject and all its attributes
				 *
				 * @return This return the GObject as a XML tree
				 */
				virtual shared_ptr<xmlpp::Document> serializeXML();


				/**
				 * This method deserialize the GObject and all its attributes from a XML element
				 *
				 * @param _element The XML Element to parse data from
				 */
				//virtual void deSerializeXML( xmlpp::Element* _element );


				/**
				 * Initialize all sub-components
				 * @param page Page where is located the DynamicObject
				 */
				virtual void init(CPage* page);


				/**
				 * Exits all sub-components
				 */
				 virtual void exit();
		};
	}
}

#endif // __GOBJECT_H__
