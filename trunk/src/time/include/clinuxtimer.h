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


/*---------------------------clinuxtimer.h---------------------------------*\
|   A class to encapsulate a timer based on GNU/Linux standard libraries    |
|                                                                           |
|   Changelog :                                                             |
|               09/07/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/



#ifndef __CLINUXTIMER_H__
#define __CLINUXTIMER_H__


#include "itimer.h"
#include <sys/time.h>


using namespace std;


class CLinuxTimer : public ITimer
{
	
	private:

		/**
		 * Initial time </br>
		 * This will be used as the reference time to compare to
		 */
		struct timeval m_initialTime;




	public:

		/**
		 *   A constructor
		 */
		CLinuxTimer(void);


		/**
		 *   A destructor
		 */
		~CLinuxTimer(void);


		/**
		 *   Returns elapsed milliseconds since timer start/reset
		 *   @return milliseconds elapsed
		 */
		unsigned long int getMsecs(void);


		/**
		 *   Reset the timer
		 */
		void reset(void);

};

#endif // __CLINUXTIMER_H__
