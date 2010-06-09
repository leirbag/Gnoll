/***************************************************************************
 *   Copyright (C) 2008 by Bruno Mahe                                      *
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

#include "../include/logmodule.h"

#include <iostream>

#include <OgrePlatform.h>
#include <OgreLogManager.h>

#include "../../config.h"

using namespace std;
using namespace Ogre;

namespace Gnoll
{
	namespace Log
	{
		LogModule::LogModule() :
			m_logFileName(DEFAULT_LOG_FILENAME)
		{
		}

		LogModule::~LogModule()
		{
		}

		void LogModule::init()
		{
			Ogre::LogManager* logManager = new Ogre::LogManager::LogManager();
			logManager = NULL;

			/**
			 * Log file creation is deactivated if DO_LOG is not defined
			 * To do so, we have to create a LogManager *before* Ogre's root node is created.
			 * Then a default  (see second parameter of createLog()) log is created and output to
			 * a file deactivated (if last parameter of createLog())
			 */
#if DO_LOG
			Ogre::LogManager::getSingleton().createLog(m_logFileName, true, false, false);
#else

			Ogre::LogManager::getSingleton().createLog("", true, false, true);
#endif
		}

		void LogModule::process()
		{
		}

		void LogModule::exit()
		{
		}

		void LogModule::setLogFileName(string logFileName)
		{
			m_logFileName = logFileName;
		}

		void LogModule::logMessage(string msg, bool logIt)
		{
#if DO_LOG
			if(logIt)
				Ogre::LogManager::getSingleton().logMessage(msg);
			else
			{
				size_t blankPosition = msg.find("\n");
				if(blankPosition != std::string::npos)
				{
					m_strBuffer += msg.substr(0, blankPosition);
					logMessage(m_strBuffer);
					if(blankPosition <= msg.size())
						m_strBuffer = msg.substr(blankPosition + 1);
					else
						m_strBuffer.erase();
				}
				else
					m_strBuffer += msg;
			}
#endif
		}
	}
}
