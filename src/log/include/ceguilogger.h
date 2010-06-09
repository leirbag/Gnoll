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

#ifndef __CEGUILOGGER__H_
#define __CEGUILOGGER__H_

#include <CEGUI/CEGUILogger.h>

namespace Gnoll
{
	namespace Graphic
	{
		/**
		 * @brief
		 * Implementation for the Logger class which redirects messages to Ogre's logger.
		 */
		class CEGUILogger : public CEGUI::Logger
		{
			public:
				/**
				 * @brief
				 * Constructor for CEGUILogger object.
				 */
				CEGUILogger();

				/**
				 * @brief
				 * Constructor for CEGUILogger object.
				 */
				virtual ~CEGUILogger();

				/**
				 * @brief
				 * Add an event to the log.
				 *
				 * @param message	String object containing the message to be added to the event log.
				 * @param level LoggingLevel for this message.  If \a level is greater than the current 
				 * set logging level, the message is not logged.
				 * @return Nothing
				 */
				virtual void logEvent(const CEGUI::String& message, CEGUI::LoggingLevel level = CEGUI::Standard);

				/**
				 * @brief
				 * Set the name of the log file where all subsequent log entries should be written.
				 *
				 * @param filename Name of the file to put log messages.
				 * @param append
				 *  - true if events should be added to the end of the current file.
				 *  - false if the current contents of the file should be discarded.
				 */
				virtual void setLogFilename(const CEGUI::String& filename, bool append = false);
		};
	}
}

#endif
