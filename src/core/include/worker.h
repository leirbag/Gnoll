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

#ifndef __WORKER_H__
#define __WORKER_H__

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

#include "thread.h"

namespace Gnoll
{
	namespace Core
	{
		/**
		 * Forward declaration
		 */
		class PoolThread;

		/**
		 *	This is a ressource template.</br>
		 *	This hold the ressource itself and some information
		 */
		class Worker : public Thread
		{
			public:
				/**
				 * This is a constructor
				 * @param _pool Pool containing the worker
				 */
				Worker(PoolThread* pool);

				/**
				 * This is a destructor
				 */
				virtual ~Worker();

				/**
				 * Tells the worker to exit as soon as it has finished its current job
				 */
				void stop();

				/**
				 * What is going to be executed by this thread
				 */
				virtual void run();

			private:

				/**
				 * Stop flag
				 */
				bool m_stop;

				/**
				 * Pointer on the pool this worker belongs to
				 */
				PoolThread* m_poolOfThreads;
		};
	}
}

#endif
