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


/*---------------------------audiocodecmanager.h----------------------------*\
|   The Audio codec manager                                                  |
|                                                                            |
|   Changelog :                                                              |
|               11/06/2007 - Soax - Initial release                          |
|               02/04/2008 - Bruno Mahe - Add some doxygen comments          |
|                                                                            |
\*--------------------------------------------------------------------------*/

#include "../../core/include/singleton.h"
#include "sound.h"
#include "audiocodechandler.h"
#include "oggcodechandler.h"
//#include "../include/defaultcodechandler.h"

#include <boost/shared_ptr.hpp>
#include <iostream>
#include <map>


#ifndef __AUDIOCODECMANAGER_H__
#define __AUDIOCODECMANAGER_H__

namespace Gnoll {
	namespace Sound {

		/**
		 * Singleton managing AudioCodecHandler classes
		 */
		class AudioCodecManager : public Singleton<AudioCodecManager>
		{
			
			private :
				
				/**
				 * Associative container to associate an audio codec to an handler
				 */
				map<string, shared_ptr<AudioCodecHandler> > list_codec;
				
			public :
				
				/**
				 * Constructor
				 */
				AudioCodecManager();

				/**
				 * Destructor
				 */
				~AudioCodecManager();

				/**
				 * Add a new codec handler
				 * @param _audiocodecHandler Audio codec to add
				 */
				void addAudioCodec(shared_ptr<AudioCodecHandler> _audioCodecHandler);

				/**
				 * Decode an audio stream and return a shared pointer to a Sound object
				 * @param _stream Stream to decode
				 * @param _streamName Name of the stream (file name...)
				 * @return A shared pointer to a newly created Sound object from the decoded audio stream
				 */
				shared_ptr<Sound> decodeStream(shared_ptr<IStream> _stream, string _streamName);
		
		};
	}
}
		
#endif
