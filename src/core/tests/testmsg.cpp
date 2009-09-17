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


/*------------------------------testmsg.cpp--------------------------------*\
|   This program show how to use messages and a message manager             |
|                                                                           |
|   Changelog :                                                             |
|               05/26/2006 - Paf - Initial release                          |
|               07/08/2007 - Paf - Update                                   |
|               11/16/2007 - Paf - Remove all references to                 |
|                                   CGenericMessageManager                  |
|                                                                           |
\*-------------------------------------------------------------------------*/



#include "../include/cmessagemodule.h"


#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>
#include <iostream>


using namespace boost;
using namespace std;
using namespace Gnoll::Core;


/**
 * An idiot message listener 
 */
class MyMessageListener : public CMessageListener
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
		virtual void handle ( shared_ptr<CMessage> message ) 
		{ 
			/*
			 * A string is embeded in the message and is displayed
			 * A very dirty thing but it works ;)
			 */
			cout << message->getData<string>() << endl ;
		}


};



int main()
{
	// A message manager 
	CMessageModule* messageModule = Gnoll::Core::CMessageModule::getInstancePtr();
	CMessageManager* mymanager = messageModule->getMessageManager();

	// A message type called "string" 
	Messages::MessageType mytype("string");

	// A listener
	shared_ptr<CMessageListener> mylistener(new MyMessageListener);




	/*
	 * We add a listner and send some messages
	 * Each of them are handle by the listener thanks to the message manager
	 */

		mymanager->addListener ( mylistener, mytype );
		cout << "Listener ajoute" << endl;

	shared_ptr<boost::any> texte (new boost::any(string("blablabla..."))) ;

	shared_ptr<CMessage>  mymessage (new CMessage(mytype, texte ));

	try
	{
		mymanager->queueMessage(mymessage);
		cout << "Message ajoute" << endl;

		mymanager->queueMessage(mymessage);
		cout << "Message ajoute" << endl;

		mymanager->queueMessage(mymessage);
		cout << "Message ajoute" << endl;
	}
	catch(...)
	{
	}

	messageModule->processQueue();




	/*
	 * If the listener is deleted, no one care about the message.
	 * The message manager rejects the message since there is no listener 
	 *  which care about this message type
	 */

	try
	{
		mymanager->delListener ( mylistener, mytype );
		cout << "Listener supprime" << endl;

		mymanager->queueMessage(mymessage);
		cout << "Message ajoute" << endl;
	}
	catch(...)
	{
	}

	messageModule->processQueue();

	/*
	 * We re-add the listener, send some messages and abort all of them.
	 */

	try
	{
		mymanager->addListener ( mylistener, mytype );
		cout << "Listener ajoute" << endl;
		mymanager->queueMessage(mymessage);
		cout << "Message ajoute" << endl;

		mymanager->queueMessage(mymessage);
		cout << "Message ajoute" << endl;

		mymanager->abortMessage(mytype, true); 
		cout << "Message annule" << endl;
	}
	catch(...)
	{
	}

	messageModule->processQueue();




	/*
	 * The listener is re-deleted and a message is sent
	* -> No one cares about this message
	 */

	try
	{
		mymanager->delListener ( mylistener, mytype );
		cout << "Listener supprime" << endl;

		mymanager->queueMessage(mymessage);
		cout << "Message ajoute" << endl;
	}
	catch(...)
	{
	}

	messageModule->processQueue();

	// Bye bye 
	cout << "Au revoir !" << endl;
	messageModule->destroy();
	return 0;
}
