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


/*------------------------------CThreadStopper-----------------------------*\
|   This is basic thread object.                                            |
|                                                                           |
|                                                                           |
|   Changelog :                                                             |
|               11/09/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/



#ifndef __CTHREADSTOPPER_H__
#define __CTHREADSTOPPER_H__



#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>

#include <iostream>

#include "cworker.h"

using namespace std;
using namespace boost;

namespace Gnoll {

	namespace Core {

		/**
		 *	This is a ressource template.</br>
		 *	This hold the ressource itself and some information
		 */
		class CThreadStopper : public CThread
		{
			private:

				/**
				 * Pointer to the worker to terminate
				 */
				shared_ptr<CWorker> m_worker;

			public:

				/**
				 * What is going to be executed by this thread
				 */
				virtual void run ();


				/**
				 * This is a constructor
				 */
				CThreadStopper(shared_ptr<CWorker> _worker); 
	
				/**
				 * This is a destructor
				 */
				virtual ~CThreadStopper();


		};

	}
}

#endif // __CTTHREADSTOPPER_H__

