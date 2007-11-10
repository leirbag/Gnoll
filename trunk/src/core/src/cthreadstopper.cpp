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



#include "../include/cthreadstopper.h"



using namespace std;
using namespace boost;

namespace Gnoll {

	namespace Core {


		void CThreadStopper::run ()
		{
			m_worker->stop();
		}


		CThreadStopper::CThreadStopper(shared_ptr<CWorker> _worker) : m_worker(_worker)
		{
		}
	
		CThreadStopper::~CThreadStopper()
		{
		}


	}
}


