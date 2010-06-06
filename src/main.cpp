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

#include "core/include/isource.h"
#include "dynamicobject/include/dynamicobjectmanager.h"
#include "core/include/sourcefile.h"
#include "core/messages/include/listener.h"

#include "input/include/coisinputmodule.h"
#include "input/include/cinputmouseevents.h"
#include "input/include/cinputeventstranslator.h"
#include "graphic/include/cgraphicmodule.h"
#include "time/include/ctimemodule.h"
#include "sound/include/csoundmodule.h"
#include "scene/include/cscenemanager.h"
#include "scene/include/cameramanager.h"
#include "stats/include/statsmodule.h"
#include "log/include/clogmacros.h"
#include "input/include/ctranslationevents.h"
#include "config.h"

#include <boost/shared_ptr.hpp>
#include <iostream>
#include <list>
#include <stdlib.h>
#include <time.h>
#include <boost/program_options.hpp>

#include <Ogre.h>

#ifdef HAVE_SIGNAL
	#include <signal.h>
#endif

using namespace boost;
using namespace boost::program_options;
using namespace std;
using namespace Gnoll::Core;
using namespace Gnoll::Time;
using namespace Gnoll::Sound;
using namespace Gnoll::Input;
using namespace Gnoll::Scene;
using namespace Gnoll::Graphic;
using namespace Gnoll::DynamicObject;
using namespace Gnoll::Log;
using namespace Gnoll::Stats;


namespace Gnoll
{
	/**
	 * Application class, the main class
	 */
	class Application : public Gnoll::Core::CModule, public Gnoll::Core::Singleton<Application>
	{
		private:
			CLogModule *            logModule;
			CGraphicModule*         graphicmanager;
			COISInputModule         inputmanager;
			CTimeModule*            timeModule;
			CMessageModule*         messageModule;
			CSoundModule *          soundmanager;
			CInputEventsTranslator* inputEventsTranslator;
			StatsModule*           statsModule;


			bool done;

			void analyzeArguments (int argc, char* argv[]);

		public:

			bool getDone() {return done;}

			void setDone(bool _done) {done = _done;}

			void init(int argc, char* argv[]);

			void init();

			void process();

			void exit();

			/**
			 * Callback method when we press the CEGUI::PushButton "Quit"
			 */
			bool quit_OnClick(const CEGUI::EventArgs &args);
	};

	/*
	 * Listener of the application
	 */
	class ApplicationListener : public Gnoll::Core::Messages::Listener
	{
		public:
			/**
			 * This is a constructor
			 */
			ApplicationListener() {}

			/**
			 * This is a destructor
			 */
			~ApplicationListener() {}

			/**
			 * This method is called in order to process a message
			 * @param message The message this method will have to process
			 */
			void handle(MessagePtr receivedMessage);
	};


	//
	//===============================================================================
	// Method of ApplicationListener class
	//===============================================================================
	//

	void ApplicationListener::handle(MessagePtr receivedMessage)
	{
		Gnoll::Input::ActionEvent ae = receivedMessage->getData<Gnoll::Input::ActionEvent>();
		if(ae.action == "APPLICATION_QUIT")
		{
			Gnoll::Application::getInstancePtr()->setDone(1);
		}
	}




	//
	//===============================================================================
	// Method of Application class
	//===============================================================================
	//

	void Application::analyzeArguments (int argc, char* argv[])
	{

		// Declare the supported options.
		options_description desc("Allowed options");
		desc.add_options()
			("help,h", "Produce help message")
			("log,l", value<std::string>(), "Log file name")
			("load-source-directory", value< vector<string> >()->composing(), "Add a directory as a load source")
			("save-source-directory", value< vector<string> >()->composing(), "Add a directory as a save source")
		;

		variables_map vm;
		store(parse_command_line(argc, argv, desc), vm);
		notify(vm);


		/**
		 * If help is required
		 *  -> Display usage
		 *  -> Exit
		 */
		if (vm.count("help"))
		{
			cout << desc;
			::exit(1);
		}


		CLogModule *logModule = CLogModule::getInstancePtr();

		if (vm.count("log"))
		{
			std::string logFile = vm["log"].as<std::string>();

			logModule->setLogFileName(logFile);
		}

		logModule->init();



		DynamicObjectManager *pom = DynamicObjectManager::getInstancePtr();
		SoundManager *soundManager = SoundManager::getInstancePtr();
		CameraManager *cameraManager = CameraManager::getInstancePtr();

		/**
		 * If a loading source has to be added
		 */
		if (vm.count("load-source-directory"))
		{


			vector<string> lsd = vm["load-source-directory"].as< vector<string> >();
			for (vector<string>::iterator it = lsd.begin(); it != lsd.end(); it++)
			{

				GNOLL_LOG() << "Adding new load source directory : \n";
				shared_ptr<ISource> userLoadChannel(new SourceFile(*it, false, 10));
				pom->addLoadSource(userLoadChannel);
				soundManager->addLoadSource(userLoadChannel);
				cameraManager->addLoadSource(userLoadChannel);
			}
		}


		/**
		 * If a saving source has to be added
		 */
		if (vm.count("save-source-directory"))
		{

			vector<string> lsd = vm["save-source-directory"].as< vector<string> >();
			for (vector<string>::iterator it = lsd.begin(); it != lsd.end(); it++)
			{

				GNOLL_LOG() << "Adding new save source directory : \n";
				shared_ptr<ISource> userSaveChannel(new SourceFile( *it, true, 10 ));
				pom->addSaveSource(userSaveChannel);
				soundManager->addSaveSource(userSaveChannel);
				cameraManager->addLoadSource(userSaveChannel);
			}
		}


	}


	void Application::init(int argc, char* argv[])
	{
		done = 0;
		analyzeArguments (argc, argv);
		GNOLL_LOG() << "Arguments analyzed [DONE]\n";
		init();
	}


	void Application::init()
	{
		// The very first thing to do is to add the current directory in DynamicObjectManager's list of repositories
		// In case some modules would need to load some config files
		shared_ptr<ISource> loadChannel(new SourceFile(".", false));
		shared_ptr<ISource> saveChannel(new SourceFile(".", true));

		DynamicObjectManager *pom = DynamicObjectManager::getInstancePtr();
		SoundManager *soundManager = SoundManager::getInstancePtr();

		GNOLL_LOG() << "Adding load/save source for current path to the DynamicObjectManager\n";
		pom->addLoadSource(loadChannel);
		pom->addSaveSource(saveChannel);

		GNOLL_LOG() << "Adding load/save source for current path to the SoundManager\n";
		soundManager->addLoadSource(loadChannel);
		soundManager->addSaveSource(saveChannel);

		/**
		 * Now program options have been parsed,
		 * program is initialized
		 */

		GNOLL_LOG() << "Instanciating modules...\n";
		logModule             = CLogModule::getInstancePtr();
		graphicmanager        = CGraphicModule::getInstancePtr();
		timeModule            = CTimeModule::getInstancePtr();
		messageModule         = CMessageModule::getInstancePtr();
		soundmanager          = CSoundModule::getInstancePtr();
		inputEventsTranslator = CInputEventsTranslator::getInstancePtr();
		statsModule           = StatsModule::getInstancePtr();
		GNOLL_LOG() << "Instanciating modules...[DONE]\n";


		try
		{
			GNOLL_LOG() << "Initializing message module\n";
			messageModule->init();
			GNOLL_LOG() << "Initializing message module [DONE]\n";

			GNOLL_LOG() << "Initializing graphic module\n";
			graphicmanager->init();
			GNOLL_LOG() << "Initializing graphic module [DONE]\n";

			GNOLL_LOG() << "Initializing input manager\n";
			inputmanager.init();
			GNOLL_LOG() << "Initializing input manager [DONE]\n";

			GNOLL_LOG() << "Initializing sound manager\n";
			soundmanager->init();
			GNOLL_LOG() << "Initializing sound manager [DONE]\n";

			GNOLL_LOG() << "Initializing time module\n";
			timeModule->init();
			GNOLL_LOG() << "Initializing time module [DONE]\n";

			GNOLL_LOG() << "Initializing input event translator\n";
			inputEventsTranslator->init();
			GNOLL_LOG() << "Initializing input event translator [DONE]\n";

			GNOLL_LOG() << "Initializing stats module\n";
			statsModule->init();
			GNOLL_LOG() << "Initializing stats module [DONE]\n";
		}
		catch (Glib::ustring str)
		{
			cout << str << endl;
		}

		/*
		 * Define the listener of the application
		 * like the message on quit
		 */
		shared_ptr<Gnoll::ApplicationListener> listenerInput = shared_ptr<Gnoll::ApplicationListener>(
															   new Gnoll::ApplicationListener());
		messageModule->getMessageManager()->addListener ( listenerInput, Messages::MessageType(Gnoll::Input::ACTION_EVENT_TYPE) );

		/**
		 * Define the listener for the button
		 */
		CEGUI::PushButton *button_quit;
		button_quit = (CEGUI::PushButton *) CEGUI::WindowManager::getSingleton().getWindow("buttonQuit");
		button_quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Application::quit_OnClick, this));
	}


	bool Application::quit_OnClick(const CEGUI::EventArgs &args)
	{
		done = 1;
		return done;
	}


	void Application::process()
	{
		CSceneManager* gSceneManager = new CSceneManager("gSceneManager");
		while (!done)
		{
			statsModule->process();
			inputmanager.process();
			inputEventsTranslator->process();
			messageModule->process();
			graphicmanager->process();
			timeModule->process();
			soundmanager->process();
		}
		delete gSceneManager;
	}


	void Application::exit()
	{
		timeModule->exit();
		inputmanager.exit();
		inputEventsTranslator->exit();
		graphicmanager->exit();
		messageModule->exit();
		soundmanager->exit();
		logModule->exit();
		statsModule->exit();

		Gnoll::Stats::StatsModule::destroy();
		CSoundModule::destroy();
		CTimeModule::destroy();
		CInputEventsTranslator::destroy();
		CGraphicModule::destroy();
		CMessageModule::destroy();
		CLogModule::destroy();
		StatsModule::destroy();
		Gnoll::DynamicObject::AttributeHandlerRegistry::destroy();

		std::ostringstream tmpString;
		tmpString << "Size of DO cache before destroying it : " << DynamicObjectManager::getInstancePtr()->getSize();
		GNOLL_LOG() <<  tmpString.str() << "\n";
		DynamicObjectManager::destroy();

		// Bye bye
		GNOLL_LOG() << "Au revoir !\n";
		::exit(0);
	}
};


//
//===============================================================================
// Terminate function when we receive a SIGKILL signal to destroy
// correctly managers (like input)
//===============================================================================
//
#ifdef HAVE_SIGNAL
	void terminate (int param)
	{
		printf ("Terminating program...\n");
		Gnoll::Application::getInstancePtr()->setDone(true);
	}
#endif


//
//===============================================================================
// Start point of the application
//===============================================================================
//
int main(int argc, char* argv[])
{
	srand ( time(NULL) );

	Gnoll::Application* app = Gnoll::Application::getInstancePtr();

	#ifdef HAVE_SIGNAL
		struct sigaction action;
		action.sa_handler = terminate;
	sigaction(SIGKILL, &action, NULL);
		sigaction(SIGTERM, &action, NULL);
	#endif

	app->init(argc, argv);
	app->process();
	app->exit();

	return 0;
}
