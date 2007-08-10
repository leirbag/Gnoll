/***************************************************************************
 *   Copyright (C) 2006 by Puzzle Team                                     *
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


/*-------------------------------BaseManager-------------------------------*\
|   This is basic manager with cache ability.                               |
|                                                                           |
|                                                                           |
|   Changelog :                                                             |
|               07/20/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/



#include "basemanager.h"
#include "singleton.h"
#include "persistentobject.h"
#include "attributehandlerregistry.h"

#ifndef __PERSISTENTOBJECTMANAGER_H__
#define __PERSISTENTOBJECTMANAGER_H__

using namespace std;
using namespace boost;



namespace Viracocha {

	namespace Core {


		class PersistentObjectManager : public BaseManager<PersistentObject>, public Singleton<PersistentObjectManager>
		{

			protected:

				virtual shared_ptr<PersistentObject> loadImpl( shared_ptr<IStream> _stream);

				virtual bool saveImpl( shared_ptr<IStream> _stream, shared_ptr<PersistentObject> _obj);

			public:

				/**
				 * This is a constructor.
				 */
				PersistentObjectManager();


				/**
				 * This is a destructor.
				 */
				virtual ~PersistentObjectManager();

		};

	}
}

#endif // __PERSISTENTOBJECTMANAGER_H__

