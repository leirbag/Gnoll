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

#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include <OgreVector3.h>

#include "abstractattribute.h"

namespace Gnoll
{
	namespace DynamicObject
	{
		/**
		 *	This is a simple attribute. </br>
		 *	This hold a signed integer.
		 */
		class Vector3 : public AbstractAttribute, public Ogre::Vector3
		{
			public:
				/**
				 * Returns Vector3's DynamicObject name
				 * @return Vector3's DynamicObject name
				 */
				inline static const char* DYNAMIC_OBJECT_NAME() { return "vector3"; }

				/**
				 * Returns attribute name "x".<br/>
				 * This attribute contains the X component of a vector
				 * @return The attribute name "x"
				 */
				inline static const char* ATTRIBUTE_X() { return "x"; }

				/**
				 * Returns attribute name "y".<br/>
				 * This attribute contains the Y component of a vector
				 * @return The attribute name "y"
				 */
				inline static const char* ATTRIBUTE_Y() { return "y"; }

				/**
				 * Returns attribute name "z".<br/>
				 * This attribute contains the Z component of a vector
				 * @return The attribute name "z"
				 */
				inline static const char* ATTRIBUTE_Z() { return "z"; }

				/**
				 * This is a constructor
				 */
				Vector3(const Ogre::Vector3& vec = Ogre::Vector3(0, 0, 0));

				/**
				 * This is a destructor
				 */
				virtual ~Vector3();

				/**
				 * This method serialize the object. <br/>
				 * It has to be implemented by all classes that inherits from this class.
				 *
				 * @return This return the object as a XML tree
				 */
				virtual shared_ptr<xmlpp::Document> serializeXML();

				/**
				 * This method deserialize the object. <br/>
				 * This method initializes this object thanks to a XML tree given as a parameter. <br/>
				 * It has to be implemented by all classes that inherits from this class.
				 *
				 * @param _element This is the XML tree containing the state of this object
				 */
				virtual void deSerializeXML(xmlpp::Element* element);
		};
	};
};

#endif
