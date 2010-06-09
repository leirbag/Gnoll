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

#include "../include/soundmanager.h"

namespace Gnoll
{
	namespace Sound 
	{
		SoundManager::SoundManager()
		{
		}

		SoundManager::~SoundManager()
		{
		}

		shared_ptr<Sound> SoundManager::loadImpl( shared_ptr<AbstractStream> stream, string instance)
		{	
			shared_ptr<AudioCodecHandler> audioCodecHandler;
			audioCodecHandler = AudioCodecManager::getInstancePtr()->getAudioCodecHandler(stream, instance);

			/**
			 * If an AudioCodecHandler has been found, decode the stream.
			 * Else return NULL
			 */
			if (audioCodecHandler.get())
				return audioCodecHandler->handle(stream);
			else
				return shared_ptr<Sound>();
		}

		bool SoundManager::saveImpl(shared_ptr<AbstractStream> stream, shared_ptr<Sound> obj, string instance)
		{
			return false;	
		}
	}
}
