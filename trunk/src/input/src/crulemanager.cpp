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


/*-------------------------CInputEventsTranslator.h-------------------------*\
|   An input rule based on a dynamicobject			             |
|                                                                            |
|   Changelog :                                                              |
|               15/08/2008 - WT  - Initial release                           |
\*--------------------------------------------------------------------------*/



#include <boost/shared_ptr.hpp>

#include "../include/crule.h"
#include "../include/crulemanager.h"
#include "../include/ctranslationevents.h"

#include "../../core/include/cmessage.h"
#include "../../core/include/cmessagetype.h"
#include "../../core/include/cmessagemanager.h"
#include "../../core/include/cmessagemodule.h"

#include "../../dynamicobject/include/dynamicobject.h"
#include "../../dynamicobject/include/dynamicobjectmanager.h"

#include <vector>


using namespace std;
using namespace boost;
using namespace Gnoll::Core;
using namespace Gnoll::DynamicObject;


namespace Gnoll
{

	namespace Input
	{

		CRuleManager::CRuleManager(const std::string &instanceName) :
			m_stateEvent(ACTION_EVENT_STATE_TYPE),
			m_continuousEvent(ACTION_EVENT_TYPE)
		{
			DynamicObjectManager *dom = DynamicObjectManager::getInstancePtr();
			shared_ptr< Gnoll::DynamicObject::DynamicObject > rules = dom->load(instanceName);

			// We get all the dynamicobject filenames
			shared_ptr< List > ruleList = rules->getAttribute< List >("ruleList");
			typedef list< shared_ptr< IAttribute > >::iterator ListIterator;
			for (ListIterator itAttrs = ruleList->begin(); itAttrs != ruleList->end(); itAttrs++)
			{
				if (shared_ptr< String > rule = dynamic_pointer_cast< String >(*itAttrs))
				{
					// Add the rule to the table
					m_rules.push_back(CRule( *rule ));
					cout << *rule << endl;
				}
			}
		}



		void CRuleManager::handle( shared_ptr<CMessage> message )
		{
			ActionEvent actionEvent = message->getData<ActionEvent>();
			CMessageType msgType = message->getType();

			for(vector<CRule>::iterator it = m_rules.begin(); it != m_rules.end(); ++it)
			{
				if(msgType == m_stateEvent)
				{
					it->updateRule(actionEvent);
				}
				else if(msgType == m_continuousEvent)
				{
					if(actionEvent.action == it->getTrigger() && it->checkConditions() == true)
					{
						it->fireAction();
					}
				}
			}
		}

	}
}
