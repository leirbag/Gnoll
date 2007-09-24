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


/*--------------------------ctimemodule.h----------------------------------*\
|   The time module handle all time related functionnalities                |
|                                                                           |
|   Changelog :                                                             |
|               05/12/2007 - Vince - Initial release                        |
|               09/20/2007 - Paf   - Complete the module                    |
|               09/23/2007 - Paf   - Add createTimer method()               |
|                                  - s/TIMER/TIMEOUT/g to make a separation |
|                                  - Rename timeout to delayed event        |
|                                  - Rename periodic timeout to periodic    |
|                                      event                                |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include "../include/ctimemodule.h"
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

		CTimeModule::CTimeModule()
		{
		}


		CTimeModule::~CTimeModule()
		{
		}


		void CTimeModule::init()
		{

			m_timer = this->createTimer();

			m_timer->reset();


			shared_ptr<CMessageListener> createDelayedEventListener ( new CCreateDelayedEventListener() );
			CMessageType createDelayedEventType("CREATE_DELAYED_EVENT");

			this->addListener ( createDelayedEventListener, createDelayedEventType );


			shared_ptr<CMessageListener> destroyDelayedEventListener ( new CDestroyDelayedEventListener() );
			CMessageType destroyDelayedEventType("DESTROY_DELAYED_EVENT");

			this->addListener ( destroyDelayedEventListener, destroyDelayedEventType );


			shared_ptr<CMessageListener> createPeriodicEventListener ( new CCreatePeriodicEventListener() );
			CMessageType createPeriodicEventType("CREATE_PERIODIC_EVENT");

			this->addListener ( createPeriodicEventListener, createPeriodicEventType );


			shared_ptr<CMessageListener> destroyPeriodicEventListener ( new CDestroyPeriodicEventListener() );
			CMessageType destroyPeriodicEventType("DESTROY_PERIODIC_EVENT");

			this->addListener ( destroyPeriodicEventListener, destroyPeriodicEventType );
		}

		bool CTimeModule::addListener(shared_ptr<CMessageListener> _listener, CMessageType _type)
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

		void CTimeModule::process()
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

					addPeriodicEvent(period, msg, period);
			
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


		void CTimeModule::exit()
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


		void CTimeModule::addDelayedEvent(unsigned long int delay, shared_ptr<CMessage> msg)
		{
			m_timers.insert(pair<unsigned long int, shared_ptr<CMessage> >(m_timer->getMsecs() + delay, msg));
		}


		void CTimeModule::delDelayedEvent(unsigned long int delay, shared_ptr<CMessage> msg)
		{
			for(msgMapIter it = m_timers.begin(); it != m_timers.end(); it++)
			{
				if(it->first == delay && it->second == msg)
				{
					m_timers.erase(it);
				}
			}
		}


		void CTimeModule::addPeriodicEvent(unsigned long int delay, shared_ptr<CMessage> msg, unsigned long int period)
		{
			pair<unsigned long int, shared_ptr<CMessage> > p(period, msg);
			m_timersPeriodic.insert(pair<unsigned long int, pair<unsigned long int, shared_ptr<CMessage> > >(m_timer->getMsecs() + delay, p));
		}


		void CTimeModule::delPeriodicEvent(unsigned long int delay, shared_ptr<CMessage> msg, unsigned long int period)
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


		unsigned long int CTimeModule::getMsecs()
		{
			return m_timer->getMsecs();
		}


		void CTimeModule::resetTimer(bool resTimeouts)
		{
			m_timer->reset();
		
			if(resTimeouts)
			{
				m_timers.clear();
			}
		}


		shared_ptr<ITimer> CTimeModule::createTimer()
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


