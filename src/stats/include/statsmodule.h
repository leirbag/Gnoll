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

#ifndef INCLUDED_STATSMODULE_H
#define INCLUDED_STATSMODULE_H

#include <boost/shared_ptr.hpp>

#include "../include/statsmodulelistener.h"
#include "../../core/include/cmodule.h"
#include "../../core/include/singleton.h"

using namespace Gnoll::Core;

namespace Gnoll
{
	namespace Stats
	{
		struct stats_i;

		/**
		 *	The game stats module.
		 */
		class StatsModule : public CModule, public Gnoll::Core::Singleton<StatsModule>
		{
			public:
				/**
				 * A constructor
				 */
				StatsModule();

				/**
				 * @copydoc CModule::~CModule
				 */
				~StatsModule();

				/**
				 * @copydoc CModule::init
				 */
				void init();

				/**
				 * @copydoc CModule::process
				 */
				void process();

				/**
				 * @copydoc CModule::exit
				 */
				void exit();

				/**
				 * You dont have to call this method, it only reserved for the listener
				 * @param time This is the time between 2 frames
				 */
				void setRenderTime(float time);

				/**
				 * This method return the render time between 2 frames
				 * @return the time between 2 frames
				 */
				float getRenderTime();

			private:
				stats_i* m_pStats;
				boost::shared_ptr<StatsModuleListener> m_listenerModule;
		};
	};
};

#endif
