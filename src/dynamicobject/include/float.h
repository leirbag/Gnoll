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


/*----------------------------------float----------------------------------*\
|   This is a float attribute for DynamicObject                             |
|                                                                           |
|   Changelog :                                                             |
|               09/24/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/



#ifndef __FLOAT_H__
#define __FLOAT_H__


#include "iattribute.h"
#include "scalar.h"

using namespace std;
using namespace boost;

namespace Gnoll
{
	namespace DynamicObject
	{

		/**
		 *	This is a simple attribute.
		 */
		class Float : public Scalar<float>
		{
			public:


				/**
				 * Returns Float's DynamicObject name
				 * @return Float's DynamicObject name
				 */
				inline static const char * DYNAMIC_OBJECT_NAME() {return "float";}



				Float(float _value = 0.0f) : Scalar<float>(Float::DYNAMIC_OBJECT_NAME(), _value) {};

		};

	}
}

#endif // __FLOAT_H__
