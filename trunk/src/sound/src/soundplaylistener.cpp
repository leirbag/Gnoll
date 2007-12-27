#include "../include/soundplaylistener.h"

namespace Gnoll
{
	namespace Sound
	{

		SoundPlayListener::SoundPlayListener(vector< string > * _sound_queue)
		{
			sound_queue = _sound_queue;
		}
		
		void SoundPlayListener::handle( shared_ptr<CMessage> message)
		{
			string instance = message->getData<string>();
			sound_queue->push_back(instance);
		}
	}
}



