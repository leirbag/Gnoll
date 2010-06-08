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

#include "../include/dynamicobjectproxy.h"

#include "../include/dynamicobjectmanager.h"

using namespace std;
using namespace boost;

namespace Gnoll
{
	namespace DynamicObject
	{
		DynamicObjectProxy::DynamicObjectProxy(string instanceName)
		{
			DynamicObjectManager* pom = DynamicObjectManager::getInstancePtr();
			m_self = pom->load(instanceName);
		}

		DynamicObjectProxy::~DynamicObjectProxy()
		{
			this->save();
		}

		void DynamicObjectProxy::save()
		{
			DynamicObjectManager* pom = DynamicObjectManager::getInstancePtr();
			pom->save(m_self->getInstance(), m_self);
		}

		void DynamicObjectProxy::setAttribute(const Glib::ustring name, 
				                               shared_ptr<AbstractAttribute> value)
		{
			m_self->setAttribute(name, value);
		}

		bool DynamicObjectProxy::hasAttribute(const Glib::ustring& name) const
		{
			return m_self->hasAttribute(name);
		}

		void DynamicObjectProxy::deleteAttribute(Glib::ustring name)
		{
			m_self->deleteAttribute(name);
		}

		void DynamicObjectProxy::deleteAllAttributes ( void )
		{
			m_self->deleteAllAttributes();
		}

		List DynamicObjectProxy::getAttributesNames(void)
		{
			return m_self->getAttributesNames();
		}

		DynamicObject::mapAttributes::const_iterator DynamicObjectProxy::begin() const
		{
			return m_self->begin();
		}

		DynamicObject::mapAttributes::const_iterator DynamicObjectProxy::end() const
		{
			return m_self->end();
		}

		string DynamicObjectProxy::getInstance() const
		{
			return m_self->getInstance();
		}
	}
}
