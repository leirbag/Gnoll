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

#include "../include/gobject.h"
#include "../../graphic/include/cgraphicmodule.h"
#include "../../log/include/logmodule.h"
#include "../include/cpage.h"
#include <glibmm/ustring.h>
#include "../../dynamicobject/include/float.h"

#include <iostream>
using namespace std;
using namespace boost;
using namespace Ogre;
using namespace Gnoll::Graphic;

namespace Gnoll
{
	namespace Scene
	{


		GObject::GObject():
				DynamicObject::DynamicObject(Glib::ustring(""))
		{

			Gnoll::Log::LogModule::getInstancePtr()->logMessage( "GObject object created");
		}


		GObject::~GObject()
		{
		}

		shared_ptr<xmlpp::Document> GObject::serializeXML()
		{
			shared_ptr<xmlpp::Document> document = DynamicObject::serializeXML();
			xmlpp::Element* root = document->get_root_node();

			if (root != NULL)
			{
				root->set_name( GObject::DYNAMIC_OBJECT_NAME() );
			}

			return document;
		}


		void GObject::init(CPage* page)
		{
			/**
			 * Initialize components
			 */
			if (this->hasAttribute( GObject::ATTRIBUTE_COMPONENTS() ))
			{
				shared_ptr< Gnoll::DynamicObject::List > listComponents = this->getAttribute < Gnoll::DynamicObject::List > ( GObject::ATTRIBUTE_COMPONENTS() );

				typedef list< shared_ptr<Gnoll::DynamicObject::AbstractAttribute> >::iterator ListIterator;

				for( ListIterator it = listComponents->begin(); it != listComponents->end(); it++)
				{
					if (shared_ptr<Gnoll::Scene::IComponent> component = dynamic_pointer_cast<Gnoll::Scene::IComponent>(*it))
					{
						component->init(this, page);
					}
				}
			}

		}


		void GObject::exit()
		{
			/**
			 * DeInitialize components
			 */
			if (this->hasAttribute( GObject::ATTRIBUTE_COMPONENTS() ))
			{
				shared_ptr< Gnoll::DynamicObject::List > listComponents = this->getAttribute < Gnoll::DynamicObject::List > ( GObject::ATTRIBUTE_COMPONENTS() );

				typedef list< shared_ptr<Gnoll::DynamicObject::AbstractAttribute> >::iterator ListIterator;

				for( ListIterator it = listComponents->begin(); it != listComponents->end(); it++)
				{
					if (shared_ptr<Gnoll::Scene::IComponent> component = dynamic_pointer_cast<Gnoll::Scene::IComponent>(*it))
					{
						component->exit();
					}
				}
			}


		}
	}
}


