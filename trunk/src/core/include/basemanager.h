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



#include <boost/shared_ptr.hpp>
#include <map>
#include <set>
#include <vector>
#include <time.h>
#include <algorithm>

#include "isource.h"
#include "iattribute.h"


#ifndef __BASEMANAGER_H__
#define __BASEMANAGER_H__

using namespace std;
using namespace boost;



namespace Viracocha {

	namespace Core {

		template <class U> class Ressource
		{
			private:
				string m_instance;
				shared_ptr<U> m_obj;
				time_t m_lastTime;

			public:

				Ressource(string _instance = "default", shared_ptr<U> _obj = shared_ptr<U>()) :
					m_instance (_instance),
					m_obj (_obj),
					m_lastTime (time(NULL)) 
				{
				}

				~Ressource() {};

				bool operator< (const Ressource<U> &b) const
				{
					return m_lastTime < b.m_lastTime;
				}

				void update() 
				{
					m_lastTime = time(NULL);
				}

				shared_ptr<U> getObject()
				{
					return m_obj;
				}

				string getInstance()
				{
					return m_instance;
				}

		};



		template <class T> class BaseManager
		{

			private:
			typedef typename vector<Ressource<T> >::iterator RessourceVectorIterator;
			typedef typename map<string, Ressource<T> >::iterator CacheIterator;


			set< shared_ptr<ISource> >	m_loadSources;
			set< shared_ptr<ISource> >	m_saveSources;
			map<string, Ressource<T> > m_cache;

			vector< Ressource<T> > m_LRUTable;
			map<string, RessourceVectorIterator> m_LRUIndex;

			unsigned int m_maxCache;



		
			protected:

				virtual shared_ptr<T> loadImpl( shared_ptr<IStream> _stream) = 0;

				virtual bool saveImpl( shared_ptr<IStream> _stream, shared_ptr<T> _obj) = 0;


				shared_ptr<ISource> findLoadSource( string _instance)
				{

					set< shared_ptr<ISource> >::iterator iter = m_loadSources.begin();
					while (iter != m_loadSources.end())
					{
						shared_ptr<ISource> temp = *iter;

						if (temp->isFetchable(_instance))
						{
							return temp;
						}

						iter++;
					}

					return shared_ptr<ISource>();
				}

				shared_ptr<ISource> findSaveSource( string _instance)
				{
	
					set< shared_ptr<ISource> >::iterator iter = m_saveSources.begin();
					while (iter != m_saveSources.end())
					{
						shared_ptr<ISource> temp = *iter;

						if (temp->isFetchable(_instance))
						{
							return temp;
						}

						iter++;
					}

					return shared_ptr<ISource>();
				}



			public:

				/**
				 * This is a constructor.
				 */
				BaseManager()
				{
				};


				/**
				 * This is a destructor.
				 */
				virtual ~BaseManager()
				{
				};
   

				void addLoadSource(shared_ptr<ISource> _source)
				{
					m_loadSources.insert(_source);
				}

				void addSaveSource(shared_ptr<ISource> _source)
				{
					m_saveSources.insert(_source);
				}


				shared_ptr<T> load(string _instance)
				{
					// First we check if this instance exists in the cache
					CacheIterator iter = m_cache.find(_instance);

					// There is no such instance in the cache
					if (iter == m_cache.end())
					{
						// We are going to load it and put it in the cache.
						// We might have also to make some room in the cache.


						// First we need to find a suitable source to load the instance
						shared_ptr<ISource> source = findLoadSource(_instance);

						// If no source is able to retrieve this instance
						// NULL is returned
						if (source.get() == NULL)
						{
							return shared_ptr<T>();
						}

						// First we need a stream
						shared_ptr<IStream> stream = source->load(_instance);

						// Then we load we object from the stream
						// This method has to be implemented by any class that inherits from BaseManager
						// (that's why there is 'Base' in the name)
						shared_ptr<T> object = this->loadImpl(stream);
			
						stream->close();

						// A Ressource object is created from the object
						Ressource<T> ressource(_instance, object);

						// Now we check if the cache hasn't reach its maximum number of objects
						if (m_cache.size() > m_maxCache)
						{
				
							Ressource<T> res = m_LRUTable.back();
							string instanceToDelete = res.getInstance();
		
							m_LRUIndex.erase(instanceToDelete);
							m_LRUTable.pop_back();

						} 

						// We can safely add the object to the cache and return the object

						// The resource is added to the cache
						m_cache[_instance] = ressource;

						// We also need to add the ressource to the LRU vector in order to keep track of the last recent
						// used object.
						// We also use a map _instance -> object in order to be able to delete it quickly
						RessourceVectorIterator it = m_LRUTable.insert(m_LRUTable.end(), ressource);
						std::sort(m_LRUTable.begin(), m_LRUTable.end());
						m_LRUIndex[_instance] = it;

						return dynamic_pointer_cast<T>(ressource.getObject());
					} else { 
						// An instance has been found in the cache
		
						// 1./ We get the Ressource associated to this instance
						Ressource<T> res = iter->second;

						// 2./ We update its stats (last time it has been used...).
						// So the cache can guess what objects are the more/less used
						res.update();
	
						// 3./ We return the instance.
						return dynamic_pointer_cast<T>(res.getObject());

					}

				}

			bool save(string _instance);



			void release (string _instance)
			{
				RessourceVectorIterator it = m_LRUIndex[_instance];

				m_LRUIndex.erase(_instance);
				m_LRUTable.erase(it);
				m_cache.erase(_instance);
			}



		};

	}
}

#endif // __BASEMANAGER_H__

