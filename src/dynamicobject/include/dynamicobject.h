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

#ifndef __DYNAMICOBJECT_H__
#define __DYNAMICOBJECT_H__

#include <map>

#include <boost/shared_ptr.hpp>
#include <libxml++/libxml++.h>
#include <glibmm/ustring.h>

#include "abstractattribute.h"
#include "abstractattributehandler.h"
#include "attributehandlerregistry.h"
#include "string.h"
#include "list.h"

using namespace std;
using namespace boost;

namespace Gnoll
{
	namespace DynamicObject
	{
		/**
		 * Forward declaration
		 */
		class List;

		template <class T> class ErrorPolicyExceptions
		{
			public:
				static shared_ptr<T> attributeNotFound(const Glib::ustring name)
				{
					throw("Attribut [" + name + "] non trouve !");
				}
		};

		template <class T> class NicePolicyExceptions
		{
			public:
				static shared_ptr<T> attributeNotFound(const Glib::ustring name)
				{
					return shared_ptr<T>(new T());
				}
		};

		/**
		 *	This is a Dynamic Object.
		 *	This object maps a unicode string to any object inheriting from 
		 *	AbstractAttribute (ie is (de)serializable).<br/>
		 *  Thus we can store any data or game object through a DynamicObject 
		 *  (as long as it inherits from AbstractAttribute).<br/>
		 *  Each DynamicObject is referenced via its instance name.
		 *  @see AbstractAttribute
		 */
		class DynamicObject : public AbstractAttribute
		{
			public:
				typedef map< Glib::ustring, shared_ptr<AbstractAttribute> > mapAttributes;

			public:
				/**
				 * This is the constructor.
				 * @param _instance This is the instance name; the DynamicObject's name
				 */
				DynamicObject(Glib::ustring _instance = "NULL");

				/**
				 * This is the destructor
				 */
				virtual ~DynamicObject();

				/**
				 * This sets an attribute
				 * @param name The name of the attribute
				 * @param _value The attribute itself
				 */
				void setAttribute(const Glib::ustring name, shared_ptr<AbstractAttribute> _value);

				/**
				 * This return an attribute
				 * @param name The name of the attribute we want
				 * @return The attribute
				 */
				template< class T >
						shared_ptr<T> getAttribute(const Glib::ustring name) const
				{
					typedef NicePolicyExceptions<T> ErrorPolicy;

					mapAttributes::const_iterator iter = m_attributes.find(name);

					// If there is no attribute with such a name, throw an exception.
					if( iter == m_attributes.end())
						return ErrorPolicy::attributeNotFound(name);

					shared_ptr<AbstractAttribute> value = iter->second;
					return dynamic_pointer_cast<T>(value);
				}

				/**
				 * This return an attribute. If the attribute doesn't exists, it will<br/>
				 * return a default value
				 * @param name The name of the attribute we want
				 * @return The attribute if it exists, or a default value
				 */
				template< class T >
				shared_ptr<T> getAttributeOrDefault(const Glib::ustring name,
													shared_ptr<T> defaultValue = shared_ptr<T>())
				{
					if (this->hasAttribute(name))
						return getAttribute<T>(name);
					return defaultValue;
				}

				/**
				 * Check if an attribute exists
				 * @param name Name of the attribute
				 * @return It returns true if an attribute with such a name exists, 
				 * 		   or false if it doesn't
				 */
				bool hasAttribute(const Glib::ustring name)const;

				/**
				 * Delete an attribute
				 * @param name Name of the attribute to delete
				 */
				void deleteAttribute(Glib::ustring name);

				/**
				 * Delete all the attributes
				 */
				void deleteAllAttributes();

				/**
				 * Get a list of attributes names
				 * @return A list of all attribute's names
				 */
				List getAttributesNames();

				/**
				 * Get a const_iterator on the first element of the map of the attributes
				 * @return const_iterator at the beginning of the map of attributes
				 */
				mapAttributes::const_iterator begin() const;

				/**
				 * Get a const_iterator on the end of the map of the attributes
				 * @return const_iterator at the end of the map of attributes
				 */
				mapAttributes::const_iterator end() const;

				/**
				 * Get the instance name of the DynamicObject
				 * @return The instance name
				 */
				string getInstance();

				/**
				 * This method serialize the DynamicObject and all its attributes
				 *
				 * @return This return the DynamicObject as a XML tree
				 */
				virtual shared_ptr<xmlpp::Document> serializeXML();

				/**
				 * This method deserialize the DynamicObject and all its attributes 
				 * from a XML element
				 *
				 * @param _element The XML Element to parse data from
				 */
				virtual void deSerializeXML( xmlpp::Element* _element );

			private:
				/**
				 * This maps all the attributes to their names
				 */
				mapAttributes m_attributes;

				/**
				 * This is the instance name
				 */
				Glib::ustring m_instance;
		};
	}
}

#endif // __DYNAMICOBJECT_H__
