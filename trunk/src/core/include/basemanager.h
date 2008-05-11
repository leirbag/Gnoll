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
|               08/13/2007 - Paf - Update comments, implement method save() |
|               09/25/2007 - Paf - Replace namespace Viracocha by Gnoll     |
|               10/17/2007 - Paf - Add a new method isInstanceInCache()     |
|               11/19/2007 - Paf - Add a mutex                              |
|               12/15/2007 - Paf - Replace boost::mutex by                  |
|                                    boost::recursive_mutex                 |
|                                  Add method                               |
|                                    saveObj(string _instanceName, T _obj)  |
|                                  Add a policy for objects not found       |
|                                    when saving                            |
|               01/09/2008 - Soax - Add a parameter _instance to loadImpl   |
|                                    and saveImpl	                         |
|               02/14/2008 - Bruno Mahe - Take in account a source priority |
|                                    when looking for a suitable source for |
|                                    loading or saving a resource           |
|               02/15/2008 - Bruno Mahe - ISource objects provide a new     |
|                                    method to check if a source can        |
|                                    find a writable stream                 |
|                                                                           |
\*-------------------------------------------------------------------------*/



#include <boost/shared_ptr.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <map>
#include <set>
#include <vector>
#include <time.h>
#include <algorithm>
#include <iostream>
#include "isource.h"


#ifndef __BASEMANAGER_H__
#define __BASEMANAGER_H__

using namespace std;
using namespace boost;



namespace Gnoll {

	namespace Core {


		/**
		 * Policy for BaseManager when loading a non-existing ressource </br>
		 * -> Throwing an error
		 */
		template <class T> class ObjectNotFoundError
		{
			public:
				shared_ptr<T> objectNotFoundIssue( string _instance )
				{
					return shared_ptr<T>();
				}
		};

		
		/**
		 * Policy for BaseManager when loading a non-existing ressource </br>
		 * -> Creating a new ressource
		 * Pre-condition : T must have a constructor with instance name as parameter
		 */
		template <class T> class ObjectNotFoundNewObject
		{
			public:
				shared_ptr<T> objectNotFoundIssue( string _instance)
				{
					return shared_ptr<T>( new T(_instance) );
				}
		};



		/**
		 *	This is a ressource template.</br>
		 *	This hold the ressource itself and some information
		 */
		template <class U> class Ressource
		{
			private:

				/**
				 * Name of the instance
				 */
				string m_instance;

				/**
				 * The ressource held
				 */
				shared_ptr<U> m_obj;

				/**
				 * Last time this ressource has been accessed
				 */
				time_t m_lastTime;


			public:

				/**
				 * This is a constructor
				 * @param _instance Instance name
				 * @param _obj The object to be held
				 */
				Ressource(string _instance = "default", shared_ptr<U> _obj = shared_ptr<U>()) :
					m_instance (_instance),
					m_obj (_obj),
					m_lastTime (time(NULL)) 
				{
				}
			
				/**
				 * This is a destructor
				 */
				~Ressource() {};


				/**
				 * < operator
				 * @param b Other Ressource to compare to
				 */
				bool operator< (const Ressource<U> &b) const
				{
					return m_lastTime < b.m_lastTime;
				}

				/**
				 *	Update information regarding the ressource held
				 */
				void update() 
				{

					// Update when it has been accessed 
					m_lastTime = time(NULL);
				}

				/**
				 *	Get the ressource held
				 *	@return Smart pointer to the ressource held
				 */
				shared_ptr<U> getObject()
				{
					return m_obj;
				}

				/**
				 * Get The instance name og the ressource held
				 * @return The instance name
				 */
				string getInstance()
				{
					return m_instance;
				}

		};


		/**
		 * This a base manager that can manage any kind of objects.
		 */
		template <class T, class ObjectNotFoundPolicy = ObjectNotFoundError<T> > class BaseManager : public ObjectNotFoundPolicy
		{

			private:
				typedef typename vector<Ressource<T> >::iterator RessourceVectorIterator;
				typedef typename map<string, Ressource<T> >::iterator CacheIterator;

				/**
				* Set of loading sources 
				*/
				set< shared_ptr<ISource> >	m_loadSources;

				/**
				* Set of saving sources 
				*/
				set< shared_ptr<ISource> >	m_saveSources;

				/**
				* This is the cache that associates a Ressource to its instance name
				*/
				map<string, Ressource<T> > m_cache;

				/**
				* This is a sorted vector used as a Last Recently Used Table
				*/
				vector< Ressource<T> > m_LRUTable;

				/**
				* This is a map associating an instance name to the ressource iterator from m_LRUTable
				*/
				map<string, RessourceVectorIterator> m_LRUIndex;

				/**
				* Maximum number of elements the cache can hold
				*/
				static const unsigned int m_maxCache = 250;

				/**
				 * Mutex
				 */
				boost::recursive_mutex m_mutex;

		
			protected:

				/**
				 * This is a virtual pure method that is going to be implemented by classes that inherits from this class.
				 * This method is in charge of creating an actual ressource from a stream
				 *
				 * @param _stream Stream from which the ressource will be extracted
				 * @return Smart pointer to the newly created object
				 */
				virtual shared_ptr<T> loadImpl( shared_ptr<IStream> _stream, string _instance) = 0;

				/**
				 * This is a virtual pure method that is going to be implemented by classes that inherits from this class.
				 * This method is in charge of saving an actual ressource to a stream
				 *
				 * @param _stream Stream to which the ressource will be saved
				 * @param _obj Smart pointer to the object to be saved
				 * @return The successfulness of this operation
				 */
				virtual bool saveImpl( shared_ptr<IStream> _stream, shared_ptr<T> _obj, string _instance) = 0;

				/**
				 * This method tries to find a suitable source to load an instance
				 * @param _instance Instance name
				 * @return A smart pointer to a source able to load the instance passed as a parameter
				 */
				shared_ptr<ISource> findLoadSource( string _instance)
				{
					boost::recursive_mutex::scoped_lock lock(m_mutex);

					shared_ptr<ISource> bestSource;

					set< shared_ptr<ISource> >::iterator iter = m_loadSources.begin();
					while (iter != m_loadSources.end())
					{
						shared_ptr<ISource> temp = *iter;

						/**
						 * Can the resource be loaded by this source ?
						 */
						if (temp->isFetchable(_instance))
						{
							/**
							 * Have we already found a potential source ?
							 */
							if (bestSource.get() == 0)
							{
								bestSource = temp;
							} else
							{
								/**
								 * We have two potential source.
								 * So we compare their priorities
								 */
								if ( (*bestSource) < temp)
								{
									bestSource = temp;
								}
							}
						}

						iter++;
					}

					return bestSource;
				}

				/**
				 * This method tries to find a suitable source to save an instance
				 * @param _instance Instance name
				 * @return A smart pointer to a source able to save the instance passed as a parameter
				 */
				shared_ptr<ISource> findSaveSource( string _instance)
				{
	
					boost::recursive_mutex::scoped_lock lock(m_mutex);

					shared_ptr<ISource> bestSource;

					set< shared_ptr<ISource> >::iterator iter = m_saveSources.begin();
					while (iter != m_saveSources.end())
					{
						shared_ptr<ISource> temp = *iter;

						/**
						 * Can the resource be saved by this source ?
						 */
						if (temp->isWritable(_instance))
						{
							/**
							 * Have we already found a potential source ?
							 */
							if (bestSource.get() == 0)
							{
								bestSource = temp;
							} else
							{
								/**
								 * We have two potential source.
								 * So we compare their priorities
								 */
								if ( (*bestSource) < temp)
								{
									bestSource = temp;
								}
							}
						}

						iter++;
					}

					return bestSource;
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
   
				/**
				 * Add a Source to the pool of available loading sources
				 * @param _source Source to add
				 */ 
				void addLoadSource(shared_ptr<ISource> _source)
				{
					boost::recursive_mutex::scoped_lock lock(m_mutex);
					m_loadSources.insert(_source);
				}

				/**
				 * Add a Source to the pool of available saving sources
				 * @param _source Source to add
				 */ 
				void addSaveSource(shared_ptr<ISource> _source)
				{
					boost::recursive_mutex::scoped_lock lock(m_mutex);
					m_saveSources.insert(_source);
				}

				
				/**
				 * Check if an instance has already been load and is cached
				 * @param _instance Instance name
				 * @return Return true if the instance is cached. False otherwise
				 */
				bool isInstanceInCache(string _instance)
				{
					boost::recursive_mutex::scoped_lock lock(m_mutex);
					CacheIterator iter = m_cache.find(_instance);

					// There is no such instance in the cache
					if (iter == m_cache.end())
					{
						return false;
					} else {
						return true;
					}
				}

				/**
				 * This method loads an object from its instance name
				 * @param _instance Instance name
				 * @return Smart pointer to the object
				 */
				shared_ptr<T> load(string _instance)
				{
					boost::recursive_mutex::scoped_lock lock(m_mutex);

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
							return this->objectNotFoundIssue(_instance);
						}

						// First we need a stream
						shared_ptr<IStream> stream = source->loadStream(_instance);

						// Then we load the object from the stream
						// This method has to be implemented by any class that inherits from BaseManager
						// (that's why there is 'Base' in the name)
						shared_ptr<T> object = this->loadImpl(stream, _instance);
			
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

						return ressource.getObject();
					} else { 
						// An instance has been found in the cache
		
						// 1./ We get the Ressource associated to this instance
						Ressource<T> res = iter->second;

						// 2./ We update its stats (last time it has been used...).
						// So the cache can guess what objects are the more/less used
						res.update();
	
						// 3./ We return the instance.
						return res.getObject();

					}

				}

				/**
				 * This method saves an object from its instance name
				 * @param _instance Instance name
				 * @return The successfulness of this operation
				 */
				bool saveObj(string _instance, shared_ptr<T> _obj)
				{
					boost::recursive_mutex::scoped_lock lock(m_mutex);

					// We need to find a suitable source to load the instance
					shared_ptr<ISource> source = findSaveSource(_instance);

					// If no source is able to retrieve this instance
					// NULL is returned
					if (source.get() == NULL)
					{
						return false;
					}

					// We need a stream to save this object
					shared_ptr<IStream> stream = source->saveStream(_instance);

					// Then we save the object to the stream
					// This method has to be implemented by any class that inherits from BaseManager
					// (that's why there is 'Base' in the name)
					this->saveImpl(stream, _obj, _instance);

					stream->close();


					// Everything is a success.
					return true;

				}



				/**
				 * This method saves an object from its instance name
				 * @param _instance Instance name
				 * @return The successfulness of this operation
				 */
				bool save(string _instance, shared_ptr<T> _obj = shared_ptr<T>())
				{
					boost::recursive_mutex::scoped_lock lock(m_mutex);

					// First we check if this instance exists in the cache
					CacheIterator iter = m_cache.find(_instance);

					// There is no such instance in the cache
					if (iter == m_cache.end())
					{
						if (_obj.get() == 0)
						{
							return false;
						} else 
						{
							return saveObj(_instance, _obj);
						}

					} else { 
						// An instance has been found in the cache
		
						// 1./ We get the Ressource associated to this instance
						Ressource<T> res = iter->second;

						// 2./ We update its stats (last time it has been used...).
						// So the cache can guess what objects are the more/less used
						res.update();

						// 3./ We need to find a suitable source to load the instance
						shared_ptr<ISource> source = findSaveSource(_instance);

						// If no source is able to retrieve this instance
						// NULL is returned
						if (source.get() == NULL)
						{
							return false;
						}

						// We need a stream to save this object
						shared_ptr<IStream> stream = source->saveStream(_instance);

						// Then we save the object to the stream
						// This method has to be implemented by any class that inherits from BaseManager
						// (that's why there is 'Base' in the name)
						this->saveImpl(stream, res.getObject(), _instance);

						stream->close();


						// 3./ Everything is a success.
						return true;

					}


				}

				/**
				* Erase an object from the cache
				* @param _instance Object to erase from the cache
				*/
				void release (string _instance)
				{
					boost::recursive_mutex::scoped_lock lock(m_mutex);

					RessourceVectorIterator it = m_LRUIndex[_instance];

					m_LRUIndex.erase(_instance);
					m_LRUTable.erase(it);
					m_cache.erase(_instance);
				}
		};
	}
}

#endif // __BASEMANAGER_H__

