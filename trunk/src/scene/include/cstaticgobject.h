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


/*-----------------------------CStaticGObject------------------------------*\
|   This is a static game object                                            |
|                                                                           |
|                                                                           |
\*-------------------------------------------------------------------------*/


#ifndef __CSTATICGOBJECT_H__
#define __CSTATICGOBJECT_H__


#include "../../dynamicobject/include/dynamicobject.h"
#include "cpage.h"

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
		 * This is a static game object.<br/>
		 * This shouldn't be able to interact with anything else, except for collisions<br/>
		 * <br/>
		 * Dynamic attributes :
		 *  * name         -> name of this object
		 *  * mesh         -> file name of the mesh file
		 *  * description  -> Description of this object
		 *  * pos_x        -> Position along X axis
		 *  * pos_y        -> Position along Y axis
		 *  * pos_z        -> Position along Z axis
		 *  * rot_x        -> Rotation around X axis
		 *  * rot_y        -> Rotation around Y axis
		 *  * rot_z        -> Rotation around Z axis
		 *  * scale_x      -> Scale on X axis
		 *  * scale_y      -> Scale on Y axis
		 *  * scale_z      -> Scale on Z axis
		 *
		 */
		class CStaticGObject : public Gnoll::DynamicObject::DynamicObject
		{
			
			public:


				/**
				 * Constructor
				 */
				CStaticGObject();
				

				/**
				 * Destructor
				 */
				virtual ~CStaticGObject();

				
				/**
				 * Initialization of the static game object.<br/>
				 * This is about instanciating the model
				 * @param _parentPage Page which will contain the CStaticGObject
				 */
				void init( CPage* _parentPage );

				/**
				 * In case the model needs any update
				 */
				void update();

				/**
				 * When game exits all static game objects need to free all memory they use.
				 * @param _parentPage Page which contains the CStaticGObject
				 */
				void exit( CPage* _parentPage );
	

				/**
				 * This method serialize the CStaticGObject and all its attributes
				 *
				 * @return This return the CStaticGObject as a XML tree
				 */
				virtual shared_ptr<xmlpp::Document> serializeXML();


				/**
				 * This method deserialize the CStaticGObject and all its attributes from a XML element
				 *
				 * @param _element The XML Element to parse data from
				 */
				//virtual void deSerializeXML( xmlpp::Element* _element );

		};
	}
}

#endif // __CSTATICGOBJECT_H__
