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

#include "../include/messagemodule.h"
#include "../messages/include/genericmessenger.h"

using namespace std;
using namespace boost;

namespace Gnoll
{
	namespace Core
	{
		Messages::Messenger* MessageModule::getMessageManager()
		{
			/**
			 * The default Message Manager is GenericMessenger
			 */
			return m_messageManager.get();
		}

		MessageModule::MessageModule() : 
			m_messageManager(new Messages::GenericMessenger())
		{
		}

		void MessageModule::init()
		{
		}

		void MessageModule::process()
		{
			m_messageManager->processQueue();
		}

		void MessageModule::exit()
		{
		}

		MessageModule::~MessageModule()
		{
		}
	}
}

