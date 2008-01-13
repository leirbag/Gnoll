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


/*-------------------------audiocodecmanager.cpp----------------------------*\
|   The Audio Codec Manager                                                  |
|                                                                            |
|   Changelog :                                                              |
|               11/06/2007 - Soax - Initial release                          |
|                                                                            |
\*--------------------------------------------------------------------------*/


#include "../include/audiocodecmanager.h"

namespace Gnoll
{
	namespace Sound 
	{
		AudioCodecManager::AudioCodecManager()
		{
		        //Initialise la liste des codecHandler
			shared_ptr<AudioCodecHandler> oggAch (new OggCodecHandler);
			this->addAudioCodec(oggAch);
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
			
		
		shared_ptr<Sound> AudioCodecManager::decodeStream(shared_ptr<IStream> _stream)	
		{
			/*string type;
			//Récupère le type du fichier à partir de l'extension
			size_t pos = _instance.find_last_of('.') + 1;
		
			if ( pos != string::npos )
				type = _instance.substr(pos);
			
			
			if (list_codec.find(type) != list_codec.end())
				return list_codec[type]->handle(_stream);
			else
			{
				return list_codec["default"]->handle(_stream);
			}*/
			
			return list_codec["ogg"]->handle(_stream);
		}		
		
	}
}