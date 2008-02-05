/**************************************************************************
*   Copyright (C) 2008 by Bruno Mahe, Gabriel, Soax                       *
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


/*-------------------------------SoundManager.h----------------------------*\
|   This is basic sound manager with cache ability.                         |
|                                                                           |
|                                                                           |
|   Changelog :                                                             |
|               11/06/2007 - Soax - Initial release                         |
|               02/04/2008 - Bruno Mahe - Add some doxygen comments         |
|                                                                           |
\*-------------------------------------------------------------------------*/



#include "../../core/include/basemanager.h"
#include "../../core/include/singleton.h"
#include "../../core/include/attributehandlerregistry.h"
#include "../../core/include/sourcefile.h"
#include "sound.h"
#include "audiocodecmanager.h"
#include <iostream>


#ifndef __SOUNDMANAGER_H__
#define __SOUNDMANAGER_H__



using namespace std;
using namespace boost;

using namespace Gnoll::Core;


namespace Gnoll {

	namespace Sound {
	
		/**
		 * Sound manager <br/>
		 * * This singleton is in charge of loading and caching Sound objects
		 */
		class SoundManager : public BaseManager<Sound, ObjectNotFoundError<Sound> >, public Singleton<SoundManager>
		{

			protected:

				/**
				 * @copydoc BaseManager::loadImpl
				 */
				virtual shared_ptr<Sound> loadImpl( shared_ptr<IStream> _stream, string _instance);

				/**
				 * @copydoc BaseManager::saveImpl
				 */
				virtual bool saveImpl( shared_ptr<IStream> _stream, shared_ptr<Sound> _obj, string _instance);

			public:
	
				/**
				* This is a constructor.
				*/
				SoundManager();
			
			
				/**
				* This is a destructor.
				*/
				virtual ~SoundManager();


		};
	}
}

#endif
