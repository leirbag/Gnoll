/***************************************************************************
 *   Copyright (C) 2008 by Bruno Mahé and gnoll development team           *
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


/*------------------------------CCEGUILogger--------------------------------*\
|   This is a logger for CEGUI which redirects everything to Ogre's logger   |
|                                                                            |
|   Changelog :                                                              |
|               05/15/2008 - Bruno Mahe - Initial release                    |
|                                                                            |
\*--------------------------------------------------------------------------*/



#include "../include/cceguilogger.h"
#include <OgreLogManager.h>
#include <string>

namespace Gnoll
{

	namespace Graphic
	{

		CCEGUILogger::CCEGUILogger(void)
		{
		}


		CCEGUILogger::~CCEGUILogger(void)
		{
		}


		void CCEGUILogger::logEvent(const CEGUI::String& message, CEGUI::LoggingLevel level)
		{

			/**
			 * This will hold the log message which will be sent to Ogre.
			 * This should look like this :
			 * (LEVEL)	[CEGUI]	MSG
			 */
			std::ostringstream line;


			/**
			 * Converting logging level to string
			 */
			switch(level)
			{
				case CEGUI::Errors:
						line << "(Error)\t";
						break;

				case CEGUI::Standard:
						line << "(InfL1)\t";
						break;

				case CEGUI::Informative:
						line << "(InfL2)\t";
						break;

				case CEGUI::Insane:
						line << "(InfL3)\t";
						break;

				default:
						line << "(Unkwn)\t";
						break;
			}


			/**
			 * This message comes from CEGUI subsystem
			 */
			line << "[CEGUI]\t" << message;


			/**
			 * Finally the processed message is sent to Ogre
			 */
			Ogre::LogManager::getSingleton().logMessage( line.str() );

		}


		void CCEGUILogger::setLogFilename(const CEGUI::String& filename, bool append)
		{

			/**
			 * There is no file to create since everything is redirected to Ogre's logger
			 */

		}

	}

}

