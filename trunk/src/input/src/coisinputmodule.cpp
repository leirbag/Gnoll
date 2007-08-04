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


/*--------------------------coisinputmodule.cpp----------------------------*\
|   The game input module that use OIS                                      |
|                                                                           |
|   Changelog :                                                             |
|               06/23/2006 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../include/coisinputmodule.h"
#include "../include/coisinputmanager.h"
#include <iostream>


COISInputModule::COISInputModule()
{

}


void COISInputModule::init()
{
	inputmanager = shared_ptr<COISInputManager>(new COISInputManager());
	inputmanager->initialise( );

}


void COISInputModule::process()
{	
	inputmanager->capture();
}


void COISInputModule::exit()
{

}


COISInputModule::~COISInputModule()
{

}
