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

#include <boost/shared_ptr.hpp>

#include "../include/crule.h"
#include "../include/ctranslationevents.h"

#include "../../core/include/message.h"
#include "../../core/messages/include/messagetype.h"
#include "../../core/messages/include/messenger.h"
#include "../../core/include/messagemodule.h"

#include "../../dynamicobject/include/dynamicobject.h"
#include "../../dynamicobject/include/dynamicobjectmanager.h"

using namespace std;
using namespace boost;
using namespace Gnoll::Core;
using namespace Gnoll::DynamicObject;

namespace Gnoll
{
	namespace Input
	{

		CRule::CRule(const std::string &instanceName)
		{
			DynamicObjectManager *dom = DynamicObjectManager::getInstancePtr();
			shared_ptr< Gnoll::DynamicObject::DynamicObject > rule = dom->load(instanceName);

			// We get all the rule conditions and store them in the conditions map
			shared_ptr< List > conditions = rule->getAttribute< List >("conditions");
			typedef list< shared_ptr< AbstractAttribute > >::iterator ListIterator;
			for (ListIterator itAttrs = conditions->begin(); itAttrs != conditions->end(); itAttrs++)
			{
				if (shared_ptr< String > condition = dynamic_pointer_cast< String >(*itAttrs))
				{
					// Conditions are initially false
					m_conditions[*condition] = false;
				}
			}
			shared_ptr< List > exceptions = rule->getAttribute< List >("exceptions");
			for (ListIterator itAttrs = exceptions->begin(); itAttrs != exceptions->end(); itAttrs++)
			{
				if (shared_ptr< String > exception = dynamic_pointer_cast< String >(*itAttrs))
				{
					// EXceptions are initially false
					m_exceptions[*exception] = false;
				}
			}

			// Then just grab the trigger and the action to be fired
			m_trigger = *(rule->getAttribute< String >("rule_trigger"));
			m_action  = *(rule->getAttribute< String >("rule_action"));
		}



		bool CRule::checkConditions()
		{
			bool fire = true;
			for(map<string, bool>::iterator it = m_conditions.begin(); (it != m_conditions.end()) && (fire == true); ++it)
			{
				fire = it->second;
			}
			for(map<string, bool>::iterator it = m_exceptions.begin(); (it != m_exceptions.end()) && (fire == true); ++it)
			{
				fire = !it->second;
			}

			return fire;
		}



		void CRule::updateRule(const ActionEvent &ae)
		{
			// First check if this is a condition of this rule
			if( m_conditions.find(ae.action) != m_conditions.end() )
			{
				m_conditions[ae.action] = ae.intensity ? true : false;
			}
			if( m_exceptions.find(ae.action) != m_exceptions.end() )
			{
				m_exceptions[ae.action] = ae.intensity ? true : false;
			}
		}


		bool CRule::fireAction() const
		{
			Messages::MessageType actionEventType(ACTION_EVENT_TYPE);

			ActionEvent actionEvent(m_action, 1.0f);

			shared_ptr<boost::any> data (new boost::any(actionEvent) ) ;
			shared_ptr<Message>  actionMessage (new Message( actionEventType, data ));

			std::ostringstream tmpString;
			try
			{
				MessageModule::getInstancePtr()->getMessageManager()->queueMessage(actionMessage);
				tmpString << "Message ajoute ["<< m_action << "]";
			}
			catch(...)
			{
				tmpString << "Message NON ajoute ["<< m_action << "]";
			}
			Gnoll::Log::LogModule::getInstancePtr()->logMessage( tmpString.str() );

			return true;
		}

	}
}
