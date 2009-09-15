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

#ifndef CROTATIONCOMPONENT_H_
#define CROTATIONCOMPONENT_H_

#include "cpage.h"
#include "icomponent.h"
#include "../../core/messages/include/listener.h"
#include "../include/gobject.h"
#include <boost/shared_ptr.hpp>

using namespace boost;
using namespace Gnoll::Scene;
using namespace Gnoll::Core;

namespace Gnoll
{

	namespace Scene
	{
		/**
     * This is a component able to set a rotation
		 */
		class CRotationComponent: public Gnoll::Scene::IComponent
		{
			private:
				Ogre::Vector3                rotation;

				Gnoll::Scene::GObject*       parent;

				shared_ptr<Messages::Listener> rotationListener;

			public:

				/**
				 * Returns CRotationComponent's DynamicObject name
				 * @return CRotationComponent's DynamicObject name
				 */
				inline static const char * DYNAMIC_OBJECT_NAME() {return "CRotationComponent";}


				/**
				 * Returns attribute name "pos".<br/>
				 * This attribute contains the rotation of the mesh
				 * @return The attribute name "pos"
				 */
				inline static const char * ATTRIBUTE_ROTATION() {return "rotation";}


				/**
				 * This is a constructor
				 */
				CRotationComponent();


				/**
				 * This is a destructor
				 */
				virtual ~CRotationComponent();


				/**
				 * Settor of the rotation
				 * @param pos The new rotation
				 */
				void setRotation(const Ogre::Vector3& rot) { rotation = rot; }


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
				 * This method serialize the CRotationComponent and all its attributes
				 *
				 * @return This return the CRotationComponent as a XML tree
				 */
				virtual shared_ptr<xmlpp::Document> serializeXML();


		};
	}
}

#endif /* CROTATIONCOMPONENT_H_ */
