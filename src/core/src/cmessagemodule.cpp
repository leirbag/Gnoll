/***************************************************************************
 *   Copyright (C) 2006 by Paf                                             *
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


/*-----------------------------CMessageModule------------------------------*\
|   This is the message module                                              |
|                                                                           |
|   Changelog :                                                             |
|               11/13/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../include/cmessagemodule.h"


using namespace std;
using namespace boost;


namespace Gnoll
{

	namespace Core
	{
	
		CMessageManager* CMessageModule::getMessageManager()
		{
			/**
			 * The default Message Manager is CGenericMessageManager
			 */
			return m_messageManager.get();
		}


		CMessageModule::CMessageModule() : m_messageManager( new CGenericMessageManager() )
		{
		}

		void CMessageModule::init() 
		{
		}

		void CMessageModule::process() 
		{
			m_messageManager->process();
		}

		void CMessageModule::exit() 
		{
		}
	
		CMessageModule::~CMessageModule()
		{
		}

	}

}

