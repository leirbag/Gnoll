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


/*---------------------------cidiottransition.h----------------------------*\
|   An idiot FSM's transitions                                              |
|                                                                           |
|   Changelog :                                                             |
|               04/29/2006 - Paf - Initial release                          |
|               04/10/2006 - Gabriel - Add namespace Gnoll and Core         |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include "ctransition.h"


#ifndef __CIDIOTTRANSITION_H__
#define __CIDIOTTRANSITION_H__

namespace Gnoll
{
	namespace Core
	{
		/**
		 *	An idiot FSM's transition. <br> It will always be evaluated as True or False.
		 */
		class CIdiotTransition : public CTransition
		{
			private:

				/**
				 * isvalidate() will always return this value.
				 */
				bool m_valid;

			public:

				/**
				 * This is a constructor.
				 * @param validation This the value this transition will always return when it's evaluated.
				 */
				CIdiotTransition( bool validation): m_valid(validation) {};

				/**
				 * This will check if this transition is valid, or not.
				 * @return The validation test's result
				 */
				bool isValid() { return m_valid; }

		};
	};
};

#endif // __CIDIOTTRANSITION_H__
