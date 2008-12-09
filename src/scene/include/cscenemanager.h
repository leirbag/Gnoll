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


/*-----------------------------CSceneManager-------------------------------*\
|   This is the scene manager                                               |
|                                                                           |
|   Changelog :                                                             |
|               12/05/2007 - Paf - Initial release                          |
|               31/10/2008 - Gabriel - Add cameramanager           GHOST    |
\*-------------------------------------------------------------------------*/


#ifndef __CSCENEMANAGER_H__
#define __CSCENEMANAGER_H__


#include <boost/shared_ptr.hpp>
#include <libxml++/libxml++.h>
#include <glibmm/ustring.h>


#include "../../dynamicobject/include/cdynamicobjectproxy.h"
#include "../../core/include/cpoolthreads.h"
#include "../include/cpage.h"
#include "../../core/include/cmessagelistener.h"

#include "../../dynamicobject/include/string.h"
#include "../../dynamicobject/include/float.h"
#include "../../dynamicobject/include/set.h"

using namespace std;
using namespace boost;
using namespace Gnoll::Core;
using namespace Gnoll::DynamicObject;

namespace Gnoll
{
	namespace Scene
	{


		class CSceneManager : public CDynamicObjectProxy
		{

			private:

				/**
				 * Pool of threads used for all sort of jobs
				 */
				CPoolThreads m_poolOfThreads;


				/**
				 * Listener which will update the scene manager
				 */
				shared_ptr<CMessageListener> sceneUpdateListener;

			public:

				/**
				 * This is a constructor
				 */
				CSceneManager(string _instanceName);


				/**
				 * This is a destructor
				 */
				~CSceneManager();


				/**
				 * Update scene module
				 */
				void update();


				/**
				 * Enqueue a job in the pool of threads
				 */
				void queueJob( shared_ptr<CJob> _job);


				/**
				 * Check if a CPage object is within the camera frustrum
				 * @param _page CPage object to check
				 * @return Whether or not that page is visible
				 */
				bool isPageVisible(const CPage& _page);


				/**
				 * Set up a page (position, etc.)
				 * @param _pageInstance Page Instance to setup
				 * @param _loadedPages List of loaded pages
				 * @param _offset Page offset
				 */
				void setupPage( const string _pageInstance, shared_ptr< Gnoll::DynamicObject::List > _loadedPages, const Ogre::Vector3 _offset = Ogre::Vector3());
		};

	}
}

#endif // __CSCENEMANAGER_H__
