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

#include "../include/statsmodule.h"

#include "../../core/include/message.h"
#include "../../core/messages/include/messagetype.h"
#include "../../core/messages/include/messenger.h"
#include "../../input/include/coisinputmodule.h"
#include "../../input/include/ctranslationevents.h"
#include "../../input/include/cinputmouseevents.h"

namespace Gnoll
{
	namespace Stats
	{
		struct stats_i
		{
			float render_time;
			float fps;
		};

		StatsModule::StatsModule() :
			m_pStats(new stats_i)
		{
		}

		void StatsModule::init()
		{
			Gnoll::Core::MessageModule* messageModule = Gnoll::Core::MessageModule::getInstancePtr();
			m_listenerModule = boost::shared_ptr<StatsModuleListener>(new StatsModuleListener);
			messageModule->getMessageManager()->addListener(m_listenerModule, Messages::MessageType("GRAPHIC_FRAME_RENDERED"));
			m_pStats->render_time = 0.0f;
			m_pStats->fps = 0.0f;
		}

		void StatsModule::process()
		{
		}

		void StatsModule::exit()
		{
		}

		StatsModule::~StatsModule()
		{
			delete m_pStats;
		}

		void StatsModule::setRenderTime(float time)
		{
			if(time > 0.0f)
				m_pStats->render_time = time;
		}

		float StatsModule::getRenderTime()
		{
			return m_pStats->render_time;
		}
	};
};

