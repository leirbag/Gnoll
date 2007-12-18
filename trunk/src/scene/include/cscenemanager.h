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
|                                                                           |
\*-------------------------------------------------------------------------*/


#ifndef __CSCENEMANAGER_H__
#define __CSCENEMANAGER_H__


#include <boost/shared_ptr.hpp>
#include <libxml++/libxml++.h>
#include <glibmm/ustring.h>


#include "../../core/include/cpersistentobjectproxy.h"
#include "../../core/include/cpoolthreads.h"

#include "string.h"



using namespace std;
using namespace boost;
using namespace Gnoll::Core;

namespace Gnoll
{
	namespace Scene
	{


		class CSceneManager : public CPersistentObjectProxy
		{
		
			private:
	
				CPoolThreads m_poolOfThreads;
				
		
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
				 * Update scene
				 */
				void update();
				
				
				/**
				 * Enqueue a job in the pool of threads
				 */
				void queueJob( shared_ptr<CJob> _job);
		
						
		};
				
	}
}

#endif // __CSCENEMANAGER_H__
