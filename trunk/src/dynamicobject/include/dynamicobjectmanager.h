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


/*-------------------------DynamicObjectManager-------------------------*\
|   This is a singleton object managing DynamicObjects                   |
|                                                                           |
|                                                                           |
|   Changelog :                                                             |
|               07/20/2007 - Paf - Initial release                          |
|               08/16/2007 - Paf - Update comments                          |
|               09/25/2007 - Paf - Replace namespace Viracocha by Gnoll     |
|               12/15/2007 - Paf - Add BaseManager policy                   |
|                                                                           |
\*-------------------------------------------------------------------------*/



#ifndef __DYNAMICOBJECTMANAGER_H__
#define __DYNAMICOBJECTMANAGER_H__

#include "../../core/include/basemanager.h"
#include "../../core/include/singleton.h"
#include "dynamicobject.h"


using namespace std;
using namespace boost;



namespace Gnoll {

	namespace DynamicObject {

		/**
		 * DynamicObjectManager is a singleton managing DynamicObject instances.</br>
		 * It can load/save from any Source and provide a cache to improve performance (using LRU)
		 */
		class DynamicObjectManager : public BaseManager<DynamicObject, ObjectNotFoundNewObject<DynamicObject> >, public Singleton<DynamicObjectManager>
		{

			private:

				/**
				 * This method loads a DynamicObject from a stream
				 * @param _stream Stream from which the ressource will be extracted
				 * @return Smart pointer to the newly created object
				 */
				virtual shared_ptr<DynamicObject> loadImpl( shared_ptr<IStream> _stream, string _instance);

				/**
				 * This method saves a DynamicObject to a stream
				 * @param _stream Stream to which the ressource will be saved
				 * @param _obj Smart pointer to the object to be saved
				 * @return The successfulness of this operation
				 */
				virtual bool saveImpl( shared_ptr<IStream> _stream, shared_ptr<DynamicObject> _obj, string _instance);

			public:

				/**
				 * This is a constructor.
				 */
				DynamicObjectManager();


				/**
				 * This is a destructor.
				 */
				virtual ~DynamicObjectManager();

		};

	}
}

#endif // __DYNAMICOBJECTMANAGER_H__

