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


/*------------------------------singleton.h--------------------------------*\
|   A singleton template                                                    |
|                                                                           |
|   Changelog :                                                             |
|               06/23/2006 - Paf - Initial release                          |
|               07/13/2007 - Yellow - 'Singleton' ambiguous symbol, change  |
|                                       to '::Singleton'                    |
|               11/27/2007 - Paf - Singleton is now part of the namespace   |
|                                    Gnoll::Core                            |
|                                  Add a mutex to make it thread friendly   |
|                                                                           |
\*-------------------------------------------------------------------------*/



#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

#include <iostream>

#include "../../config.h"

namespace Gnoll
{

	namespace Core
	{


		/**
		 *	Interface of all game modules. 
		 */ 
		template <typename T> class Singleton
		{
			private:

				/**
				 * Instance pointer
				 */
				static T* m_instance;
		
				/**
				 * Mutex
				 */
				static boost::mutex m_mutex;

	

			public:

				/**
				 * A constructor
				 */
				Singleton() {}

				/**
				 * A virtual destructor
				 */
				virtual ~Singleton() {};

				/**
				 * This returns a pointer to the singleton
				 */
				static T* getInstancePtr() 
				{ 
					boost::mutex::scoped_lock lock(m_mutex);

					if (m_instance == 0)
						m_instance = new T;

					return m_instance;
				}

				/**
				 * This destroys the singleton
				 */
				static void destroy()
				{
#if DEBUG
					std::cout << "Trying to destroy one singleton" << std::endl;
#endif

					boost::mutex::scoped_lock lock(m_mutex);

					if (m_instance != 0)
					{
#if DEBUG
					std::cout << "Deleting one singleton" << std::endl;
#endif
						delete m_instance;
						m_instance = 0;
					}			
		
				}

		};

		template <typename T> T * Singleton<T>::m_instance = 0;
		template <typename T> boost::mutex Singleton<T>::m_mutex;

	}
}


#endif // __SINGLETON_H__
