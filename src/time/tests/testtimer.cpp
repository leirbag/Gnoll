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


/*-----------------------------testtimer.cpp-------------------------------*\
|   This program show how to use messages and a message manager             |
|                                                                           |
|   Changelog :                                                             |
|               08/30/2007 - Paf - Initial release                          |
|               09/23/2007 - Paf - Update class names                       |
|               11/16/2007 - Paf - Remove all references to                 |
|                                   CGenericMessageManager                  |
|                                                                           |
\*-------------------------------------------------------------------------*/



#include "../../core/include/message.h"
#include "../../core/messages/include/messagetype.h"
#include "../../core/messages/include/messenger.h"
#include "../../core/include/messagemodule.h"
#include "../../core/messages/include/listener.h"
#include "../include/timemodule.h"
#include "../include/abstracttimer.h"
#include "../include/clinuxtimer.h"
#include "../include/timermessages.h"


#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>
#include <iostream>


using namespace boost;
using namespace std;
using namespace Gnoll::Time;
using namespace Gnoll::Core;

/**
 * An idiot message listener 
 */
class MyMessageListener : public MessageListener
{

	public:
		
		/**
		 * This is a constructor
		 */
		MyMessageListener() {}

		/**
		 * This is a destructor
		 */
		virtual ~MyMessageListener() {}

		/**
		 * This method is called in order to process a message
		 * @param message The message this method will have to process
		 */
		virtual void handle ( shared_ptr<Message> message ) 
		{ 
			/*
			 * A string is embeded in the message and is displayed
			 */
			cout << message->getData<string>() << endl ;
		}


};



int main()
{

	// A message manager 
	MessageModule* messageModule = MessageModule::getInstancePtr();
	MessageManager* myManager = messageModule->getMessageManager();
	TimeModule* myTimeModule = TimeModule::getInstancePtr();

	// A message type called "string" 
	Messages::MessageType mytype("string");


	// A simple listener
	shared_ptr<MessageListener> mylistener(new MyMessageListener);


	// Initialization of the message module
	messageModule->init();

	// Initialization of the time module
	myTimeModule->init();


	/*
	 * We add a listner and send some messages
	 * Each of them are handle by the listener thanks to the message manager
	 */

	if (myManager->addListener ( mylistener, mytype ) == true)
		cout << "Listener ajoute" << endl;



	// Some texts that will be displayed later on
	shared_ptr<boost::any> texte (new boost::any(string("blablabla..."))) ;
	shared_ptr<boost::any> texte2 (new boost::any(string("bliblublo..."))) ;

	shared_ptr<Message>  mymessage (new Message(mytype, texte ));
	shared_ptr<Message>  mymessage2 (new Message(mytype, texte2 ));

	// In 20 000 milliseconds mymessage will be sent
	myTimeModule->addDelayedEvent(20000, mymessage);

	// Every 1 000 milliseconds my message will be sent. But this will happen in 20 000 millisecondes
	myTimeModule->addPeriodicEvent(20000, mymessage, 1000);



	/**
	 * Now we are going to create a periodic timer.
	 * This timer will send mymessage2 every 2020 milliseconds in 1 second
	 */

	// Message type to create a periodic timer
	Messages::MessageType creation("CREATE_PERIODIC_EVENT");

	// Description of the timer
	TimerPeriodicEvent ev;
	ev.delay = 1000;
	ev.message = mymessage2;
	ev.period = 2020;
	shared_ptr<boost::any> ave(new boost::any(ev));


	shared_ptr<Message> periodicMsg (new Message(creation, ave));	

	// We send a message to ask for the creation of the timer
	if (myManager->queueMessage(periodicMsg))
		cout << "Message enqueued" << endl;




	/**
	 * We are going to create a timer that will send another message to 
	 * destroy the previous periodic timer.
	 * This will happen in 35 seconds
	 */



	// Message type
	Messages::MessageType destroyation("DESTROY_PERIODIC_EVENT");

	// Mesage that will destroy the previous timer :
	// destroyation means we want to destroy one periodic timer
	// ave describes the timer we want to destroy
	shared_ptr<Message> destroyPeriodicMsg (new Message(destroyation, ave));	

	// We want to create a one-shot timer in 35 seconds.
	// This timer will shout the message desztroyPeriodicMsg
	Messages::MessageType simpleTimer("CREATE_DELAYED_EVENT");
	TimerEvent sTimer;
	sTimer.delay = 35000;
	sTimer.message = destroyPeriodicMsg;

	shared_ptr<boost::any> simpleEv(new boost::any(sTimer));

	shared_ptr<Message> destroyMsg (new Message(simpleTimer, simpleEv));	
	
	if (myManager->queueMessage(destroyMsg))
		cout << "Message enqueued" << endl;




	// A simple loop
	for (unsigned long i = 0; i < 999999999; i++)
	{
		messageModule->process();
		myTimeModule->process();
	}


	// Cleaning time !
	if (myManager->delListener ( mylistener, mytype ) == true)
		cout << "Listener supprime" << endl;


	myTimeModule->exit();
	messageModule->exit();



	// Bye bye 
	cout << "Au revoir !" << endl;

	return 0;
}
