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


/*--------------------------CPersistentObjectProxy-------------------------*\
|   This is a PersistentObject proxy                                        |
|                                                                           |
|   Changelog :                                                             |
|               12/14/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/




#include "../include/cpersistentobjectproxy.h"
#include "../include/persistentobjectmanager.h"


using namespace std;
using namespace boost;

namespace Gnoll
{
	namespace Core
	{


		CPersistentObjectProxy::CPersistentObjectProxy (string _instanceName) 
		{
			PersistentObjectManager *pom = PersistentObjectManager::getInstancePtr();
			m_self = pom->load(_instanceName);
		}

		CPersistentObjectProxy::~CPersistentObjectProxy ()
		{
			this->save();
		}

		void CPersistentObjectProxy::save()
		{
			PersistentObjectManager *pom = PersistentObjectManager::getInstancePtr();
			pom->save( m_self->getInstance(), m_self);
		}

		void CPersistentObjectProxy::setAttribute( const Glib::ustring _name, shared_ptr<IAttribute> _value )
		{
			m_self->setAttribute(_name, _value);
		}


		bool CPersistentObjectProxy::hasAttribute ( Glib::ustring _name )
		{
			return m_self->hasAttribute(_name);
		}


		void CPersistentObjectProxy::deleteAttribute ( Glib::ustring _name )
		{
			m_self->deleteAttribute(_name);
		}


		void CPersistentObjectProxy::deleteAllAttributes ( void )
		{
			m_self->deleteAllAttributes();
		}


		List CPersistentObjectProxy::getAttributesNames(void)
		{
			return m_self->getAttributesNames();
		}


		PersistentObject::mapAttributes::const_iterator CPersistentObjectProxy::begin() const
		{
			return m_self->begin();
		}
	

		PersistentObject::mapAttributes::const_iterator CPersistentObjectProxy::end() const
		{
			return m_self->end();
		}


		string CPersistentObjectProxy::getInstance() const
		{
			return m_self->getInstance();
		}
		
	}
}


