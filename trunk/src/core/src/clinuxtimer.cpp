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


/*----------------------------clinuxtimer.cpp------------------------------*\
|   A class to encapsulate a timer based on standard GNU/Linux libraries    |
|                                                                           |
|   Changelog :                                                             |
|               09/07/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../include/clinuxtimer.h"
#include <sys/time.h>
#include <stddef.h>


CLinuxTimer::CLinuxTimer(void)
{
	this->reset();
}


CLinuxTimer::~CLinuxTimer(void)
{
}


unsigned long int CLinuxTimer::getMsecs(void)
{
	struct timeval now;
	gettimeofday( &now, NULL);

	return (now.tv_sec - m_initialTime.tv_sec) * 1000 + (now.tv_usec - m_initialTime.tv_usec) / 1000;

}


void CLinuxTimer::reset(void)
{
	gettimeofday( &m_initialTime, NULL);
}
