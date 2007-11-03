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
|               04/01/2007 - WT  - New Message listener to handle the camera|
|                                   added                                   |
|               04/02/2007 - Paf - Fix the camera message listener. The hero|
|                                   movements no longer depend on the mouse |
|                                - Add vertical movements to the camera     |
|               04/05/2007 - Paf - Add a new constant about the speed of    |
|                                   rotation                                |
|                                  Add smooth turns to the robot            |
|               04/07/2007 - Paf - When a user press OIS::KC_A, it creates  |
|                                   a new robot at a random position        |
|               04/08/2007 - Paf - When a user press OIS::KC_R, it removes  |
|                                   one of the robots                       |
|               05/09/2007 - Paf - Adapt everything to the new interface of |
|                                   CMessage                                |
|               09/30/2007 - Paf - Add Time Module                          |
|                                                                           |
\*-------------------------------------------------------------------------*/



#include "core/include/cmessage.h"
#include "core/include/cmessagetype.h"
#include "core/include/cmessagemanager.h"
#include "core/include/cgenericmessagemanager.h"
#include "core/include/cmessagelistener.h"
#include "core/include/camerafreefly.h"

#include "input/include/coisinputmodule.h"
#include "input/include/cinputmouseevents.h"
#include "graphic/include/cgraphicmodule.h"
#include "time/include/ctimemodule.h"

#include <boost/shared_ptr.hpp>
#include <iostream>
#include <list>
#include <stdlib.h>
#include <time.h>

#include <Ogre.h>
#include <OgreLogManager.h>

using namespace boost;
using namespace std;
using namespace Gnoll::Core;
using namespace Gnoll::Time;

bool done = false;

float vitesse = 14.4;

float vtourne = 30.0;
int tourne = 0;
bool toucheavance = false;
bool toucherecule = false;

string state = "Idle1";

Ogre::Radian direction(0.0);


int objcnt = 0;


/**
 * An idiot message listener 
 */
class MousePressedListener : public CMessageListener
{

	public:
		
		/**
		 * This is a constructor
		 */
		MousePressedListener() {}

		/**
		 * This is a destructor
		 */
		virtual ~MousePressedListener() {}

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
			MouseButton temp ( message->getData<MouseButton>());

			cout << temp << endl;
		}


};




/**
 * An idiot message listener 
 */
class AllMessageListener : public CMessageListener
{

	public:
		
		/**
		 * This is a constructor
		 */
		AllMessageListener() {}

		/**
		 * This is a destructor
		 */
		virtual ~AllMessageListener() {}

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
			MouseEvent event = message->getData<MouseEvent>() ;

			cout << '(' << event.relX 
							<< ", " << event.relY 
							<< ", " << event.relZ 
							<< ", " << event.abX 
							<< ", " << event.abY 
							<< ", " << event.abZ
							<< ", " << event.absOnly << ')'
 
					<< endl;

		}


};


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

			OIS::KeyCode kc = message->getData<OIS::KeyCode>();

			cout << kc << endl;

			if (kc == OIS::KC_Q)
				done = true;

		}


};

using namespace Ogre;

class robotcontroler : public CMessageListener
{

	public:
		
		/**
		 * This is a constructor
		 */
		robotcontroler() {}

		/**
		 * This is a destructor
		 */
		virtual ~robotcontroler() {}

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
			cout << message->getData<OIS::KeyCode>() << endl;

			OIS::KeyCode temp2 = message->getData<OIS::KeyCode>();


			SceneManager* scenetemp = CGraphicModule::getInstancePtr()->getSceneManager();
			Ogre::SceneNode* robotnode = scenetemp->getSceneNode("RobotNode");


/*			if (temp2 == OIS::KC_LEFT) {
				direction -= Degree(5.0);
				robotnode->setOrientation(Quaternion(-Radian(direction), Vector3(0.0, 1.0, 0.0)));
			}

			if (temp2 == OIS::KC_RIGHT) {
				direction += Degree(5.0);
				robotnode->setOrientation(Quaternion(-Radian(direction), Vector3(0.0, 1.0, 0.0)));
			}
*/

			Vector3 pos = robotnode->getPosition();
			pos.y = 2500;
        RaySceneQuery* raySceneQuery = scenetemp->createRayQuery(
            Ray(pos, Vector3::NEGATIVE_UNIT_Y));
			Ray updateRay;
  
        updateRay.setOrigin( pos );
        updateRay.setDirection(Vector3::NEGATIVE_UNIT_Y);
        raySceneQuery->setRay(updateRay);
        RaySceneQueryResult& qryResult = raySceneQuery->execute();
        RaySceneQueryResult::iterator i = qryResult.begin();
        if (i != qryResult.end() && i->worldFragment)
        {
            robotnode->setPosition(pos.x, 
                i->worldFragment->singleIntersection.y , 
                pos.z);
        }


		}
};

void deleteRobot(){
	int tempcnt = --objcnt;
	cout << "B : " << objcnt <<endl;
	if (tempcnt >= 0) {
		Ogre::SceneManager* sceneMgr = CGraphicModule::getInstancePtr()->getSceneManager();
		Ogre::SceneNode *node = sceneMgr->getSceneNode(Ogre::StringConverter::toString(tempcnt));
		node->detachObject(Ogre::StringConverter::toString(tempcnt));
		Ogre::Entity *ent = sceneMgr->getEntity(Ogre::StringConverter::toString(tempcnt));
		sceneMgr->destroyEntity(Ogre::StringConverter::toString(tempcnt));
		sceneMgr->destroySceneNode(Ogre::StringConverter::toString(tempcnt));
		cout << "Objet supprime x: " << tempcnt <<endl;
	}
}

class keydown : public CMessageListener
{

	private:
		SceneManager* scenetemp;


	public:
		
		/**
		 * This is a constructor
		 */
		keydown(): scenetemp(NULL) {}

		/**
		 * This is a destructor
		 */
		virtual ~keydown() {}

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
			cout << message->getData<OIS::KeyCode>() << endl;

			OIS::KeyCode temp2 = message->getData<OIS::KeyCode>();

			// Add a robot at a random position
			if (temp2 == OIS::KC_A) {
				cout << "IN : " << objcnt << endl;
				Ogre::SceneManager* sceneMgr = CGraphicModule::getInstancePtr()->getSceneManager();
				Ogre::Entity *ent = sceneMgr->createEntity( Ogre::StringConverter::toString(objcnt), "ninja.mesh" );
				cout << "F2 : " << objcnt << endl;
				Ogre::SceneNode *node1 = sceneMgr->getRootSceneNode()->createChildSceneNode( Ogre::StringConverter::toString(objcnt));

				cout << "G : " << objcnt << endl;
				bool found = false;
				while(!found) {
					Ogre::Vector3 pos(rand()  % 1500,
							  rand() % 1500,
							rand() % 1500);	

					Ogre::RaySceneQuery* raySceneQuery = sceneMgr->createRayQuery(
            				Ogre::Ray(pos, Ogre::Vector3::NEGATIVE_UNIT_Y));

        				Ogre::Ray updateRay;
        				updateRay.setOrigin(pos);
       					updateRay.setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);
					raySceneQuery->setRay(updateRay);
					Ogre::RaySceneQueryResult& qryResult = raySceneQuery->execute();
					Ogre::RaySceneQueryResult::iterator i = qryResult.begin();
					if (i != qryResult.end() && i->worldFragment) {
						node1->setPosition(pos.x, 
						i->worldFragment->singleIntersection.y + 5, 
						pos.z);
	
						cout << "Objet ajoute : " << objcnt <<endl;
						found = true;
					}
				}
				node1->attachObject( ent );
				ent->setCastShadows( true );
				objcnt++;
				cout << "OUT" << endl;
			}


			if (temp2 == OIS::KC_R && objcnt >= 0) {
				cout << "A : " << objcnt <<endl;
				deleteRobot();
				if (objcnt < 0) {
					objcnt = 0;
				}
			}

			if (temp2 == OIS::KC_LEFT) {
				tourne = -1;
			}
				
			if (temp2 == OIS::KC_RIGHT) {
				tourne = 1;
			}

			if (temp2 == OIS::KC_DOWN)
			{
				toucherecule = true;

				if (scenetemp == NULL)
					scenetemp = CGraphicModule::getInstancePtr()->getSceneManager();
				Entity *ent = scenetemp->getEntity( "Robot" );
				AnimationState * anim = ent->getAnimationState( "Walk" );
   	      anim->setLoop( true );
   	      anim->setEnabled( true );
				state = "Walk";

			}

			if (temp2 == OIS::KC_UP)
			{
				toucheavance = true;

				if (scenetemp == NULL)
					scenetemp = CGraphicModule::getInstancePtr()->getSceneManager();
				Entity *ent = scenetemp->getEntity( "Robot" );
				AnimationState * anim = ent->getAnimationState( "Walk" );
			   	anim->setLoop( true );
				anim->setEnabled( true );
				state = "Walk";

			}


		}
};


class keyup : public CMessageListener
{

	public:
		
		/**
		 * This is a constructor
		 */
		keyup() {}

		/**
		 * This is a destructor
		 */
		virtual ~keyup() {}

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
			cout << message->getData<OIS::KeyCode>() << endl;

			OIS::KeyCode temp2 = message->getData<OIS::KeyCode>();

			if (temp2 == OIS::KC_LEFT) {
				tourne = 0;	
			}

			if (temp2 == OIS::KC_RIGHT) {
				tourne = 0;	
			}

			if (temp2 == OIS::KC_DOWN)
			{
				toucherecule = false;
				state = "Idle1";
			}

			if (temp2 == OIS::KC_UP)
			{
				toucheavance = false;
				state = "Idle1";
			}


		}
};


class AnimationListener : public CMessageListener
{
	private:

		SceneManager* scenetemp;
	

	public:
		
		/**
		 * This is a constructor
		 */
		AnimationListener(): scenetemp(NULL) {}

		/**
		 * This is a destructor
		 */
		virtual ~AnimationListener() {}

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
			unsigned long lasttime = message->getData<unsigned long>();

			if (scenetemp == NULL)
			{
				scenetemp = CGraphicModule::getInstancePtr()->getSceneManager();
			}

			Entity *ent = scenetemp->getEntity( "Robot" );
			AnimationState * anim = ent->getAnimationState( state);
			anim->addTime( lasttime / 1000.0);


			scenetemp = CGraphicModule::getInstancePtr()->getSceneManager();
			Ogre::SceneNode* robotnode = scenetemp->getSceneNode("RobotNode");
			if (tourne == -1) 
			{
				direction -= Degree(vtourne * lasttime / 1000.0);
				robotnode->setOrientation(Quaternion(-Radian(direction), Vector3(0.0, 1.0, 0.0)));
			}
			else if (tourne == 1) 
			{
				direction += Degree(vtourne * lasttime / 1000.0);
				robotnode->setOrientation(Quaternion(-Radian(direction), Vector3(0.0, 1.0, 0.0)));
			}


			if (state == "Walk")
			{


				if (toucheavance)
					robotnode->translate( Ogre::Vector3(0.0f, 0.0f, -1.0f), Ogre::Node::TS_LOCAL );
				else if (toucherecule)
					robotnode->translate( Ogre::Vector3(0.0f, 0.0f, 1.0f), Ogre::Node::TS_LOCAL );


				Vector3 pos = robotnode->getPosition();
				pos.y = 2500;
			    RaySceneQuery* raySceneQuery = scenetemp->createRayQuery(
			    Ray(pos, Vector3::NEGATIVE_UNIT_Y));
				Ray updateRay;
  
				updateRay.setOrigin( pos );
				updateRay.setDirection(Vector3::NEGATIVE_UNIT_Y);
				raySceneQuery->setRay(updateRay);
				RaySceneQueryResult& qryResult = raySceneQuery->execute();
				RaySceneQueryResult::iterator i = qryResult.begin();
			    if (i != qryResult.end() && i->worldFragment)
				{
					robotnode->setPosition(pos.x, 
			       i->worldFragment->singleIntersection.y , 
			       pos.z);
				}
			}
		}
};


int main()
{
	srand ( time(NULL) );

	// A message type called "string" 
	CMessageType mytype("KEYBOARD_KEYDOWN");
	CMessageType mytype2("KEYBOARD_KEYUP");
	CMessageType alltype("MOUSE_MOVED");
	CMessageType mptype("MOUSE_PRESSED");
	CMessageType mrtype("MOUSE_RELEASED");

	CMessageType framerendered("GRAPHIC_FRAME_RENDERED");

	// A listener
	shared_ptr<CMessageListener> mylistener(new MyMessageListener);
	shared_ptr<CMessageListener> mylistener2(new robotcontroler);	
	shared_ptr<CMessageListener> mylistener3(new AnimationListener);	
	shared_ptr<CMessageListener> mylistener4(new keyup);	
	shared_ptr<CMessageListener> mylistener5(new keydown);	
	shared_ptr<CMessageListener> mylistener6(new AllMessageListener);	
	shared_ptr<CMessageListener> mylistener7(new MousePressedListener);
	shared_ptr<CMessageListener> mylistener8(new MousePressedListener);
 
	CGraphicModule* graphicmanager = CGraphicModule::getInstancePtr();
	COISInputModule inputmanager;
	CTimeModule timeModule;


	graphicmanager->init();
	inputmanager.init();
	timeModule.init();

	/*
	 * We add a listner and send some messages
	 * Each of them are handle by the listener thanks to the message manager
	 */
	if (CGenericMessageManager::getInstancePtr()->addListener ( mylistener6, alltype ) == true)
		cout << "Listener ajoute" << endl;

	if (CGenericMessageManager::getInstancePtr()->addListener ( mylistener, mytype ) == true)
		cout << "Listener ajoute" << endl;

	if (CGenericMessageManager::getInstancePtr()->addListener ( mylistener2, mytype ) == true)
		cout << "Listener ajoute" << endl;

	if (CGenericMessageManager::getInstancePtr()->addListener ( mylistener3, framerendered ) == true)
		cout << "Listener ajoute" << endl;

	if (CGenericMessageManager::getInstancePtr()->addListener ( mylistener4, mytype2 ) == true)
		cout << "Listener ajoute" << endl;

	if (CGenericMessageManager::getInstancePtr()->addListener ( mylistener5, mytype ) == true)
		cout << "Listener ajoute" << endl;

	if (CGenericMessageManager::getInstancePtr()->addListener ( mylistener7, mptype ) == true)
		cout << "Listener ajoute" << endl;

	if (CGenericMessageManager::getInstancePtr()->addListener ( mylistener8, mrtype ) == true)
		cout << "Listener ajoute" << endl;

	while (done == false)
	{

		inputmanager.process();
		CGenericMessageManager::getInstancePtr()->process();
		graphicmanager->process();
		timeModule.process();
		
	}






	/*
	 * If the listener is deleted, no one care about the message.
	 * The message manager rejects the message since there is no listener 
	 *  which care about this message type
	 */

	if (CGenericMessageManager::getInstancePtr()->delListener ( mylistener, mytype ) == true)
		cout << "Listener supprime" << endl;

	if (CGenericMessageManager::getInstancePtr()->delListener ( mylistener2, mytype ) == true)
		cout << "Listener supprime" << endl;

	if (CGenericMessageManager::getInstancePtr()->delListener ( mylistener3, framerendered ) == true)
		cout << "Listener supprime" << endl;

	if (CGenericMessageManager::getInstancePtr()->delListener ( mylistener4, mytype2 ) == true)
		cout << "Listener supprime" << endl;

	if (CGenericMessageManager::getInstancePtr()->delListener ( mylistener5, mytype ) == true)
		cout << "Listener supprime" << endl;

	if (CGenericMessageManager::getInstancePtr()->delListener ( mylistener6, alltype ) == true)
		cout << "Listener supprime" << endl;

	if (CGenericMessageManager::getInstancePtr()->delListener ( mylistener7, mptype ) == true)
		cout << "Listener supprime" << endl;

	if (CGenericMessageManager::getInstancePtr()->delListener ( mylistener8, mrtype ) == true)
		cout << "Listener supprime" << endl;

	 
	while(objcnt >=0) {
		deleteRobot();
	}

	timeModule.exit();
	inputmanager.exit();
	graphicmanager->exit();

	// Bye bye 
	cout << "Au revoir !" << endl;
	return 0;
}
