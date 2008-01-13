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


/*---------------------------oggcodechandler.cpp-------------------------------*\
|   The sound module                                                  |
|                                                                            |
|   Changelog :                                                              |
|               11/06/2007 - Soax - Initial release                          |
|                                                                            |
\*--------------------------------------------------------------------------*/

#include "../include/oggcodechandler.h"



namespace Gnoll 
{
	namespace Sound 
	{
		OggCodecHandler::OggCodecHandler(){}
	
		
		OggCodecHandler::~OggCodecHandler(){}

		
		//Créé l'objet Sound et rempli son tampon audio avec le contenu du flux (décodé) -> Retourne l'objet Sound
		shared_ptr<Sound> OggCodecHandler::handle(shared_ptr<IStream> _stream)
		{
			shared_ptr<Sound> _sound(new Sound);
			
			//Format du fichier
			ALenum format = 0;
			//Taux d'échnatillonage du fichier
			ALsizei sampleRate = 0;
				
			OggVorbis_File ogg_stream;
				
			
			ov_callbacks vorbisCallbacks;
			vorbisCallbacks.read_func = vorbisRead;
			vorbisCallbacks.close_func = NULL;
			vorbisCallbacks.seek_func = NULL;
			vorbisCallbacks.tell_func = NULL;
			
			//Créé un flux ogg à partir du flux d'entrée
			ov_open_callbacks(_stream.get(), &ogg_stream, NULL, 0, vorbisCallbacks);
				
				
			//Récupère les infos sur le flux (format audio)
			vorbis_info* infos = ov_info(&ogg_stream, 0);
				
			if (infos->channels)
				format = AL_FORMAT_MONO16;
			else
				format = AL_FORMAT_STEREO16;
				
			sampleRate = infos->rate;
				
				
			
			//Récupère les échantillons (ici l'intégralité)
			ALsizei size_read  = 0;
			ALsizei nb_read = 0;
			char temp_buffer[SIZE_BUFFER];
			char * file_buffer = NULL;
			
			do 
			{
				nb_read = ov_read(&ogg_stream, temp_buffer, SIZE_BUFFER, 0, 2, 1, NULL);
					
				if (nb_read > 0)
				{
					file_buffer = (char *) realloc(file_buffer, size_read + nb_read);
					memcpy(file_buffer + size_read, temp_buffer, nb_read);
					size_read += nb_read;
					//cout <<  "  " << size_read << endl;
				}
			}
			while (nb_read > 0);
		
			// Remplissage du tampon avec les données lues
			if (size_read > 0){
				
				alBufferData(_sound->getBuffer(), format, file_buffer, size_read, sampleRate);
			}
			//Ferme le flux
			ov_clear(&ogg_stream);
				
			cout << "Ressource sonore créée" << endl;
			return _sound;
		}
			
		
		
		string OggCodecHandler::getFileType()
		{
			return "ogg";
		}
			
			
			
		size_t vorbisRead(void * ptr, size_t size, size_t nmemb, void * datasource)
		{
			char buffer[size * nmemb];
			IStream * stream;
			size_t res;
			
			stream = (IStream *)datasource;
			
			if (stream->eof())
				return 0;
			
			res = stream->read(buffer, size * nmemb);
			memcpy(ptr, buffer, res);
				
			return res;	
		}
	}
}
