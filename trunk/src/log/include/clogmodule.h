/***************************************************************************
 *   Copyright (C) 2008 by Bruno Mahe                                      *
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


/*-----------------------------CLogModule----------------------------------*\
|   This module provides some logging facilities                            |
|                                                                           |
|   Changelog :                                                             |
|               06/12/2008 - Bruno Mahe - Initial release                   |
|                                                                           |
\*-------------------------------------------------------------------------*/



#ifndef __CLOGMODULE_H__
#define __CLOGMODULE_H__



#include "../../core/include/cmodule.h"
#include "../../core/include/singleton.h"


using namespace Gnoll::Core;
using namespace std;
using namespace boost;

namespace Gnoll
{
	namespace Log
	{

		class CLogModule : public CModule, public Gnoll::Core::Singleton<CLogModule>
		{
			public:

				/**
				 *   Default constructor
				 */
				CLogModule();


				/**
				 *   Virtual destructor
				 */
				virtual ~CLogModule();


				/**
				 * Init the module and its subsystems
				 */
				virtual void init();

				/**
				 * Handle everything related to logging.</br>
				 */
				virtual void process();

				/**
				 * Shutdown the module and its subsystems
				 */
				virtual void exit();


				/**
				 * Define the log filename
				 */
				void setLogFileName(string logFileName);


			private:

				/**
				 * Log file name
				 */
				string m_logFileName;

		};
	}
}

#endif // __CLOGMODULE_H__
