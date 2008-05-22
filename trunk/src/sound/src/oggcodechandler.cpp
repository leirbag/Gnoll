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


/*---------------------------oggcodechandler.cpp----------------------------*\
|   Codec handler for Ogg files                                              |
|                                                                            |
|   Changelog :                                                              |
|               11/06/2007 - Soax - Initial release                          |
|               02/04/2008 - Bruno Mahe - Update comments                    |
|                                                                            |
\*--------------------------------------------------------------------------*/

#include "../include/oggcodechandler.h"

#include <string.h>

namespace Gnoll 
{
	namespace Sound 
	{
		OggCodecHandler::OggCodecHandler()
		{
		}
	
		
		OggCodecHandler::~OggCodecHandler()
		{
		}

		
		/**
		 * Instanciate a new Sound object from the data extracted from the audio stream
		 */
		shared_ptr<Sound> OggCodecHandler::handle(shared_ptr<IStream> _stream)
		{
			shared_ptr<Sound> _sound(new Sound);
			
			/**
			 * File format
			 */
			ALenum format = 0;

			/**
			 * File sample rate
			 */
			ALsizei sampleRate = 0;
				
			OggVorbis_File ogg_stream;
			
			ov_callbacks vorbisCallbacks;
			vorbisCallbacks.read_func = vorbisRead;
			vorbisCallbacks.close_func = NULL;
			vorbisCallbacks.seek_func = NULL;
			vorbisCallbacks.tell_func = NULL;
		
	
			/**
			 * Create an ogg stream from the input stream
			 */
			int ovOpenCallbacksResult = ov_open_callbacks(_stream.get(), &ogg_stream, NULL, 0, vorbisCallbacks);
				
			if (ovOpenCallbacksResult != 0)
			{
				cout << "Error while calling ov_opencallbacks()" << endl;

				switch (ovOpenCallbacksResult)
				{
					case OV_EREAD:
						cout << "  ->  A read from media returned an error." << endl;
						break;

					case OV_ENOTVORBIS:
						cout << "  ->  Bitstream does not contain any Vorbis data." << endl;
						break;

					case OV_EVERSION:
						cout << "  ->  Vorbis version mismatch." << endl;
						break;

					case OV_EBADHEADER:
						cout << "  ->  Invalid Vorbis bitstream header." << endl;
						break;

					case OV_EFAULT:
						cout << "  ->  Internal logic fault; indicates a bug or heap/stack corruption." << endl;
						break;

					default:
						cout << "  ->  Error unknown." << endl;
						break;

				}

				if (_stream->eof())
				{
					cout << "ERROR : End of OGG stream" << endl;
				}

			}
				
			/**
			 * Get audio stream information
			 */
			vorbis_info* infos = ov_info(&ogg_stream, 0);
				
			if (infos->channels == 1)
				format = AL_FORMAT_MONO16;
			else if (infos->channels == 2)
				format = AL_FORMAT_STEREO16;
				
			sampleRate = infos->rate;
				
				
			
			/**
			 * Get all samples
			 */
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
		
			/**
			 * Fill out OpenAL sound buffer
			 */
			if (size_read > 0){
				
				alBufferData(_sound->getBuffer(), format, file_buffer, size_read, sampleRate);
			}

			/**
			 * Close ogg stream
			 */
			ov_clear(&ogg_stream);
				
			cout << "Sound resource created" << endl;
			return _sound;
		}
			
		
		
		string OggCodecHandler::getFileType()
		{
			return "ogg";
		}
			
			
			
		size_t vorbisRead(void * ptr, size_t size, size_t nmemb, void * datasource)
		{
			/**
			 * Size of the buffer
			 */ 
			const unsigned int maxSizeBuffer = 4096;

			
			char buffer [maxSizeBuffer];
			IStream * stream;
			size_t res;
		
			stream = (IStream *)datasource;

			/**
			 * Nothing to read
			 */
			if (stream->eof()) {
				return 0;
			}

			/**
			 * Number of bytes read
			 */  
			unsigned int i = 0;

			/**
			 * Size of the destination buffer
			 */ 
			unsigned int totalMem = size * nmemb;

			bool endOfStream = false;

			while ((i < totalMem) && (endOfStream == false))
			{
				res = stream->read(buffer, maxSizeBuffer);
			
				if (res == 0)	
				{ 
					endOfStream = true;

				} else {
			
					memcpy((char*)ptr+i, buffer, res);
					i += maxSizeBuffer;

				}
			}				

			return i;	
		}
	}
}
