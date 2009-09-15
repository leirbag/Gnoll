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


/*----------------------------CTranslationEvents----------------------------*\
|   Translation event                                                        |
|                                                                            |
|   Changelog :                                                              |
|               01/09/2008 - Paf - Initial release                           |
|               01/11/2008 - Paf - Renamed file to ctranslationevents.h      |
|                                                                            |
\*--------------------------------------------------------------------------*/


#ifndef __CTRANSLATIONEVENTS_H__
#define __CTRANSLATIONEVENTS_H__

#include <string>

namespace Gnoll
{

	namespace Input
	{


		/**
		 * Message type for action events
		 */
		extern const std::string ACTION_EVENT_TYPE;
		extern const std::string ACTION_EVENT_STATE_TYPE;


		/**
		 * Action events
		 */
		struct ActionEvent
		{
			/**
			 * Action type
			 */
			std::string action;

			/**
			 * Intensity of the action
			 */
			float intensity;

			/**
			 * Constructor
			 */
			ActionEvent( std::string _action, float _intensity);

		};

	}
}


#endif // __CTRANSLATIONEVENTS_H__
