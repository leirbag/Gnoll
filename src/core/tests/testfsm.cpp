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


/*------------------------------testfsm.cpp--------------------------------*\
|   This program show how to use a Finite State Machine                     |
|                                                                           |
|   Changelog :                                                             |
|               04/29/2006 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/



#include "../include/finitestatemachine.h"
#include "../include/cidiotstate.h"
#include "../include/cidiottransition.h"

#include <boost/shared_ptr.hpp>


using namespace boost;
using namespace std;


int main()
{
	CFSM myfsm;

	// Some states are created
	shared_ptr<State> initialstate( new CIdiotState("le 1er") );
	shared_ptr<State> deusm( new CIdiotState("le 2eme") );
	shared_ptr<State> treusm( new CIdiotState("le 3eme") );
	shared_ptr<State> quatreusm( new CIdiotState("le 4eme") );

	// Two transitions are created : one is always false and the other is always true :)
   shared_ptr<CIdiotTransition> faux(new CIdiotTransition( false));
   shared_ptr<CIdiotTransition> vrai(new CIdiotTransition( true));

	// An initial state is set and some states are linked together
	myfsm.setActiveState( initialstate );
	myfsm.add(initialstate, deusm, faux);	
	myfsm.add(initialstate, treusm, vrai);	
	myfsm.add(treusm, deusm, vrai);	
	myfsm.add(deusm, initialstate, vrai);	

	//myfsm.del(initialstate, treusm, vrai);	
	//myfsm.del(deusm, initialstate, vrai);	
	

	// BOOOM ! FIRE ! GO GO GO !
	for (unsigned int i = 0; i < 4; i++)
		myfsm.execute();

	// Bye bye 
	return 0;
}
