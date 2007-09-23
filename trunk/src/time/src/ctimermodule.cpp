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
|               09/20/2007 - Paf   - Complete the module                    |
|               09/23/2007 - Paf   - Add createTimer method()               |
|                                  - s/TIMER/TIMOUT/g to make a separation  |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include "../include/ctimermodule.h"
#include <iostream>


#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
	#include "../include/clinuxtimer.h"
#else
	#include "../include/cogretimer.h"
#endif


using namespace std;


namespace Gnoll
{

	namespace Core
	{

		CTimerModule::CTimerModule()
		{
		}


		CTimerModule::~CTimerModule()
		{
		}


		void CTimerModule::init()
		{

			m_timer = this->createTimer();

			m_timer->reset();


			shared_ptr<CMessageListener> createTimerListener ( new CCreateTimerListener() );
			CMessageType createTimerType("CREATE_TIMER");

			this->addListener ( createTimerListener, createTimerType );


			shared_ptr<CMessageListener> destroyTimerListener ( new CDestroyTimerListener() );
			CMessageType destroyTimerType("DESTROY_TIMER");

			this->addListener ( destroyTimerListener, destroyTimerType );


			shared_ptr<CMessageListener> createPeriodicTimerListener ( new CCreatePeriodicTimerListener() );
			CMessageType createPeriodicTimerType("CREATE_PERIODIC_TIMER");

			this->addListener ( createPeriodicTimerListener, createPeriodicTimerType );


			shared_ptr<CMessageListener> destroyPeriodicTimerListener ( new CDestroyPeriodicTimerListener() );
			CMessageType destroyPeriodicTimerType("DESTROY_PERIODIC_TIMER");

			this->addListener ( destroyPeriodicTimerListener, destroyPeriodicTimerType );
		}

		bool CTimerModule::addListener(shared_ptr<CMessageListener> _listener, CMessageType _type)
		{
			bool result = true;

			CGenericMessageManager* messageManager = CGenericMessageManager::getInstancePtr();
			
			if (messageManager->addListener ( _listener, _type ) != true)
			{
				result = false;
	//			LogManager::getSingleton().logMessage("Couldn't add listener for timer module");
			}

			pair< shared_ptr<CMessageListener>, CMessageType> listener( _listener, _type);
			m_listListeners.push_back(listener);

			return result;
		}

		void CTimerModule::process()
		{
			msgMapIter it = m_timers.begin();
			while(it != m_timers.end())
			{
				if(it->first < m_timer->getMsecs())
				{
					CGenericMessageManager::getInstancePtr()->trigger(it->second);

					// Delete this timer
					msgMapIter tmp = it;
					it++;
					m_timers.erase(tmp);
				}
				else
				{
					it++;
				}
			}


			pair<unsigned long int, shared_ptr<CMessage> > p;
			perMsgMapIter itPeriodic = m_timersPeriodic.begin();
			while ( itPeriodic != m_timersPeriodic.end() )
			{
				if(itPeriodic->first < m_timer->getMsecs())
				{
	
					pair<unsigned long int, shared_ptr<CMessage> > p = itPeriodic->second;

					unsigned long int period = p.first;
					shared_ptr<CMessage> msg = p.second;

					addPeriodicTimeout(period, msg, period);
			
					// Delete this timer
					perMsgMapIter tmp = itPeriodic;
					itPeriodic++;
					m_timersPeriodic.erase(tmp);

					CGenericMessageManager::getInstancePtr()->trigger(msg);
				}
				else
				{
					itPeriodic++;
				}
			}
		}


		void CTimerModule::exit()
		{
			m_timers.clear();


			/**
			 * Removing all registered listeners
			 */

			CGenericMessageManager* messageManager = CGenericMessageManager::getInstancePtr();

			for(list< pair< shared_ptr<CMessageListener>, CMessageType> >::iterator it = m_listListeners.begin(); it != m_listListeners.end(); it++)
			{
				if (messageManager->delListener ( (*it).first, (*it).second ) != true)
				{
		//			LogManager::getSingleton().logMessage("Couldn't add listener for timer module");
				}

			}

			m_listListeners.clear();

		}


		void CTimerModule::addTimeout(unsigned long int delay, shared_ptr<CMessage> msg)
		{
			m_timers.insert(pair<unsigned long int, shared_ptr<CMessage> >(m_timer->getMsecs() + delay, msg));
		}


		void CTimerModule::delTimeout(unsigned long int delay, shared_ptr<CMessage> msg)
		{
			for(msgMapIter it = m_timers.begin(); it != m_timers.end(); it++)
			{
				if(it->first == delay && it->second == msg)
				{
					m_timers.erase(it);
				}
			}
		}


		void CTimerModule::addPeriodicTimeout(unsigned long int delay, shared_ptr<CMessage> msg, unsigned long int period)
		{
			pair<unsigned long int, shared_ptr<CMessage> > p(period, msg);
			m_timersPeriodic.insert(pair<unsigned long int, pair<unsigned long int, shared_ptr<CMessage> > >(m_timer->getMsecs() + delay, p));
		}


		void CTimerModule::delPeriodicTimeout(unsigned long int delay, shared_ptr<CMessage> msg, unsigned long int period)
		{
	 
			pair<unsigned long int, shared_ptr<CMessage> > p(period, msg);
	
		  	for(perMsgMapIter it = m_timersPeriodic.begin(); it != m_timersPeriodic.end(); it++)
			{
				if(it->second == p)
				{
						m_timersPeriodic.erase(it);
				}
			}
		}


		unsigned long int CTimerModule::getMsecs()
		{
			return m_timer->getMsecs();
		}


		void CTimerModule::resetTimer(bool resTimeouts)
		{
			m_timer->reset();
		
			if(resTimeouts)
			{
				m_timers.clear();
			}
		}


		shared_ptr<ITimer> CTimerModule::createTimer()
		{

			shared_ptr<ITimer> timer;
			/**
			 * COgreTimer is multiplatform but introduces also some heavy dependencies.
			 * You cannot use it outside of Ogre as it is.
			 */

			#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
				timer = shared_ptr<ITimer>(new CLinuxTimer());
			#else
				timer = shared_ptr<ITimer>(new COgreTimer());
			#endif

			return timer;

		}

	}
}


