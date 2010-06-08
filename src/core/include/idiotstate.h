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

#ifndef __IDIOTSTATE_H__
#define __IDIOTSTATE_H__

#include <string>
#include <iostream>

#include "cstate.h"

using namespace std;

namespace Gnoll
{
	namespace Core
	{
		/**
		 *	An idiot FSM's state
		 */
		class IdiotState: public State
		{
			public:
				/**
				 * This is called after entering this state
				 */
				virtual void onInit() { cout << "Je rentre dans l'etat : " << m_name << endl; }

				/**
				 * This is called during its activation
				 */
				virtual void onProcess() { cout << "Je suis dans l'etat : " << m_name << endl; }

				/**
				 * This is called before exiting this state
				 */
				virtual void onExit() { cout << "Je sors de l'etat : " << m_name << endl << endl; }

				/**
				 * This is a constructor
				 */
				IdiotState(string myname = "Anne nonyme") : m_name(myname) {}

				/**
				 * This is a destructor
				 */
				~IdiotState() {}

			private:
				/**
				 * It is the state's name
				 */
				string m_name;
		};
	};
};

#endif // __CIDIOTSTATE_H__
