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


/*----------------------------persistentobject-----------------------------*\
|   This is a PersistentObject. An object that can store any kind of data   |
|     and can be readt/written from anywhere                                |
|                                                                           |
|   Changelog :                                                             |
|               07/09/2007 - Paf - Initial release                          |
|               07/10/2007 - Paf - Add Doxygen comments                     |
|                          - Paf - Implements                               |
|                                     PersistentObject::serializeXML()      |
|               08/10/2007 - Paf - Add Doxygen comments                     |
|               10/11/2007 - Paf - Add method getAttributesNames()          |
|               						- Make const_iterator on attributes        |
|                                    possible                               |
|               12/15/2007 - Paf - Add policies when an attribute is not    |
|                                    not found                              |
|               02/15/2008 - Bruno Mahe - Add a new method                  |
|                                    getAttributeOrDefault()                |
|                                                                           |
\*-------------------------------------------------------------------------*/


#ifndef __PERSISTENTOBJECT_H__
#define __PERSISTENTOBJECT_H__


#include <boost/shared_ptr.hpp>
#include <libxml++/libxml++.h>
#include <glibmm/ustring.h>
#include <map>

#include "iattribute.h"
#include "iattributehandler.h"
#include "attributehandlerregistry.h"

#include "string.h"
#include "list.h"


using namespace std;
using namespace boost;

namespace Gnoll
{
	namespace Core
	{

		/**
		 * Forward declaration
		 */
		class List;

		
		template <class T> class ErrorPolicyExceptions
		{
			public:
			
				static shared_ptr<T> attributeNotFound( const Glib::ustring _name )
				{
					throw("Attribut [" + _name + "] non trouve !");
				}
		
		};

		template <class T> class NicePolicyExceptions
		{
			public:
			
				static shared_ptr<T> attributeNotFound( const Glib::ustring _name )
				{
					return shared_ptr<T>( new T() );
				}
		
		};



		/**
		 *	This is a Persistent Object.
		 *	This object maps a unicode string to any object inheriting from IAttribute (ie is (de)serializable).<br/>
		 * Thus we can store any data or game object through a PersistentObject (as long as it inherits from IAttribute).<br/>
		 * Each PersistentObject is referenced via its instance name.
		 * @see IAttribute
		 */ 
		class PersistentObject : public IAttribute
		{
			public: 

				typedef map< Glib::ustring, shared_ptr<IAttribute> > mapAttributes;


			private:	

				/**
				 * This maps all the attributes to their names
				 */
				mapAttributes m_attributes;

				/**
				 * This is the instance name
				 */
				Glib::ustring m_instance;

						
			public:


				/**
				 * This is the constructor.
				 * @param _instance This is the instance name; the PersistentObject's name
				 */
				PersistentObject(Glib::ustring _instance);

				/**
				 * This is the destructor
				 */
				virtual ~PersistentObject();


				/**
				 * This sets an attribute
				 * @param _name The name of the attribute
				 * @param _value The attribute itself
				 */
				void setAttribute( const Glib::ustring _name, shared_ptr<IAttribute> _value );
		

				/**
				 * This return an attribute 
				 * @param _name The name of the attribute we want
				 * @return The attribute
				 */
				template< class T > 
						shared_ptr<T> getAttribute( const Glib::ustring _name  ) const
				{
					typedef  NicePolicyExceptions<T> ErrorPolicy;
	
	
					mapAttributes::const_iterator iter = m_attributes.find(_name);
	
					// If there is no attribute with such a name, throw an exception.
					if( iter == m_attributes.end() ) {
						return ErrorPolicy::attributeNotFound(_name);						
					}


					shared_ptr<IAttribute> value = iter->second;	
					return dynamic_pointer_cast<T>(value);
				}

	
				/**
				 * This return an attribute. If the attribute doesn't exists, it will<br/>
				 * return a default value
				 * @param _name The name of the attribute we want
				 * @return The attribute if it exists, or a default value
				 */
				template< class T >
						shared_ptr<T> getAttributeOrDefault( const Glib::ustring _name, shared_ptr<T> _defaultValue = shared_ptr<T>()  )
				{
					if (this->hasAttribute(_name))
					{
						return getAttribute<T>(_name);
					}

					return _defaultValue;
				}


				/**
				 * Check if an attribute exists
				 * @param _name Name of the attribute
				 * @return It returns true if an attribute with such a name exists, or false if it doesn't
				 */
				bool hasAttribute ( Glib::ustring _name ) const;


				/**
				 * Delete an attribute
				 * @param _name Name of the attribute to delete
				 */
				void deleteAttribute ( Glib::ustring _name );


				/**
				 * Delete all the attributes
				 */
				void deleteAllAttributes ( void ) ;

				/**
				 * Get a list of attributes names
				 * @return A list of all attribute's names
				 */
				List getAttributesNames(void);

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
				 * Get the instance name of the PersistentObject
				 * @return The instance name
				 */
				string getInstance();


				/**
				 * This method serialize the PersistentObject and all its attributes
				 *
				 * @return This return the PersistentObject as a XML tree 
				 */
				virtual shared_ptr<xmlpp::Document> serializeXML(); 
		
		
				/**
				 * This method deserialize the PersistentObject and all its attributes from a XML element
				 *
				 * @param _element The XML Element to parse data from 
				 */
				virtual void deSerializeXML( xmlpp::Element* _element );		
		};
	}
}

#endif // __PERSISTENTOBJECT_H__
