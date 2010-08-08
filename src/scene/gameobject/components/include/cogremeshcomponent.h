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

#ifndef COGREMESHCOMPONENT_H_
#define COGREMESHCOMPONENT_H_

#include "../../../page/include/cpage.h"
#include "../../../../core/messages/include/listener.h"
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
		 * This is a component able to display Ogre models
		 */
		class COgreMeshComponent: public Gnoll::Scene::IComponent
		{

			private:

				/**
				 * GObject this component belongs to
				 */
				Gnoll::Scene::GObject* m_parent;


				/**
				 * Page where the parent GObject is located
				 */
				std::string   m_parentPageName;

                SceneNode *parentNode;

				/**
				 * Listener of the component
				 */
				shared_ptr<Messages::Listener> componentPositionListener;
				shared_ptr<Messages::Listener> componentScalingListener;
				shared_ptr<Messages::Listener> componentRotationListener;

			public:

				/**
				 * Returns COgreMeshComponent's DynamicObject name
				 * @return COgreMeshComponent's DynamicObject name
				 */
				inline static const char * DYNAMIC_OBJECT_NAME() {return "COgreMeshComponent";}


				/**
				 * Returns attribute name "mesh".<br/>
				 * This attribute contains the name of the mesh to be rendered
				 * @return The attribute name "mesh"
				 */
				inline static const char * ATTRIBUTE_MESH() {return "mesh";}


				/**
				 * Returns the suffix of the scene node created from the instance name.<br/>
				 * @return Scene node suffix
				 */
				inline static const char * ENTITY_SUFFIX() {return "_entity";}


				/**
				 * This is a constructor
				 */
				COgreMeshComponent();


				/**
				 * This is a destructor
				 */
				virtual ~COgreMeshComponent();


				/**
				 * Getter for m_parent
				 */
				Gnoll::Scene::GObject *getParent() const;


				/**
				 * Setter for m_parent
				 */
				void setParent(Gnoll::Scene::GObject *m_parent);


				/**
				 * Getter for m_parentPage
				 */
				const std::string& getParentPageName() const;


				/**
				 * Setter for m_parentPage
				 */
				void setParentPageName(const std::string& m_parentPageName);


				/**
				 * Setter for m_parentPage
				 */
				void setParentPage(const std::string& m_parentPageName);


				/**
				 * Settor of the position of the mesh
				 */
				void setPosition(const Ogre::Vector3& position);


				/**
				 * Settor of the scale of the mesh
				 */
				void setScaling(const Ogre::Vector3& scale);


				/**
				 * Settor of the scale of the mesh
				 */
				void setRotation(const Ogre::Vector3& rotation);


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
				 * This method serialize the COgreMeshComponent and all its attributes
				 *
				 * @return This return the COgreMeshComponent as a XML tree
				 */
				virtual shared_ptr<xmlpp::Document> serializeXML();


		};
	}
}

#endif /* COGREMESHCOMPONENT_H_ */

