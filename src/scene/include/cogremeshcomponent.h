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


/*--------------------------COgreMeshComponent-----------------------------*\
|   This is a component able to display Ogre models                         |
|                                                                           |
|                                                                           |
\*-------------------------------------------------------------------------*/

#ifndef COGREMESHCOMPONENT_H_
#define COGREMESHCOMPONENT_H_

#include "cpage.h"
#include "icomponent.h"

using namespace Gnoll::Scene;

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
				Gnoll::Scene::CPage*   m_parentPage;


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
				 * Returns attribute name "scale_x".<br/>
				 * This attribute contains the X scale of the mesh
				 * @return The attribute name "scale_x"
				 */
				inline static const char * ATTRIBUTE_SCALE_X() {return "scale_x";}


				/**
				 * Returns attribute name "scale_Y".<br/>
				 * This attribute contains the Y scale of the mesh
				 * @return The attribute name "scale_y"
				 */
				inline static const char * ATTRIBUTE_SCALE_Y() {return "scale_y";}


				/**
				 * Returns attribute name "scale_z".<br/>
				 * This attribute contains the Z scale of the mesh
				 * @return The attribute name "scale_z"
				 */
				inline static const char * ATTRIBUTE_SCALE_Z() {return "scale_z";}


				/**
				 * Returns attribute name "rot_x".<br/>
				 * This attribute contains the X orientation of the mesh
				 * @return The attribute name "rot_x"
				 */
				inline static const char * ATTRIBUTE_ROTATE_X() {return "rot_x";}


				/**
				 * Returns attribute name "rot_y".<br/>
				 * This attribute contains the Y orientation of the mesh
				 * @return The attribute name "rot_y"
				 */
				inline static const char * ATTRIBUTE_ROTATE_Y() {return "rot_y";}


				/**
				 * Returns attribute name "rot_z".<br/>
				 * This attribute contains the Z orientation of the mesh
				 * @return The attribute name "rot_z"
				 */
				inline static const char * ATTRIBUTE_ROTATE_Z() {return "rot_z";}


				/**
				 * Returns attribute name "pos_x".<br/>
				 * This attribute contains the X position of the mesh
				 * @return The attribute name "pos_x"
				 */
				inline static const char * ATTRIBUTE_POSITION_X() {return "pos_x";}


				/**
				 * Returns attribute name "pos_y".<br/>
				 * This attribute contains the Y position of the mesh
				 * @return The attribute name "pos_y"
				 */
				inline static const char * ATTRIBUTE_POSITION_Y() {return "pos_y";}


				/**
				 * Returns attribute name "pos_z".<br/>
				 * This attribute contains the Z position of the mesh
				 * @return The attribute name "pos_z"
				 */
				inline static const char * ATTRIBUTE_POSITION_Z() {return "pos_z";}


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
				Gnoll::Scene::CPage *getParentPage() const;


				/**
				 * Setter for m_parentPage
				 */
				void setParentPage(Gnoll::Scene::CPage *m_parentPage);


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

