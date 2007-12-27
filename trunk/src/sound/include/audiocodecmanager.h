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


/*---------------------------audiocodecmanager.h-------------------------------*\
|   The Audio codec manager                                                  |
|                                                                            |
|   Changelog :                                                              |
|               11/060/2007 - Soax - Initial release                          |
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

		class AudioCodecManager : public Singleton<AudioCodecManager>
		{
			
			private :
				
				map<string, shared_ptr<AudioCodecHandler> > list_codec;
				
			public :
				
				AudioCodecManager();
				~AudioCodecManager();
				void addAudioCodec(shared_ptr<AudioCodecHandler>);
				shared_ptr<Sound> decodeStream(shared_ptr<IStream>);
		
		};
	}
}
		
#endif
