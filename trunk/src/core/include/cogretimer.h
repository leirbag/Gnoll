/***************************************************************************
 *   Copyright (C) 2006 by Puzzle Team                                     *
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


/*---------------------------------cogretimer.h----------------------------*\
|   A class to encapsulate the Ogre Timer                                   |
|                                                                           |
|   Changelog :                                                             |
|               05/12/2007 - Vince - Initial release                        |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include <OgreTimer.h>
#include <Ogre.h>
#include <OgrePlatformManager.h>
#include <boost/shared_ptr.hpp>
#include "itimer.h"


#ifndef __COGRETIMER_H__
#define __COGRETIMER_H__


using namespace std;
using namespace boost;


class COgreTimer : public ITimer
{
    public:
        COgreTimer(void);
        ~COgreTimer(void);


        /**
         *   Returns elapsed milliseconds since timer start/reset
         *   @return milliseconds elapsed
         */
        unsigned long int getMsecs(void);


        /**
         *   Reset the timer
         */
        void reset(void);

    private:
        shared_ptr<Ogre::Timer> mTimer;
};

#endif // __COGRETIMER_H__
