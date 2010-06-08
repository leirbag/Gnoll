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

#ifndef __FINITESTATEMACHINE_H__
#define __FINITESTATEMACHINE_H__

#include <map>

#include <boost/shared_ptr.hpp>

#include "state.h"
#include "transition.h"

using namespace boost;
using namespace std;

namespace Gnoll
{
	namespace Core
	{
		typedef shared_ptr<State> State_ptr;
		typedef shared_ptr<Transition> Transition_ptr;

		/**
		 *	The Finite State Machine. <br />
		 *  See also http://en.wikipedia.org/wiki/Finite_state_machine
		 */
		class FiniteStateMachine
		{
			public:
				/**
				 * This is a constructor
				 */
				FiniteStateMachine();

				/**
				 * This is a destructor
				 */
				~FiniteStateMachine();

				/**
				 * This method adds a transition (and its associated state) to a state.
				 * @param first The transition will be added to this state
				 * @param second This state will be activated if the transition is evaluated to True and first is activated.
				 * @param transition This is the transition between first and second
				 */
				void add(State_ptr first, State_ptr second, Transition_ptr transition);

				/**
				 * This method deletes a transition (and its associated state) to a state.
				 * @param first The transition will be deleted to this state
				 * @param second This state will be activated if the transition is evaluated to True and frist is activated.
				 * @param transition This is the transition between first and second
				 */
				void del(State_ptr first, State_ptr second, Transition_ptr transition);

				/**
				 * This will execute the active state and will evaluate all of the active state's transition
				 * in order to go ahead in the fsm.
				 */
				void execute();

				/**
				 *	This will set the active state.
				 * @param state This is the newly active state
				 */
				void setActiveState(State_ptr state);

			private:
				/**
				 * This is a private struct.<br>
				 * It contains a state and a transition.<br>
				 * If the transition is evaluated to True, the state is activated. A superTransition is linked to another state.
				 * And the superTransition's transition is evaluated only if this state is activated.
				 */
				struct superTransition
				{
					/**
					 * The state that will be activated once the transition will be evaluated to True
					 */
					State_ptr state;

					/**
					 * This transition has to be evaluated to True in order to activated its associated state
					 */
					shared_ptr<Transition> transition;

					/**
					 * This is a constructor
					 */
					superTransition(State_ptr st, Transition_ptr tr) { state = st; transition = tr; }

					/**
					 * This is an operator
					 */
					bool operator==(superTransition& op) { return (op.state == state) && (op.transition == transition); }
				};

				/**
				 * This is the active state
				 */
				State_ptr m_activestate;

				/**
				 * This holds every states and their superTransitions.
				 */
				multimap< State_ptr, shared_ptr<superTransition> > m_graph;
		};
	};
};

#endif // __CTRANSITION_H__
