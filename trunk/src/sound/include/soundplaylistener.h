
#include "../../core/include/cmessagelistener.h"
#include <vector>


#ifndef __SOUNDPLAYLISTENER_H__
#define __SOUNDPLAYLISTENER_H__


namespace Gnoll
{
	namespace Sound
	{
		class SoundPlayListener : public CMessageListener
		{
			private:
				vector<string> * sound_queue;
				
			public:
			
				/**
				* This is a constructor
				*/
				SoundPlayListener(vector<string> *);
		
				/**
				* This is a destructor
				*/
				virtual ~SoundPlayListener() {}
		
				/**
				* This method is called in order to process a message
				* @param message The message this method will have to process
				*/
				virtual void handle ( shared_ptr<CMessage> message );
		};
	}
}
	
#endif
