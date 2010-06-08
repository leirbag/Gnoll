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

#include "../include/worker.h"

#include <sstream>

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

#include "../include/thread.h"
#include "../include/poolthread.h"
#include "../../log/include/clogmodule.h"

using namespace std;

namespace Gnoll
{
	namespace Core
	{
		/**
		 * What is going to be executed by this thread
		 */
		void Worker::run()
		{
			if(m_poolOfThreads == NULL)
			{
				Gnoll::Log::CLogModule::getInstancePtr()->logMessage("  No Poolthread sent. Exiting.");
				return;
			}

			std::ostringstream tmpString;
			while(m_stop == false)
			{
				tmpString << this << " I don't have to stop [" << m_stop << "]. Popping a job...";
				Gnoll::Log::CLogModule::getInstancePtr()->logMessage( tmpString.str() );
				tmpString.clear();

				shared_ptr<Job> job = m_poolOfThreads->popJob();

				Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "  Job popped" );
				if (job.get())
				{
					tmpString << this << " It's a real job ! It's not empty";
					Gnoll::Log::CLogModule::getInstancePtr()->logMessage(  tmpString.str() );
					tmpString.clear();

					job->run();

					tmpString << this << " Job run";
					Gnoll::Log::CLogModule::getInstancePtr()->logMessage( tmpString.str() );
					tmpString.clear();
				}
			}
			tmpString << this << " I have to stop. Byebye..." << endl;
			Gnoll::Log::CLogModule::getInstancePtr()->logMessage(tmpString.str());
			tmpString.clear();
		}

		/**
		 * This is a constructor
		 */
		Worker::Worker(PoolThread* pool) :
			m_stop(false), m_poolOfThreads(pool)
		{
		}

		/**
		 * This is a destructor
		 */
		Worker::~Worker()
		{
		}

		void Worker::stop()
		{
			m_stop = true;
			std::ostringstream tmpString;
			tmpString << this << "  m_stop changed to true";
			Gnoll::Log::CLogModule::getInstancePtr()->logMessage( tmpString.str() );
			tmpString.clear();
		}
	}
}

