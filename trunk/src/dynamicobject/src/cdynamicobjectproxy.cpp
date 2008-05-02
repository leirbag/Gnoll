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


/*---------------------------CDynamicObjectProxy---------------------------*\
|   This is a DynamicObject proxy                                           |
|                                                                           |
|   Changelog :                                                             |
|               12/14/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/




#include "../include/cdynamicobjectproxy.h"
#include "../include/dynamicobjectmanager.h"


using namespace std;
using namespace boost;

namespace Gnoll
{
	namespace DynamicObject
	{


		CDynamicObjectProxy::CDynamicObjectProxy (string _instanceName)
		{
			DynamicObjectManager *pom = DynamicObjectManager::getInstancePtr();
			m_self = pom->load(_instanceName);
		}

		CDynamicObjectProxy::~CDynamicObjectProxy ()
		{
			this->save();
		}

		void CDynamicObjectProxy::save()
		{
			DynamicObjectManager *pom = DynamicObjectManager::getInstancePtr();
			pom->save( m_self->getInstance(), m_self);
		}

		void CDynamicObjectProxy::setAttribute( const Glib::ustring _name, shared_ptr<IAttribute> _value )
		{
			m_self->setAttribute(_name, _value);
		}


		bool CDynamicObjectProxy::hasAttribute ( Glib::ustring _name ) const
		{
			return m_self->hasAttribute(_name);
		}


		void CDynamicObjectProxy::deleteAttribute ( Glib::ustring _name )
		{
			m_self->deleteAttribute(_name);
		}


		void CDynamicObjectProxy::deleteAllAttributes ( void )
		{
			m_self->deleteAllAttributes();
		}


		List CDynamicObjectProxy::getAttributesNames(void)
		{
			return m_self->getAttributesNames();
		}


		DynamicObject::mapAttributes::const_iterator CDynamicObjectProxy::begin() const
		{
			return m_self->begin();
		}


		DynamicObject::mapAttributes::const_iterator CDynamicObjectProxy::end() const
		{
			return m_self->end();
		}


		string CDynamicObjectProxy::getInstance() const
		{
			return m_self->getInstance();
		}

	}
}


