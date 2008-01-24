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


/*----------------------------iscenefactory.h-------------------------------*\
|   This is the scene factory interface                                      |
|                                                                            |
|   Changelog :                                                              |
|               01/07/2007 - Paf - Initial release                           |
|                                                                            |
\*--------------------------------------------------------------------------*/


#include <boost/shared_ptr.hpp>
#include <Ogre.h>
#include <OgreLogManager.h>
#include <OgrePlatform.h>
#include <libxml++/libxml++.h>

#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif


using namespace std;

#ifndef __ISCENEFACTORY_H__
#define __ISCENEFACTORY_H__


/**
 *	The game graphic module. 
 */ 
class  ISceneFactory 
{
	public:
		ISceneFactory() {};
		virtual ~ISceneFactory() {};

		virtual void loadScene() = 0;

};

#endif // __ISCENEFACTORY_H__
