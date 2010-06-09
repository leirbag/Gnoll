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

#ifndef __AUDIOCODECHANDLER_H__
#define __AUDIOCODECHANDLER_H__

#include <iostream>
#include <map>

#include <boost/shared_ptr.hpp>

#include "../../core/include/singleton.h"
#include "../../core/include/abstractstream.h"
#include "sound.h"

using namespace Gnoll::Core;
using namespace boost;

namespace Gnoll 
{
	namespace Sound 
	{
		/**
		 * Interface for audio codecs handles.
		 * An audio codec handler is a class that will decode audio streams
		 */
		class AudioCodecHandler
		{
			public:
				/**
				 * Constructor
				 */
				AudioCodecHandler() {};

				/**
				 * destructor
				 */
				virtual ~AudioCodecHandler() {};
				
				/**
				 * Handler method
				 * @param _stream Stream to decode
				 * @return Sound object containing the decoded stream
				 */
				virtual shared_ptr<Sound> handle(shared_ptr<AbstractStream> stream) = 0;

				/**
				 * Audio codec that the class is handling (ogg...)
				 * @return A string id to identify the codec handled by this class
				 */
				virtual string getFileType() = 0;
		};
	}
}
		
#endif
