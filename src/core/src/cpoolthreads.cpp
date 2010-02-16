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


/*------------------------------CPoolThreads-------------------------------*\
|   This is basic pool of threads executing jobs.                           |
|                                                                           |
|                                                                           |
|   Changelog :                                                             |
|               10/30/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/




#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/function/function0.hpp>
#include <boost/thread/xtime.hpp>
#include <utility>


#include "../include/cthread.h"
#include "../include/cthreadstopper.h"
#include "../include/cmtqueue.h"
#include "../include/cpoolthreads.h"


using namespace std;
using namespace boost;


namespace Gnoll {

	namespace Core {


		void CPoolThreads::updateWorkers() 
		{
			/**
			 * This method has a lock since we don't want to mess up
			 * with multiple updates/clearpool happening at the same time
			 */
			 boost::mutex::scoped_lock lock(m_updateMutex);


			/**
			 * Too many threads are running.
			 * Need to stop some
			 */
			while ( this->getMaxThreads() < this->getNumRunningThreads() )
			{
				
				/**
				 * The way terminate a thread is simple :
				 *  - Pick any running thread
				 *  - Create a job which will set the stop flag of a worker to true
				 *  - Join on the thread hosting this worker
				 */


				/**
				 * First step : get the first thread on the list
				 */ 
				pair< shared_ptr<boost::thread> , shared_ptr<CWorker> > pair = m_listThreads.front();
				m_listThreads.pop_front();

				shared_ptr<boost::thread> thread = pair.first;
				shared_ptr<CWorker> worker = pair.second;

				
				/**
				 * Second step : create a job to set the stop flag of the worker associated to the thread
				 *  we want to stop
				 */
				shared_ptr<CJob> stopWorker( new CThreadStopper(worker));

				this->pushJob(stopWorker);


				/**
				 * Third step : join on the future terminated thread
				 */
				thread->join();
				
			}
				


			/**
			 * Too few threads are running
			 * Need to create some
			 */
			while ( this->getMaxThreads() > this->getNumRunningThreads() )
			{

				shared_ptr<CWorker> worker ( new CWorker(this) );

				/**
				 * This gets a reference on the instance () operator directly.
				 */
				boost::function0<void> functor;
				functor = boost::ref(*worker);


				/**
				 * If *worker would have been directly used, the thread would have used a
				 *  copy of *worker. So we wouldn't get any control over the worker since we
				 *  wouldn't have any reference of this one
				 */
				shared_ptr<boost::thread> thread (new boost::thread( functor ) );

				pair< shared_ptr<boost::thread>, shared_ptr<CWorker> > newThread(thread, worker);

				m_listThreads.push_back(newThread);

			}

		}

		void CPoolThreads::clearPool()
		{
			/**
			 * This method has a lock since we don't want to mess up
			 * with multiple updates/clearpool happening at the same time
			 */
			boost::mutex::scoped_lock lock(m_updateMutex);
	

			/**
			 * This goes through each thread and send a message making it to stop
			 */

			for (list< pair< shared_ptr<boost::thread> , shared_ptr<CWorker> > >::iterator it = m_listThreads.begin(); it != m_listThreads.end(); it++)	
			{
				
				pair< shared_ptr<boost::thread> , shared_ptr<CWorker> > pair = *it;

				shared_ptr<boost::thread> thread = pair.first;
				shared_ptr<CWorker> worker = pair.second;
				
				
				shared_ptr<CJob> stopWorker( new CThreadStopper(worker));

				this->pushJob(stopWorker);
				thread->join();

			}
				
		}


		CPoolThreads::CPoolThreads() : m_maxThreads(MAX_THREADS)
		{
			updateWorkers();
		}


		CPoolThreads::~CPoolThreads()
	  	{
			clearPool(); 
		}


		void CPoolThreads::setMaxThreads(unsigned int _maxThreads) 
		{
			boost::mutex::scoped_lock lock(m_mutatorMutex);

			m_maxThreads = _maxThreads;
		}


		unsigned int CPoolThreads::getMaxThreads() 
		{
			return m_maxThreads;
		}


		unsigned int CPoolThreads::getNumRunningThreads() 
		{
			return m_listThreads.size();
		}


		void CPoolThreads::pushJob( shared_ptr<CJob> _job) 
		{
			m_listJobs.push(_job); // Job added to the list
			m_jobAdded.notify_one(); // Threads are notified about it
		}


		shared_ptr<CJob> CPoolThreads::popJob() 
		{
			boost::mutex::scoped_lock lock(m_mutex);

			boost::xtime xt;
			boost::xtime_get(&xt, boost::TIME_UTC);
			xt.sec += TIMED_WAIT;


			if ( m_listJobs.empty() )
			{
				m_jobAdded.timed_wait(lock, xt);
				//m_jobAdded.wait(lock);
			} 

			shared_ptr<CJob> job;

			if (m_listJobs.empty() == false)
			{
				job = m_listJobs.pop();
			}
			
			return job;
		}

		bool CPoolThreads::emptyQueue()
		{
			return m_listJobs.empty();
		}		

	}
}


