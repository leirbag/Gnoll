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

#ifndef CPOSITIONCOMPONENT_H_
#define CPOSITIONCOMPONENT_H_

#include "../../../page/include/cpage.h"
#include "../../../../core/messages/include/listener.h"
#include "../../include/gobject.h"
#include "icomponent.h"
#include <boost/shared_ptr.hpp>

using namespace boost;
using namespace Gnoll::Scene;
using namespace Gnoll::Core;

namespace Gnoll
{

	namespace Scene
	{
		/**
		 * This is a component able to set a position.	
		 */
		class CPositionComponent: public Gnoll::Scene::IComponent
		{
			private:
				Ogre::Vector3                position;

				Gnoll::Scene::GObject*       parent;

				shared_ptr<Messages::Listener> positionListener;

			public:

				/**
				 * Returns CPositionComponent's DynamicObject name
				 * @return CPositionComponent's DynamicObject name
				 */
				inline static const char * DYNAMIC_OBJECT_NAME() {return "CPositionComponent";}


				/**
				 * Returns attribute name "pos".<br/>
				 * This attribute contains the position of the mesh
				 * @return The attribute name "pos"
				 */
				inline static const char * ATTRIBUTE_POSITION() {return "position";}


				/**
				 * This is a constructor
				 */
				CPositionComponent();


				/**
				 * This is a destructor
				 */
				virtual ~CPositionComponent();


				/**
				 * Settor of the position
				 * @param pos The new position
				 */
				void setPosition(const Ogre::Vector3& pos) { position = pos; }


				/**
				 * Component initialization
				 * @param parent DynamicObject this component belongs to
				 * @param page Page where is located the parent DynamicObject
				 */
				virtual void init(Gnoll::Scene::GObject* parent, Gnoll::Scene::CPage* page);


				/**
				 * Component exits
				 */
				 virtual void exit();


				/**
				 * This method serialize the CPositionComponent and all its attributes
				 *
				 * @return This return the CPositionComponent as a XML tree
				 */
				virtual shared_ptr<xmlpp::Document> serializeXML();


		};
	}
}

#endif /* CPOSITIONCOMPONENT_H_ */


