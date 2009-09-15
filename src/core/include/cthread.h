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


/*---------------------------------CThread---------------------------------*\
|   This is basic thread object.                                            |
|                                                                           |
|                                                                           |
|   Changelog :                                                             |
|               10/30/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/



#ifndef __CTHREAD_H__
#define __CTHREAD_H__



#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>



namespace Gnoll {

	namespace Core {

		/**
		 *	This is a ressource template.</br>
		 *	This hold the ressource itself and some information
		 */
		class CThread
		{
			private:

				/**
				 * There can be only one instance of a thread running a the same time
				 */
				boost::mutex m_threadMutex;


			public:

				/**
				 * What is going to be executed by this thread
				 */
				virtual void run () = 0;


				/**
				 * This is a constructor
				 */
				CThread()
				{
				}
	
				/**
				 * This is a copy constructor
				 */
				CThread(const CThread& _thread)
				{
				}
		
				/**
				 * This is a destructor
				 */
				virtual ~CThread() {};


				/**
				 * Functor that will run the code
				 */
				void operator()()
				{
					boost::mutex::scoped_lock scoped_lock(m_threadMutex);
					this->run();
				}

		};

	}
}

#endif // __CTHREAD_H__

