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


/*---------------------------------CWorker---------------------------------*\
|   This is thread object used by CPoolThreads.                             |
|                                                                           |
|                                                                           |
|   Changelog :                                                             |
|               11/08/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/



#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

#include "../include/cthread.h"
#include "../include/cworker.h"
#include "../include/cpoolthreads.h"

#include <iostream>


namespace Gnoll {

	namespace Core {

		/**
		 * What is going to be executed by this thread
		 */
		void CWorker::run ()
		{
			if ( m_poolOfThreads == NULL )
			{
				cout << this << "  No Poolthread sent. Exiting." << endl;
				return;
			}


			while ( m_stop == false )
			{
				cout << this << " I don't have to stop [" << m_stop << "]. Popping a job..." << endl;

				shared_ptr<CJob> job = m_poolOfThreads->popJob();	

				cout << "  Job popped" << endl;
				if (job.get())
				{
					cout << this << " It's a real job ! It's not empty" << endl;
					job->run();
					cout << this << " Job run" << endl;
				}
			}
			cout << this << " I have to stop. Byebye..." << endl;
		}



		/**
		 * This is a constructor
		 */
		CWorker::CWorker (CPoolThreads* _pool): m_stop(false), m_poolOfThreads(_pool)
		{
		}
			
		/**
		 * This is a destructor
		 */
		CWorker::~CWorker()
		{
		}


		void CWorker::stop() 
		{
			m_stop = true;
			cout << this << "  m_stop changed to true" << endl;
		}


	}
}


