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

#ifndef __THREAD_H__
#define __THREAD_H__

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

namespace Gnoll
{
	namespace Core
	{
		/**
		 *	This is a ressource template.</br>
		 *	This hold the ressource itself and some information
		 */
		class Thread
		{
			public:
				/**
				 * This is a constructor
				 */
				Thread() {}

				/**
				 * This is a copy constructor
				 */
				Thread(const Thread& thread) {}

				/**
				 * This is a destructor
				 */
				virtual ~Thread() {};

				/**
				 * Functor that will run the code
				 */
				void operator()()
				{
					boost::mutex::scoped_lock scoped_lock(m_threadMutex);
					this->run();
				}

				/**
				 * What is going to be executed by this thread
				 */
				virtual void run() = 0;

			private:
				/**
				 * There can be only one instance of a thread running a the same time
				 */
				boost::mutex m_threadMutex;
		};
	}
}

#endif 
