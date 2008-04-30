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


/*-------------------------PersistentObjectManager-------------------------*\
|   This is a singleton object managing PersistentObjects                   |
|                                                                           |
|                                                                           |
|   Changelog :                                                             |
|               07/20/2007 - Paf - Initial release                          |
|               08/16/2007 - Paf - Update comments                          |
|               09/25/2007 - Paf - Replace namespace Viracocha by Gnoll     |
|               12/15/2007 - Paf - Add BaseManager policy                   |
|                                                                           |
\*-------------------------------------------------------------------------*/



#ifndef __PERSISTENTOBJECTMANAGER_H__
#define __PERSISTENTOBJECTMANAGER_H__

#include "basemanager.h"
#include "singleton.h"
#include "persistentobject.h"


using namespace std;
using namespace boost;



namespace Gnoll {

	namespace Core {

		/**
		 * PersistentObjectManager is a singleton managing PersistentObject instances.</br>
		 * It can load/save from any Source and provide a cache to improve performance (using LRU)
		 */
		class PersistentObjectManager : public BaseManager<PersistentObject, ObjectNotFoundNewObject<PersistentObject> >, public Singleton<PersistentObjectManager>
		{

			private:

				/**
				 * This method loads a PersistentObject from a stream
				 * @param _stream Stream from which the ressource will be extracted
				 * @return Smart pointer to the newly created object
				 */
				virtual shared_ptr<PersistentObject> loadImpl( shared_ptr<IStream> _stream, string _instance);

				/**
				 * This method saves a PersistentObject to a stream
				 * @param _stream Stream to which the ressource will be saved
				 * @param _obj Smart pointer to the object to be saved
				 * @return The successfulness of this operation
				 */
				virtual bool saveImpl( shared_ptr<IStream> _stream, shared_ptr<PersistentObject> _obj, string _instance);

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

