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


/*---------------------------------ctimermodule.h--------------------------*\
|   The timer, to handle all time functionnalities                          |
|                                                                           |
|   Changelog :                                                             |
|               05/12/2007 - Vince - Initial release                        |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include <map>
#include <OgrePlatformManager.h>
#include <OgreTimer.h>
#include <boost/shared_ptr.hpp>
#include "cmodule.h"
#include "cmessagelistener.h"
#include "cogretimer.h"
#include "../../core/include/singleton.h"
#include "../../core/include/cgenericmessagemanager.h"


#ifndef __CTIMER_H__
#define __CTIMER_H__



using namespace std;
using namespace boost;


class CTimerModule : public CModule, public Singleton<CTimerModule>
{
    public:
        /**
         *   Default constructor
         */
        CTimerModule();


        /**
         *   Virtual destructor
         */
        virtual ~CTimerModule();


        /**
         *   Functions used to handle modules
         */
        virtual void init();
        virtual void process();
        virtual void exit();


        /**
         *   Add an unique timeout @ delay msecs after timer start/reset, sending
         *   msg when triggered
         */
        void addTimeout(unsigned long int delay, shared_ptr<CMessage> msg);


        /**
         *   Delete an unique timeout
         */
        void delTimeout(unsigned long int delay, shared_ptr<CMessage> msg);


        /**
         *   Add a periodic timeout @ delay msecs after timer start/reset, sending
         *   msg when triggered, every period msecs
         */
        void addPeriodicTimeout(unsigned long int delay, shared_ptr<CMessage> msg, unsigned long int period);


        /**
         *   Delete a periodic timeout
         */
        void delPeriodicTimeout(unsigned long int delay, shared_ptr<CMessage> msg, unsigned long int period);


        /**
         *   Interface between ITimer functions and public access
         */
        unsigned long int getMsecs(void);
        void resetTimer(bool resTimeouts=false);


    private:
        typedef map<unsigned long int, shared_ptr<CMessage> >::iterator msgMapIter;
        typedef map<unsigned long int, pair<unsigned long int, shared_ptr<CMessage> > >::iterator perMsgMapIter;

        shared_ptr<ITimer> mTimer;
        multimap<unsigned long int, shared_ptr<CMessage> > timers;
        multimap<unsigned long int, pair<unsigned long int, shared_ptr<CMessage> > > timersPeriodic;
};

#endif // __CTIMERMODULE_H__
