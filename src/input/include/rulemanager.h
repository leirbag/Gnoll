/***************************************************************************
 *   Copyright (C) 2008 by Paf                                             *
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

#ifndef __RULEMANAGER_H__
#define __RULEMANAGER_H__

#include <vector>

#include "../include/rule.h"
#include "../../core/include/message.h"
#include "../../core/messages/include/messagetype.h"
#include "../../core/messages/include/messenger.h"
#include "../../core/include/messagemodule.h"
#include "../../core/messages/include/listener.h"

using namespace boost;
using namespace Gnoll::Core;

namespace Gnoll
{
	namespace Input
	{
		/// An input rule manager
		class RuleManager : public Messages::Listener
		{
			public:
				/**
				 * Constructor
				 * @param instanceName Instance name of the Rule manager
				 */
				RuleManager(const std::string &instanceName);

				/**
				 * This method is called in order to process a message
				 * @param message The message this method will have to process
				 */
				virtual void handle(MessagePtr message);

			private:
				/**
				 * Rules table
				 */
				std::vector<Rule> m_rules;

				/**
				 * Messages::MessageType for state messages
				 */
				Messages::MessageType m_stateEvent;

				/**
				 * Messages::MessageType for continuous messages
				 */
				Messages::MessageType m_continuousEvent;
		};
	}
}

#endif
