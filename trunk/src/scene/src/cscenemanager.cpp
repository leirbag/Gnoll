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



#include "../include/cscenemanager.h"
#include "../include/cpage.h"



using namespace std;
using namespace boost;
using namespace Gnoll::Core;

namespace Gnoll
{
	namespace Scene
	{


		CSceneManager::CSceneManager(string _instanceName) : CPersistentObjectProxy(_instanceName)
		{
		}
				

		CSceneManager::~CSceneManager()
		{
		}
			
				
		void CSceneManager::update()
		{
			// Parcourt des sets de pages pour verifier qu'ils soient tous visibles, ainsi que leurs voisins [tant que les voisins sont visibles].
			shared_ptr<Gnoll::Core::String> focusedPageName = this->getAttribute<Gnoll::Core::String>("focusedPage");
			
			if (focusedPageName.get())
			{
			//XXX fixme
				CPage focusPage(*focusedPageName);
				
				
				
			}
			
		}
				
				
		void CSceneManager::queueJob( shared_ptr<CJob> _job)
		{
			m_poolOfThreads.pushJob(_job);
		}
		
				
	}
}


