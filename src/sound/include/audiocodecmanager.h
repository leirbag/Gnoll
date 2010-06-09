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

#ifndef __AUDIOCODECMANAGER_H__
#define __AUDIOCODECMANAGER_H__

#include <iostream>
#include <map>

#include <boost/shared_ptr.hpp>

#include "../../core/include/singleton.h"
#include "sound.h"
#include "audiocodechandler.h"
#include "oggcodechandler.h"

namespace Gnoll 
{
	namespace Sound 
	{
		/**
		 * Singleton managing AudioCodecHandler classes
		 */
		class AudioCodecManager : public Singleton<AudioCodecManager>
		{
			public:
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
				void addAudioCodec(shared_ptr<AudioCodecHandler> audioCodecHandler);

				/**
				 * Returns an AudioCodecHandler able to decode a specific audio stream
				 * @param stream Stream to decode
				 * @param streamName Name of the stream (file name...)
				 * @return A shared pointer to an AudioCodecHandler able to decode the audio 
				 * 		   stream passed as a parameter
				 */
				shared_ptr<AudioCodecHandler> getAudioCodecHandler(shared_ptr<AbstractStream> stream, string streamName);

			private :
				/**
				 * Associative container to associate an audio codec to an handler
				 */
				map<string, shared_ptr<AudioCodecHandler> > m_list_codec;
		};
	}
}
		
#endif
