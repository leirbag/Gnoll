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


/*---------------------------cgraphicmodule.cpp-----------------------------*\
|   The game graphic module                                                  |
|                                                                            |
|   Changelog :                                                              |
|               04/09/2008 - Gabriel - Initial release                       |
\*--------------------------------------------------------------------------*/

#include "../include/cstatsmodule.h"

namespace Gnoll
{
	namespace Stats
	{
		struct stats_i
		{
			float render_time;
			float fps;
		};

		CStatsModule::CStatsModule() :
			m_pStats(new stats_i)
		{
		}

		void CStatsModule::init()
		{
			m_pStats->render_time = 0.0f;
			m_pStats->fps = 0.0f;
		}

		void CStatsModule::process()
		{
		}

		void CStatsModule::exit()
		{
		}

		CStatsModule::~CStatsModule()
		{
			delete m_pStats;
		}

		void CStatsModule::setRenderTime(float time)
		{
			if(time > 0.0f)
				m_pStats->render_time = time;
		}

		float CStatsModule::getRenderTime()
		{
			return m_pStats->render_time;
		}
	};
};

