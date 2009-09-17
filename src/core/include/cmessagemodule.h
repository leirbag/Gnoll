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

#ifndef __CMESSAGEMODULE_H__
#define __CMESSAGEMODULE_H__

#include "../include/cmodule.h"
#include "../messages/include/messenger.h"
#include "../include/singleton.h"

using namespace std;
using namespace boost;

namespace Gnoll
{

	namespace Core
	{

		/**
		 *	The game messaging module. 
		 */ 
		class CMessageModule: public CModule, public Singleton<CMessageModule>
		{
			private:

				/**
				 * Pointer to the Message Manager
				 */
				shared_ptr<Messages::Messenger> m_messageManager;


			public:

				/**
				 * Default constructor
				 */
				CMessageModule();
	

				/**
				 * Return the default message manager</br>
				 *   So if one write a new implementation of a Messages::Messenger, there would be
				 *   only one line to replace
				 * @Return Default message manager
				 */
				Messages::Messenger * getMessageManager();


				/**
				 * @copydoc CModule::init
				 */
				virtual void init(); 


				/**
				 * @copydoc CModule::process
				 */
				virtual void process();


				/**
				 * @copydoc CModule::exit 
				 */
				virtual void exit();
	

				/**
				 * @copydoc CModule::~CModule
				 */
				virtual ~CMessageModule();

		};
	}

}


#endif // __CMESSAGEMODULE_H__
