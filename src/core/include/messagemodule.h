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

#ifndef __MESSAGEMODULE_H__
#define __MESSAGEMODULE_H__

#include "../include/module.h"
#include "../include/singleton.h"
#include "../messages/include/messenger.h"

using namespace std;
using namespace boost;

namespace Gnoll
{
	namespace Core
	{
		/**
		 *	The game messaging module.
		 */
		class MessageModule : public Module, public Singleton<MessageModule>
		{
			public:
				/**
				 * Default constructor
				 */
				MessageModule();

				/**
				 * Return the default message manager</br>
				 *   So if one write a new implementation of a Messages::Messenger, there would be
				 *   only one line to replace
				 * @Return Default message manager
				 */
				Messages::Messenger* getMessageManager();

				/**
				 * @copydoc Module::init
				 */
				virtual void init();

				/**
				 * @copydoc Module::process
				 */
				virtual void process();

				/**
				 * @copydoc Module::exit
				 */
				virtual void exit();

				/**
				 * @copydoc Module::~Module
				 */
				virtual ~MessageModule();

			private:
				/**
				 * Pointer to the Message Manager
				 */
				shared_ptr<Messages::Messenger> m_messageManager;
		};
	}
}

#endif
