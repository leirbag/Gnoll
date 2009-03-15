/***************************************************************************
 *   Copyright (C) 2009 by Bruno Mahé                                      *
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


/*-----------------------------IComponent----------------------------------*\
|   This is an interface for components                                     |
|                                                                           |
\*-------------------------------------------------------------------------*/


#ifndef __ICOMPONENT_H__
#define __ICOMPONENT_H__


#include "../../dynamicobject/include/dynamicobject.h"



namespace Gnoll
{
	namespace Scene
	{
		class GObject;

		/**
		 * This an interface describing components
		 */
		class IComponent : public Gnoll::DynamicObject::DynamicObject
		{


			public:


				/**
				 * This is a constructor
				 */
				IComponent() {}


				/**
				 * This is a destructor
				 */
				virtual ~IComponent() {}


				/**
				 * Component initialization
				 * @param parent DynamicObject this component belongs to
				 * @param page Page where is located the parent DynamicObject
				 */
				virtual void init(Gnoll::Scene::GObject* parent, CPage* page) = 0;


				/**
				 * Component exits
				 */
				 virtual void exit() = 0;
		};
	}
}

#endif // __IComponent_H__
