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


/*---------------------------------cfsm.h----------------------------------*\
|   The Finite State Machine                                                |
|                                                                           |
|   Changelog :                                                             |
|               04/29/2006 - Paf - Initial release                          |
|               04/10/2008 - Gabriel - Add namespace Gnoll and Core         |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include <boost/shared_ptr.hpp>
#include <map>

#include "cstate.h"
#include "ctransition.h"

#ifndef __CFSM_H__
#define __CFSM_H__

using namespace boost;
using namespace std;


namespace Gnoll
{
	namespace Core
	{
/**
 *	The Finite State Machine.
 * <br>See also http://en.wikipedia.org/wiki/Finite_state_machine
 */
		class CFSM
		{
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
					shared_ptr<CState> state;

					/**
					 * This transition has to be evaluated to True in order to activated its associated state
					 */
					shared_ptr<CTransition> transition;

					/**
					 * This is a constructor
					 */
					superTransition(shared_ptr<CState> st, shared_ptr<CTransition> tr) { state = st; transition = tr; }

					/**
					 * This is an operator
					 */
					bool operator== (superTransition& op) { return ( (op.state == state) && (op.transition == transition) );}

				};


				/**
				 * This is the active state
				 */
				shared_ptr<CState> m_activestate;

				/**
				 * This holds every states and their superTransitions.
				 */
				multimap< shared_ptr<CState>, shared_ptr<superTransition> > m_graph;


			public:

				/**
				 * This is a constructor
				 */
				CFSM();

				/**
				 * This is a destructor
				 */
				~CFSM();

				/**
				 * This method adds a transition (and its associated state) to a state.
				 * @param first The transition will be added to this state
				 * @param second This state will be activated if the transition is evaluated to True and first is activated.
				 * @param transition This is the transition between first and second
				 */
				void add ( shared_ptr<CState> first, shared_ptr<CState> second, shared_ptr<CTransition> transition);

				/**
				 * This method deletes a transition (and its associated state) to a state.
				 * @param first The transition will be deleted to this state
				 * @param second This state will be activated if the transition is evaluated to True and frist is activated.
				 * @param transition This is the transition between first and second
				 */
				void del ( shared_ptr<CState> first, shared_ptr<CState> second, shared_ptr<CTransition> transition);

				/**
				 * This will execute the active state and will evaluate all of the active state's transition
				 * in order to go ahead in the fsm.
				 */
				void execute();

				/**
				 *	This will set the active state.
				 * @param state This is the newly active state
				 */
				void setActiveState( shared_ptr<CState> state);
		};
	};
};
#endif // __CTRANSITION_H__
