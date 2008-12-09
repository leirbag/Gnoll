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


/*--------------------------------CMTQueue---------------------------------*\
|   This is basic queue.                                                    |
|                                                                           |
|                                                                           |
|   Changelog :                                                             |
|               10/30/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/



#ifndef __CMTQUEUE_H__
#define __CMTQUEUE_H__



#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <list>



using namespace std;

namespace Gnoll {

	namespace Core {

		/**
		 *	This is a multi-thread friendly queue.</br>
		 */
		template <typename T> class CMTQueue
		{
			private:

				/**
				 * A mutex used to synchronize all operations on the queue
				 */
				boost::mutex m_queueMutex;

				/**
				 * The actual list
				 */
				list<T> m_queue;


			public:
			
				/**
				 * A constructor
				 */
				CMTQueue() {}


				/**
				 * A destructor
				 */
				virtual ~CMTQueue() {}


				/**
				 * Push an element to the queue
				 * @param _elem Element to be pushed
				 */
				void push(T& _elem)
				{
					boost::mutex::scoped_lock scoped_lock(m_queueMutex);
					m_queue.push_back(_elem);
				}


				/**
				 * Pop an element from the queue
				 * @return The first element in the queue
				 */
				T pop()
				{
					boost::mutex::scoped_lock scoped_lock(m_queueMutex);

					T elem = m_queue.front();
					m_queue.pop_front();
					return elem;
				}


				/**
				 * Check if the queue is empty
				 * @return True if the queue is empty. False otherwise
				 */
				bool empty()
				{
					boost::mutex::scoped_lock scoped_lock(m_queueMutex);
					return m_queue.empty();
				}


				/**
				 * Get the size of the queue
				 * @return Size of the queue
				 */
				unsigned int size()
				{
					boost::mutex::scoped_lock scoped_lock(m_queueMutex);
					return m_queue.size();
				}

				/**
				 * Clear the queue
				 */
				void clear()
				{
					boost::mutex::scoped_lock scoped_lock(m_queueMutex);
					m_queue.clear();
				}
		
		};

	}
}

#endif // __CMTQUEUE_H__

