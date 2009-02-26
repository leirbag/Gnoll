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
|   This is a page                                                          |
|                                                                           |
|   Changelog :                                                             |
|               12/04/2007 - Paf - Initial release                          |
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
		 * Dynamic attributes :
		 *  * size -> lentgh of one side
		 *  * northLink    -> Instance name of the northern neighbor
		 *  * southLink    -> Instance name of the southern neighbor
		 *  * eastLink     -> Instance name of the eastern neighbor
		 *  * westLink     -> Instance name of the western neighbor
		 *  * initialized  -> Is that page initialized ? yes or non-existant
		 *  * PageRenderer -> IPageREnderer object in charge of displaying the ground
		 *
		 */
		class CPage : public CDynamicObjectProxy
		{
			private:

			public:

				/**
				 * Returns attribute size name.<br/>
				 * A page must be squared and have a length of "size"
				 * @return Returns the name of the size attribute
				 */
				inline static const char * ATTRIBUTE_SIZE() {return "size";}
				inline static const char * ATTRIBUTE_NORTH_LINK() {return "northLink";}
				inline static const char * ATTRIBUTE_EAST_LINK() {return "eastLink";}
				inline static const char * ATTRIBUTE_WEST_LINK() {return "westLink";}
				inline static const char * ATTRIBUTE_SOUTH_LINK() {return "southLink";}
				inline static const char * ATTRIBUTE_PAGE_RENDERER() {return "PageRenderer";}
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
