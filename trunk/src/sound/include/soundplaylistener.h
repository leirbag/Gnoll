/**************************************************************************
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


/*-------------------------------SoundManager.h----------------------------*\
|   This is basic sound manager with cache ability.                         |
|                                                                           |
|                                                                           |
|   Changelog :                                                             |
|               11/06/2007 - Soax - Initial release                         |
|               02/04/2008 - Bruno Mahe - Add some doxygen comments         |
|                                                                           |
\*-------------------------------------------------------------------------*/




#include "../../core/include/cmessagelistener.h"
#include <vector>


#ifndef __SOUNDPLAYLISTENER_H__
#define __SOUNDPLAYLISTENER_H__

using namespace Gnoll::Core;

namespace Gnoll
{
	namespace Sound
	{

		/**
		 * Listener that will load and play a sound when it receives a message
		 */
		class SoundPlayListener : public CMessageListener
		{
			private:

				/**
				 * List of sound to play
				 */
				vector<string> * sound_queue;
				
			public:
			
				/**
				* This is a constructor
				* @param _sound_queue list of sound used to initialize sound_queue attribute
				*/
				SoundPlayListener(vector<string> * _sound_queue);
		
				/**
				* This is a destructor
				*/
				virtual ~SoundPlayListener() {}
		
				/**
				* This method is called in order to process a message
				* @param message The message this method will have to process
				*/
				virtual void handle ( shared_ptr<CMessage> message );
		};
	}
}
	
#endif
