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

#include "../../core/include/cpersistentobjectproxy.h"

#include "string.h"


using namespace std;
using namespace boost;
using namespace Gnoll::Core;
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
		class CPage : public CPersistentObjectProxy
		{
			private:
			
			public:
				CPage(string _instanceName);
				
				~CPage();

				void init();
				void unInit();
				
				Ogre::SceneNode * getPageRootNode();
		};
	}
}

#endif // __CPAGE_H__
