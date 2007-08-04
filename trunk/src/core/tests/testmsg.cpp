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
|                                                                           |
\*-------------------------------------------------------------------------*/



#include "../include/cmessage.h"
#include "../include/cmessagetype.h"
#include "../include/cmessagemanager.h"
#include "../include/cgenericmessagemanager.h"
#include "../include/cmessagelistener.h"


#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>
#include <iostream>


using namespace boost;
using namespace std;

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
	CGenericMessageManager mymanager;

	// A message type called "string" 
	CMessageType mytype("string");

	// A listener
	shared_ptr<CMessageListener> mylistener(new MyMessageListener);




	/*
	 * We add a listner and send some messages
	 * Each of them are handle by the listener thanks to the message manager
	 */

	if (mymanager.addListener ( mylistener, mytype ) == true)
		cout << "Listener ajoute" << endl;

	shared_ptr<boost::any> texte (new boost::any(string("blablabla..."))) ;

	shared_ptr<CMessage>  mymessage (new CMessage(mytype, texte ));

	if (mymanager.queueMessage(mymessage) == true)
		cout << "Message ajoute" << endl;

	if (mymanager.queueMessage(mymessage) == true)
		cout << "Message ajoute" << endl;

	if (mymanager.queueMessage(mymessage) == true)
		cout << "Message ajoute" << endl;

	mymanager.process();




	/*
	 * If the listener is deleted, no one care about the message.
	 * The message manager rejects the message since there is no listener 
	 *  which care about this message type
	 */

	if (mymanager.delListener ( mylistener, mytype ) == true)
		cout << "Listener supprime" << endl;

	if (mymanager.queueMessage(mymessage) == true)
		cout << "Message ajoute" << endl;

	mymanager.process();




	/*
	 * We re-add the listener, send some messages and abort all of them.
	 */

	if (mymanager.addListener ( mylistener, mytype ) == true)
		cout << "Listener ajoute" << endl;
	if (mymanager.queueMessage(mymessage) == true)
		cout << "Message ajoute" << endl;

	if (mymanager.queueMessage(mymessage) == true)
		cout << "Message ajoute" << endl;

	if	(mymanager.abortMessage(mytype, true) == true) 
		cout << "Message annule" << endl;

	mymanager.process();




	/*
	 * The listener is re-deleted and a message is sent
    * -> No one cares about this message
	 */

	if (mymanager.delListener ( mylistener, mytype ) == true)
		cout << "Listener supprime" << endl;

	if (mymanager.queueMessage(mymessage) == true)
		cout << "Message ajoute" << endl;

	mymanager.process();




	// Bye bye 
	cout << "Au revoir !" << endl;
	return 0;
}
