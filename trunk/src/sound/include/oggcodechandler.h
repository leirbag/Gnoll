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


/*---------------------------oggcodechandler.h------------------------------*\
|   The Ogg Codec Handler                                                    |
|                                                                            |
|   Changelog :                                                              |
|               11/06/2007 - Soax - Initial release                          |
|               02/04/2008 - Bruno Mahe - Add some doxygen comments          |
|                                                                            |
\*--------------------------------------------------------------------------*/

#include "audiocodechandler.h"

#include <vorbis/vorbisfile.h>

#ifndef __OGGCODECHANDLER_H__
#define __OGGCODECHANDLER_H__

#define SIZE_BUFFER 2048
using namespace std;
using namespace Gnoll::Core;
using namespace Gnoll::Sound;

namespace Gnoll 
{
	namespace Sound 
	{	

		/**
		 * Custom read function to make vorbislib able to extract data from a shared_ptr<IStream>
		 */
		size_t vorbisRead(void * ptr, size_t size, size_t nmemb, void * datasource);
	
		/**
		 * Audio codec handler able to decode vorbis streams
		 */
		class OggCodecHandler : public AudioCodecHandler
		{
					
			public :

				/**
				 * Constructor
				 */
				OggCodecHandler();

				/**
				 * Destructor
				 */
				virtual ~OggCodecHandler();

				/**
				 * @copydoc AudioCodecHandler::handle
				 */
				virtual shared_ptr<Sound> handle(shared_ptr<IStream>);

				/**
				 * @copydoc AudioCodecHandler::getFileType
				 */
				virtual string getFileType();
		};
	}
}
		
#endif
