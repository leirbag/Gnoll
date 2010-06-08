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

#ifndef __POOLTHREADS_H__
#define __POOLTHREADS_H__

#include <utility>

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>

#include "thread.h"
#include "multithreadqueue.h"
#include "worker.h"

using namespace std;
using namespace boost;

namespace Gnoll
{
	namespace Core
	{
		/**
		 * Definition of a job : a simple thread
		 */
		typedef Thread Job;

		/**
		 *	This is a multi-thread friendly queue.</br>
		 */
		class PoolThread
		{
			public:
				/**
				 * Default maximum number of threads running in the pool
				 */
				static const unsigned int MAX_THREADS = 3;

				/**
				 *	Default time out for threads waiting on a condition
				 */
				static const unsigned int TIMED_WAIT = 5;

				/**
				 * Constructor
				 */
				PoolThread();

				/**
				 * Destructor
				 */
				virtual ~PoolThread();

				/**
				 * Set the maximum number of threads allowed by the pool
				 * @param _maxThreads Maximum number of threads
				 */
				void setMaxThreads(unsigned int maxThreads = MAX_THREADS);

				/**
				 * Return Maximum number of threads allowed by the pool
				 * @return Maximum number of threads
				 */
				unsigned int getMaxThreads();

				/**
				 * Terminates all threads in the pool
				 */
				void clearPool();

				/**
				 * Return the current number of threads running in the pool
				 * @return Number of threads running
				 */
				unsigned int getNumRunningThreads();

				/**
				 * Push a job to the end of the queue of jobs to be run
				 * @param _job Job to be run
				 */
				void pushJob(shared_ptr<Job> job);

				/**
				 *	Get the first job in the queue.</br>
				 *	If the job queue is empty, it will make the thread wait for PoolThread::TIMED_WAIT
				 *
				 *	@return A shared pointer on a Job if there was any in the queue. Shared pointer on
				 *		 	nothing otherwise
				 */
				shared_ptr<Job> popJob();

				/**
				 * Check if the job queue is empty
				 * @return True if the queue is empty. False otherwise
				 */
				bool emptyQueue();

			private:
				/**
				 * Add or remove running threads depending on the maximum number of thread allowed
				 */
				void updateWorkers();

			private:
				/**
				 * Maximum number of threads running in the pool
				 */
				unsigned int m_maxThreads;

				/**
				 * List of jobs to be run
				 */
				MultiThreadQueue< shared_ptr<Job> > m_listJobs;

				/**
				 * A condition to notify threads when a new job is queued
				 */
				boost::condition m_jobAdded;

				/**
				 * Mutex to make this class thread-safe for addition or removal of a job
				 */
				boost::mutex m_mutex;

				/**
				 * Mutex to make this class thread-safe when using accessors/mutators
				 */
				boost::mutex m_mutatorMutex;

				/**
				 * Mutex to make this class thread-safe when calling update()
				 */
				boost::mutex m_updateMutex;

				/**
				 * List of threads running
				 */
				list< pair<shared_ptr<thread>, shared_ptr<Worker> > > m_listThreads;
		};
	}
}

#endif 
