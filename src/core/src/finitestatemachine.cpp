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

#include "../include/finitestatemachine.h"

#include <boost/shared_ptr.hpp>

namespace Gnoll
{
	namespace Core
	{
		FiniteStateMachine::FiniteStateMachine()
		{
		}

		FiniteStateMachine::~FiniteStateMachine()
		{
			if(m_activestate.get() != NULL)
				m_activestate->onExit();
		}

		void FiniteStateMachine::add(shared_ptr<State> first, shared_ptr<State> second, shared_ptr<Transition> transition)
		{
			if((first.get() != NULL) && (second.get() != NULL) && (transition.get() != NULL))
			{
				shared_ptr<superTransition> sttemp(new superTransition(second, transition));
				m_graph.insert(pair<shared_ptr<State>, shared_ptr<superTransition> >(first, sttemp));
			}
		}

		void FiniteStateMachine::del(shared_ptr<State> first, shared_ptr<State> second, shared_ptr<Transition> transition)
		{
			if((first.get() != NULL) && (second.get() != NULL) && (transition.get() != NULL))
			{

				shared_ptr<superTransition> sttemp (new superTransition( second, transition));
				multimap< shared_ptr<State>, shared_ptr<superTransition> >::iterator temp;

				for(multimap< shared_ptr<State>, shared_ptr<superTransition> >::iterator it = m_graph.lower_bound(first);
						it != m_graph.upper_bound(first);
						it++)
				{
					if (*(it->second) == *sttemp)
					{
						/*
						 * It has been evaluated to True.
						 * So We have to delete the state.
						 */
						temp = it;
					}
				}
				m_graph.erase(temp);

			}
		}

		void FiniteStateMachine::execute()
		{
			// First, the active state is executed
			if(m_activestate.get() != NULL)
			{
				m_activestate->onProcess();

				// Then we will evaluate each of the active state's transitions.
				// If a transition is evaluated to True, it will activate its associated state.
				bool done = false;
				for(multimap< shared_ptr<State>, shared_ptr<superTransition> >::iterator it = m_graph.lower_bound(m_activestate);
						((it != m_graph.upper_bound(m_activestate)) && (done == false ));
						it++)
				{
					// each active state's transition is evaluated
					if(((it->second)->transition)->isValid())
					{
						/*
						 * It has been evaluated to True.
						 * So We have to Exit the active state,
						 *  update the active state,
						 *  and initialize the newly active state
						 */
						m_activestate->onExit();

						m_activestate = (it->second)->state;
						m_activestate->onInit();

						done = true;
					}
				}
			}
		}

		void FiniteStateMachine::setActiveState(shared_ptr<State> state)
		{
			/*
			 * Previous state is deactivated
			 */
			if (m_activestate.get() != NULL)
				m_activestate->onExit();

			/*
			 * The new state is set and activated
			 */
			if (state.get() != NULL)
			{
				m_activestate = state;
				state->onInit();
			}
		}
	}
}
