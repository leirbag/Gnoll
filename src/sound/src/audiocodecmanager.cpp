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


/*-------------------------audiocodecmanager.cpp----------------------------*\
|   The Audio Codec Manager                                                  |
|                                                                            |
|   Changelog :                                                              |
|               11/06/2007 - Soax - Initial release                          |
|               02/04/2008 - Bruno Mahe - Update comments                    |
|                                                                            |
\*--------------------------------------------------------------------------*/


#include "../include/audiocodecmanager.h"
#include "../../log/include/logmodule.h"

namespace Gnoll
{
	namespace Sound 
	{
		AudioCodecManager::AudioCodecManager()
		{
		   /**
			 * Initialize list of AudioCodecHandler
			 */

			// OGG codec
			shared_ptr<AudioCodecHandler> oggAch (new OggCodecHandler);
			this->addAudioCodec(oggAch);

			// Default codec
			/*shared_ptr<AudioCodecHandler> defaultAch (new DefaultCodecHandler);
			this->addAudioCodec(defaultAch);*/
		}
		
		AudioCodecManager::~AudioCodecManager()
		{
		
		}
		
		void AudioCodecManager::addAudioCodec(shared_ptr<AudioCodecHandler> ach)
		{
			list_codec[ach->getFileType()] = ach;
		}
			
		
		shared_ptr<AudioCodecHandler> AudioCodecManager::getAudioCodecHandler(shared_ptr<AbstractStream> _stream, string _instance)
		{
			string type;

			/**
			 * Get audio codec from the file extension
			 */
			size_t pos = _instance.find_last_of('.') + 1;

		
			if ( pos != string::npos )
				type = _instance.substr(pos);
			
			
			if (list_codec.find(type) != list_codec.end())
				return list_codec[type];
			else
			{
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "Codec par defaut (ogg pour le moment)" );
				return list_codec["ogg"];
			}
			
			//return list_codec["ogg"]->handle(_stream);
		}		
		
	}
}
