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

#ifndef __TIMEMODULE_H__
#define __TIMEMODULE_H__ 

#include <map>
#include <list>

#include <boost/shared_ptr.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include "abstracttimer.h"
#include "../../core/include/module.h"
#include "../../core/include/singleton.h"
#include "../../core/include/messagemodule.h"
#include "../../core/messages/include/listener.h"
#include "../../core/messages/include/messagetype.h"
#include "createdelayedeventlistener.h"
#include "destroydelayedeventlistener.h"
#include "createperiodiceventlistener.h"
#include "destroyperiodiceventlistener.h"

using namespace Gnoll::Core;
using namespace std;
using namespace boost;

namespace Gnoll
{
	namespace Time
	{
		class TimeModule : public Module, public Gnoll::Core::Singleton<TimeModule>
		{
			public:
				/**
				 *   Default constructor
				 */
				TimeModule();

				/**
				 *   Virtual destructor
				 */
				virtual ~TimeModule();

				/**
				 * Init the module and its subsystems
				 */
				virtual void init();

				/**
				 * Handle everything related to time.</br>
				 * This would generate each event that have expired
				 */
				virtual void process();

				/**
				 * Shutdown the module and its subsystems
				 */
				virtual void exit();

				/**
				 *   Add an unique event @ delay msecs after timer start/reset, sending
		 	 	 *   msg when triggered
		 	 	 */
				void addDelayedEvent(unsigned long int delay, shared_ptr<Message> msg);

				/**
				 *   Delete an unique event
				 */
				void delDelayedEvent(unsigned long int delay, shared_ptr<Message> msg);

				/**
				 *   Add a periodic event @ delay msecs after timer start/reset, sending
				 *   msg when triggered, every period msecs
				 */
				void addPeriodicEvent(unsigned long int delay, shared_ptr<Message> msg, unsigned long int period);

				/**
				 *   Delete a periodic event
				 */
				void delPeriodicEvent(unsigned long int delay, shared_ptr<Message> msg, unsigned long int period);

				/**
				 *   Get the current module's time
				 */
				unsigned long int getMsecs();

				/**
				 * Resets the internal timer of this module
				 */
				void resetTimer(bool resTimeouts = false);

				/**
				 * Create a Timer
				 * @return A Timer
				 */
				shared_ptr<AbstractTimer> createTimer();

			private:
				/**
				 * Add a listener and activates it.</br>
				 * This list of listeners is managed by the module
				 */
				bool addListener(shared_ptr<Messages::Listener> listener, Messages::MessageType type);

				/**
				 * Process timers
				 */
				void processTimers();

				/**
				 * Process periodic timers
				 */
				void processPeriodicTimers();

			private:
				typedef multimap<unsigned long int, shared_ptr<Message> >::iterator msgMapIter;
				typedef multimap<unsigned long int, pair<unsigned long int, shared_ptr<Message> > >::iterator perMsgMapIter;

				/**
				 *	Timer to get time 
				 */
				shared_ptr<AbstractTimer> m_timer;

				/**
				 * Mutex dedicated to m_timers
				 */
				boost::recursive_mutex m_timerMutex;

				/**
				 *	Map of delayed events.</br>
				 *	Key : When an event will happen (in milliseconds) </br>
				 *	Value : Message to send when the delay expires
				 */
				multimap<unsigned long int, shared_ptr<Message> > m_timers;

				/**
				 * Mutex dedicated to m_timers
				 */
				boost::recursive_mutex m_timersMutex;

				/**
				 *	Map of periodic events
				 *	Key : When an event will happen (in milliseconds) </br>
				 *	Value : Pair of the following values : period and message to send when the delay expires
				 */
				multimap<unsigned long int, pair<unsigned long int, shared_ptr<Message> > > m_timersPeriodic;

				/**
				 * Mutex dedicated to m_timersPeriodic
				 */
				boost::recursive_mutex m_timersPeriodicMutex;

				/**
				 * A list of listeners and the type of messages they are listening to
				 */
				list< pair< shared_ptr<Messages::Listener>, Messages::MessageType> > m_listListeners;

				/**
				 * Mutex dedicated to m_listListeners
				 */
				boost::recursive_mutex m_listListenersMutex;
		};
	}
}

#endif
