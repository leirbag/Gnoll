/**************************************************************************
*   Copyright (C) 2008 by Leribag, Soax, Paf                              *
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

#ifndef __SOUND_H__
#define __SOUND_H__

#include <vector>

#include <AL/al.h>
#include <AL/alc.h>

using namespace std;

namespace Gnoll 
{
	namespace Sound 
	{
		/**
		 * Sound object
		 */
		class Sound
		{	
			public:
				/**
				 * Constructor
				 */
				Sound();

				/**
				 * Destructor
				 */
				~Sound();

				/**
				 * Play that sound
				 */
				void play();

				/**
				 * Update that sound
				 */
				void update();

				//virtual void stop(int);

				//virtual void pause(int);

				//virtual bool isPlaying(){};

				/**
				 * Accessor to the OpenAL buffer
				 */
				ALuint getBuffer(); 

				/**
				 * Mutator of the OpenAL buffer
				 */
				void setBuffer(ALuint);

				/**
				 * Delete the OpenAL buffer
				 */
				void delBuffer();

			protected:
				/**
				 * OpenAL buffer
				 */
				ALuint m_buffer;	

				/**
				 * List of OpenAL sources
				 */
				vector<ALuint> m_source_list;
		};
	}
}

#endif	
