/***************************************************************************
 *   Copyright(C) 2007 by Paf                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *  (at your option) any later version.                                    *
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

#include "../include/timemodule.h"

#include <iostream>

#include <OgrePlatform.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
	#include "../include/linuxtimer.h"
#else
	#include "../include/ogretimer.h"
#endif

using namespace std;
using namespace Gnoll::Core;

namespace Gnoll
{
	namespace Time 
	{
		TimeModule::TimeModule()
		{
		}

		TimeModule::~TimeModule()
		{
		}

		void TimeModule::init()
		{
			m_timer = this->createTimer();
			m_timer->reset();

			shared_ptr<Messages::Listener> createDelayedEventListener(new CreateDelayedEventListener());
			Messages::MessageType createDelayedEventType("CREATE_DELAYED_EVENT");

			this->addListener(createDelayedEventListener, createDelayedEventType);

			shared_ptr<Messages::Listener> destroyDelayedEventListener(new DestroyDelayedEventListener());
			Messages::MessageType destroyDelayedEventType("DESTROY_DELAYED_EVENT");

			this->addListener(destroyDelayedEventListener, destroyDelayedEventType);

			shared_ptr<Messages::Listener> createPeriodicEventListener(new CreatePeriodicEventListener());
			Messages::MessageType createPeriodicEventType("CREATE_PERIODIC_EVENT");

			this->addListener(createPeriodicEventListener, createPeriodicEventType);

			shared_ptr<Messages::Listener> destroyPeriodicEventListener(new DestroyPeriodicEventListener());
			Messages::MessageType destroyPeriodicEventType("DESTROY_PERIODIC_EVENT");

			this->addListener(destroyPeriodicEventListener, destroyPeriodicEventType);
		}

		bool TimeModule::addListener(shared_ptr<Messages::Listener> _listener, Messages::MessageType type)
		{	
			boost::recursive_mutex::scoped_lock lock(m_listListenersMutex);

			bool result = true;

			Messages::Messenger* messageManager = MessageModule::getInstancePtr()->getMessageManager();
			
			try
			{
				messageManager->addListener(_listener, type);
				pair< shared_ptr<Messages::Listener>, Messages::MessageType> listener(_listener, type);
				m_listListeners.push_back(listener);
			}
			catch(...)
			{
				result = false;
			}

			return result;
		}

		void TimeModule::processTimers()
		{
			boost::recursive_mutex::scoped_lock lock(m_timersMutex);

			msgMapIter it = m_timers.begin();

			while(it != m_timers.end())
			{
				if(it->first < m_timer->getMsecs())
				{
					MessageModule::getInstancePtr()->getMessageManager()->triggerMessage(it->second);

					// Delete this timer
					msgMapIter tmp = it;
					it++;
					m_timers.erase(tmp);
				}
				else
					it++;
			}
		}

		void TimeModule::processPeriodicTimers()
		{
			boost::recursive_mutex::scoped_lock lock(m_timersPeriodicMutex);

			pair<unsigned long int, shared_ptr<Message> > p;
			perMsgMapIter itPeriodic = m_timersPeriodic.begin();

			while(itPeriodic != m_timersPeriodic.end())
			{
				if(itPeriodic->first < m_timer->getMsecs())
				{
					pair<unsigned long int, shared_ptr<Message> > p = itPeriodic->second;

					unsigned long int period = p.first;
					shared_ptr<Message> msg = p.second;

					addPeriodicEvent(period, msg, period);
			
					// Delete this timer
					perMsgMapIter tmp = itPeriodic;
					itPeriodic++;
					m_timersPeriodic.erase(tmp);

					MessageModule::getInstancePtr()->getMessageManager()->triggerMessage(msg);
				}
				else
					itPeriodic++;
			}
		}

		void TimeModule::process()
		{
			processTimers();
			processPeriodicTimers();
		}

		void TimeModule::exit()
		{
			m_timers.clear();

			/**
			 * Removing all registered listeners
			 */
			Messages::Messenger * messageManager = MessageModule::getInstancePtr()->getMessageManager();
			for(list< pair< shared_ptr<Messages::Listener>, Messages::MessageType> >::iterator it = m_listListeners.begin(); 
				it != m_listListeners.end(); it++)
			{
				try
				{
					messageManager->delListener((*it).first,(*it).second);
				}
				catch(...)
				{
				}
			}

			m_listListeners.clear();
		}


		void TimeModule::addDelayedEvent(unsigned long int delay, shared_ptr<Message> msg)
		{
			boost::recursive_mutex::scoped_lock lock(m_timersMutex);
			m_timers.insert(pair<unsigned long int, shared_ptr<Message> >(m_timer->getMsecs() + delay, msg));
		}


		void TimeModule::delDelayedEvent(unsigned long int delay, shared_ptr<Message> msg)
		{
			boost::recursive_mutex::scoped_lock lock(m_timersMutex);

			for(msgMapIter it = m_timers.begin(); it != m_timers.end(); it++)
				if(it->first == delay && it->second == msg)
					m_timers.erase(it);
		}


		void TimeModule::addPeriodicEvent(unsigned long int delay, shared_ptr<Message> msg, unsigned long int period)
		{
			boost::recursive_mutex::scoped_lock lock(m_timersPeriodicMutex);

			pair<unsigned long int, shared_ptr<Message> > p(period, msg);
			m_timersPeriodic.insert(pair<unsigned long int, pair<unsigned long int, 
									shared_ptr<Message> > >(m_timer->getMsecs() + delay, p));
		}


		void TimeModule::delPeriodicEvent(unsigned long int delay, shared_ptr<Message> msg, unsigned long int period)
		{
			boost::recursive_mutex::scoped_lock lock(m_timersPeriodicMutex);
			pair<unsigned long int, shared_ptr<Message> > p(period, msg);
	
		  	for(perMsgMapIter it = m_timersPeriodic.begin(); it != m_timersPeriodic.end(); it++)
				if(it->second == p)
					m_timersPeriodic.erase(it);
		}

		unsigned long int TimeModule::getMsecs()
		{
			boost::recursive_mutex::scoped_lock lock(m_timerMutex);
			return m_timer->getMsecs();
		}

		void TimeModule::resetTimer(bool resTimeouts)
		{
			boost::recursive_mutex::scoped_lock lock(m_timerMutex);
			m_timer->reset();
		
			if(resTimeouts)
				m_timers.clear();
		}

		shared_ptr<AbstractTimer> TimeModule::createTimer()
		{
			boost::recursive_mutex::scoped_lock lock(m_timerMutex);
			shared_ptr<AbstractTimer> timer;

			/**
			 * OgreTimer is multiplatform but introduces also some heavy dependencies.
			 * You cannot use it outside of Ogre as it is.
			 */
			#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
				timer = shared_ptr<AbstractTimer>(new LinuxTimer());
			#else
				timer = shared_ptr<AbstractTimer>(new OgreTimer());
			#endif

			return timer;
		}

	}
}
