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


/*--------------------------------CPage------------------------------------*\
|   This is a page. A page is a chunk of the scene.                         |
|                                                                           |
|                                                                           |
\*-------------------------------------------------------------------------*/


#ifndef __CPAGE_H__
#define __CPAGE_H__


#include <boost/shared_ptr.hpp>
#include <libxml++/libxml++.h>
#include <glibmm/ustring.h>
#include <Ogre.h>

#include "../../dynamicobject/include/cdynamicobjectproxy.h"

#include "string.h"


using namespace std;
using namespace boost;
using namespace Gnoll::Core;
using namespace Gnoll::DynamicObject;
using namespace Ogre;


namespace Gnoll
{
	namespace Scene
	{

		/**
		 * A page is a chunk of the scene.<br/>
		 * A scene is made of tiles named pages
		 */
		class CPage : public CDynamicObjectProxy
		{
			private:

			public:

				/**
				 * Returns attribute name "size".<br/>
				 * A page must be square and have a length of "size"
				 * @return The attribute name "size"
				 */
				inline static const char * ATTRIBUTE_SIZE() {return "size";}


				/**
				 * This attribute is the instance name of the northern neighbor
				 * @return Attribute name for the instance name of the northern neighbor
				 */
				inline static const char * ATTRIBUTE_NORTH_LINK() {return "northLink";}


				/**
				 * This attribute is the instance name of the eastern neighbor
				 * @return Attribute name for the instance name of the eastern neighbor
				 */
				inline static const char * ATTRIBUTE_EAST_LINK() {return "eastLink";}


				/**
				 * This attribute is the instance name of the western neighbor
				 * @return Attribute name for the instance name of the western neighbor
				 */
				inline static const char * ATTRIBUTE_WEST_LINK() {return "westLink";}


				/**
				 * This attribute is the instance name of the southern neighbor
				 * @return Attribute name for the instance name of the southern neighbor
				 */
				inline static const char * ATTRIBUTE_SOUTH_LINK() {return "southLink";}


				/**
				 * Page renderer is the object in charge of rendering the page itself
				 * @return The page renderer which is rendering the page
				 */
				inline static const char * ATTRIBUTE_PAGE_RENDERER() {return "PageRenderer";}

				/**
				 * This is a list of all static objects included in the page
				 * @return List of static objects to be rendered
				 */
				inline static const char * ATTRIBUTE_STATIC_OBJECTS() {return "staticObjects";}


				/**
				 * Constructor
				 * @param _instanceName Name of the instance
				 */
				CPage(string _instanceName);


				/**
				 * Destructor
				 */
				~CPage();

				/**
				 * Initialize the page. This include all Ogre related operations <br/>
				 * like creating and attaching scene nodes
				 */
				void init();


				/**
				 * De-initialize the page. This include all Ogre related operations <br/>
				 * like destroying scene nodes
				 */
				void unInit();


				/**
				 * Indicates if this page has been initialized or not
				 * @return Returns whether this page has been initialized or not
				 */
				bool isInitialized();


				/**
				 * Retrieve the root scene node of this page
				 * @return Returns the root scene node of this page
				 *
				 */
				Ogre::SceneNode * getPageRootNode() const;


				/**
				 * Checks if this page is visible from a given camera
				 * @param _cameraName Instance name of the camera which might see this page
				 * @return Returns whether this page is visible from the camera name passed
				 */
				bool isVisibleFromCamera(shared_ptr< Gnoll::DynamicObject::String > _cameraName) const;
		};
	}
}

#endif // __CPAGE_H__
