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

#include "../include/ctimermodule.h"

using namespace Ogre;


CTimerModule::CTimerModule()
{
}


CTimerModule::~CTimerModule()
{
}


void CTimerModule::init()
{
    mTimer = shared_ptr<ITimer>(new COgreTimer());
    mTimer->reset();
}


void CTimerModule::process()
{
    //cout << "CTimerModule::process" << endl;
    msgMapIter it = timers.begin();
    msgMapIter tmp;
    while(it != timers.end())
    {
        if(it->first > mTimer->getMsecs())
        {
            cout << "Timer expired @ " << mTimer->getMsecs() << endl;
            CGenericMessageManager::getInstance().trigger(it->second);

            // Supprimer ce timer :
            tmp = it;
            it++;
            timers.erase(tmp);
        }
        else
        {
            it++;
        }
    }


    // Et on fait de même avec les timers périodiques :
    /*pair<unsigned long int, unsigned long int> p;
    for(perMsgMapIter it = timersPeriodic.begin(); it != timersPeriodic.end(); it++)
    {
        p = it->first;

        if(p->
    }*/
}


void CTimerModule::exit()
{
    timers.clear();
}


void CTimerModule::addTimeout(unsigned long int delay, shared_ptr<CMessage> msg)
{
    cout << "Adding timer @ " << (mTimer->getMsecs() + delay) << endl;
    timers.insert(pair<unsigned long int, shared_ptr<CMessage> >(mTimer->getMsecs() + delay, msg));
}


void CTimerModule::delTimeout(unsigned long int delay, shared_ptr<CMessage> msg)
{
    for(msgMapIter it=timers.begin(); it != timers.end(); it++)
    {
        if(it->first == delay && it->second == msg)
            timers.erase(it);
    }
}


void CTimerModule::addPeriodicTimeout(unsigned long int delay, shared_ptr<CMessage> msg, unsigned long int period)
{
    pair<unsigned long int, shared_ptr<CMessage> > p(delay, msg);
    timersPeriodic.insert(pair<unsigned long int, pair<unsigned long int, shared_ptr<CMessage> > >(delay, p));
}


void CTimerModule::delPeriodicTimeout(unsigned long int delay, shared_ptr<CMessage> msg, unsigned long int period)
{
    pair<unsigned long int, shared_ptr<CMessage> > p(delay, msg);

    for(perMsgMapIter it=timersPeriodic.begin(); it != timersPeriodic.end(); it++)
    {
        if(it->first == delay && it->second == p)
            timersPeriodic.erase(it);
    }
}


unsigned long int CTimerModule::getMsecs()
{
    return mTimer->getMsecs();
}


void CTimerModule::resetTimer(bool resTimeouts)
{
    mTimer->reset();

    if(resTimeouts)
        timers.clear();
}
