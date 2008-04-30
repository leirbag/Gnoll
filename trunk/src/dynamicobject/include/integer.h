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


/*---------------------------------integer---------------------------------*\
|   This is a basic container supposed to an integer basic type             |
|                                                                           |
|   Changelog :                                                             |
|               08/04/2007 - Paf - Initial release                          |
|               09/25/2007 - Paf - Replace namespace Viracocha by Gnoll     |
|                                - Make Integer inherits from Scalar<int>   |
|                                                                           |
\*-------------------------------------------------------------------------*/


#ifndef __INTEGER_H__
#define __INTEGER_H__


#include "iattribute.h" 
#include "scalar.h" 
#include <sstream>

using namespace std;
using namespace boost;


namespace Gnoll
{

	namespace Core 
	{

		/**
		*	This is a simple attribute. </br> 
		*	This hold a signed integer.
		*/ 
		class Integer : public Scalar<int>
		{

			public:

				Integer(int _value = 0) : Scalar<int>("integer", _value) {};

		};
	};
};

#endif // __INTEGER_H__
