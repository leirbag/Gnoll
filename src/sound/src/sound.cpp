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

#include "../include/sound.h"

#include <iostream>

namespace Gnoll
{
	namespace Sound 
	{
		Sound::Sound()
		{
			alGenBuffers(1, &m_buffer);
		}
		
		void Sound::play()
		{
			/**
			 * Need to make sure everything is up to date
			 */
			update();
		
			/**
			 * Source creation
			 * XXX To update
			 */
			ALuint source;
			alGenSources(1, &source);
			
			ALfloat sourcePos[] = { 0.0, 0.0, 0.0 };
			ALfloat sourceVel[] = { 0.0, 0.0, 0.0 };
			
			alSourcef (source, AL_PITCH,    1.0f     );
			alSourcef (source, AL_GAIN,     1.0f     );
			alSourcefv(source, AL_POSITION, sourcePos);
			alSourcefv(source, AL_VELOCITY, sourceVel);
			// -----------
			
			/**
			 * The sound is played
			 * The sound is also added to the play list
			 */
			alSourcei(source, AL_BUFFER, m_buffer);
			alSourcePlay(source);
			m_source_list.push_back(source);
		}

		/**
		 * Update play list
		 */
		void Sound::update()
		{
			if (m_source_list.empty())
				return;

			ALint status;
			for (unsigned int i = 0; i < m_source_list.size(); i++)
			{
				ALuint t_source = m_source_list[i];
				alGetSourcei(t_source, AL_SOURCE_STATE, &status);
				if (status != AL_PLAYING)
				{
					alSourcei(t_source, AL_BUFFER, 0);
					alDeleteSources(1, &t_source);	
					m_source_list.erase(m_source_list.begin() + i);
					i--;
				}
			}		
		}

		ALuint Sound::getBuffer()
		{
			return m_buffer;
		}

		void Sound::delBuffer()
		{
			alDeleteBuffers(1, &m_buffer);
		}	

		void Sound::setBuffer(ALuint buffer)
		{	
			m_buffer = buffer;
		}

		Sound::~Sound()
		{
			/**
			 * Delete sounds that are being played
			 */
			for (unsigned int i = 0; i < m_source_list.size(); i++)
			{
				alSourcei(m_source_list[i], AL_BUFFER, 0);
				alDeleteSources(1, &(m_source_list[i]));
			}

			/**
			 * Delete audio m_buffer
			 */
			delBuffer();
		}
	}
}
