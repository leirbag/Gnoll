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


/*--------------------------cgraphicmodule.cpp-----------------------------*\
|   The game graphic module                                                 |
|                                                                           |
|   Changelog :                                                             |
|          06/24/2006 - Paf - Initial release                               |
|                                                                           |
|          03/25/2007 - Paf - Rename CGraphicModule::getGLXWindow() to      |
|                              CGraphicModule::getWindowHandle()            |
|                           - CGraphicModule::getWindowHandle() handles     |
|                              now both Windows and GNU/Linux               |
|                                  plateforms                               |
|                                                                           |
|          04/01/2007 - WT  - Add a new SceneNode for the camera            |
|                                                                           |
|          04/01/2007 - Paf - Change the window creation code to display    |
|                              the PACKAGE_STRING constant defined in       |
|                              config.h                                     |
|                                                                           |
|          04/05/2007 - Paf - Lower the camera's height                     |
|                                                                           |
|          04/25/2007 - Paf - Added CEGUI objects                           |
|                                                                           |
|          04/29/2007 - WT  - Add to the gui the current FPS and the        |
|                               number of triangles curently displayed      |
|                                                                           |
|          05/09/2007 - Paf - Everything related to CMessage has been       |
|                               updated                                     |
|                                                                           |
|          05/27/2007 - Paf - r40 - Replace SHADOWTYPE_STENCIL_MODULATIVE   |
|                               by SHADOWTYPE_TEXTURE_MODULATIVE            |
|                             r41 - Improvement of the shadows by tweaking  |
|                               some parameters                             |
|                                                                           |
|          06/10/2007 - Gabriel - Fix namespace (Viracocha to Gnoll) for    |
|                                 the camera                                |
|                                                                           |
|          06/10/2007 - Gabriel - Add thirdperson camera to test            |
|                                                                           |
|          12/17/2007 - Paf - Put resource path loading in a private method |
|                               and use PersistentObject instead of         |
|                               Ogre resource.cfg to load these resources   |
|                               paths                                       |
|                                                                           |
|          12/21/2007 - Paf - Moved SkyDome and Terrain loading to scene    |
|                               parser                                      |
|                                                                           |
|          12/24/2007 - Gabriel - change spline camera to third person      |
|                                 camera                                    |
|                                                                           |
|          02/15/2008 - Bruno Mahe - Need to keep track of camera's         |
|                                  address, so it can be freed when exiting |
|                                                                           |
|               04/10/2006 - Gabriel - Add namespace Gnoll and Graphic      |
|                                                                           |
|          02/05/2008 - WT - Put plugins path loading in a private method   |
|                               and use DynamicObject instead of            |
|                               Ogre plugins.cfg to load these plugins      |
|                                                                           |
|          10/05/2008 - WT - Put Ogre config loading in a private method    |
|                               and use DynamicObject instead of Ogre       |
|                               display.cfg to load the Renderer config     |
|                                                                           |
|          07/05/2009 - Wetneb - Added .layout support and so on            |
|									    |
\*-------------------------------------------------------------------------*/

#include "../include/cgraphicmodule.h"
#include "../../config.h"

#include "../../core/include/cmessage.h"
#include "../../core/include/cmessagetype.h"
#include "../../core/include/cmessagemanager.h"
#include "../../core/include/cmessagemodule.h"
#include "../../core/include/cmessagelistener.h"

#include "../../scene/include/camerafreefly.h"
#include "../../scene/include/camerafirstperson.h"
#include "../../scene/include/cameraspline.h"

#include "../../dynamicobject/include/dynamicobject.h"
#include "../../dynamicobject/include/dynamicobjectmanager.h"

#include "../include/cceguilogger.h"

#include <string>

using namespace Ogre;

namespace Gnoll
{
	namespace Graphic
	{
		CGraphicModule::CGraphicModule() : mwindow(0), framerendered("GRAPHIC_FRAME_RENDERED")
		{
			unsigned long time = 0;
			data = shared_ptr<boost::any>(new boost::any(time));

			mGUIRenderer = 0;
			mGUISystem = 0;
			mRootWindow = 0;
			mSceneMgr = 0;
		}


		void CGraphicModule::loadOgreResourcesPath()
		{

			Gnoll::DynamicObject::DynamicObjectManager *pom = Gnoll::DynamicObject::DynamicObjectManager::getInstancePtr();

			// Load resource paths from config file
			shared_ptr<Gnoll::DynamicObject::DynamicObject> resources = pom->load("ogre_resources");


			typedef list< shared_ptr<Gnoll::DynamicObject::IAttribute> >::iterator ListIterator;

			Gnoll::DynamicObject::List attributesNames = resources->getAttributesNames();

			/**
			 * For each resource type (zip, filesystem...)
			 */
			for (ListIterator itAttrs = attributesNames.begin(); itAttrs != attributesNames.end(); itAttrs++)
			{

				if (shared_ptr<Gnoll::DynamicObject::String> attrName = dynamic_pointer_cast<Gnoll::DynamicObject::String>(*itAttrs))
				{

					/**
					 * Get a list of resources for this resource type
					 */
					shared_ptr< Gnoll::DynamicObject::List > listResources = resources->getAttribute< Gnoll::DynamicObject::List > (string(*attrName));

					for (ListIterator itRes = listResources->begin(); itRes != listResources->end(); itRes++)
					{

						/**
						 * Add each resource path
						 */
						if (shared_ptr<Gnoll::DynamicObject::String> res = dynamic_pointer_cast<Gnoll::DynamicObject::String>(*itRes))
						{
						ResourceGroupManager::getSingleton().addResourceLocation(
						 string(*res), (*attrName));
						}
					}
				}
			}


			// Initialise, parse scripts etc
			ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		}


		void CGraphicModule::loadOgrePlugins()
		{
			Gnoll::DynamicObject::DynamicObjectManager *pom = Gnoll::DynamicObject::DynamicObjectManager::getInstancePtr();

			// Load plugins paths from config file
			shared_ptr<Gnoll::DynamicObject::DynamicObject> plugins = pom->load("ogre_plugins");

			typedef list< shared_ptr<Gnoll::DynamicObject::IAttribute> >::iterator ListIterator;

			// Get the list of plugins' paths
			shared_ptr<Gnoll::DynamicObject::List> pluginList = plugins->getAttribute<Gnoll::DynamicObject::List>("PluginList");

			// For each plugin path
			for (ListIterator itAttrs = pluginList->begin(); itAttrs != pluginList->end(); itAttrs++)
				if (shared_ptr<Gnoll::DynamicObject::String> path = dynamic_pointer_cast<Gnoll::DynamicObject::String>(*itAttrs))
				{
					string pathStr(*path);
					Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "Loading Ogre Plugin : " + pathStr);
					mRoot->loadPlugin( (*path) );
				}
		}


		void CGraphicModule::loadOgreConfig()
		{
			Gnoll::DynamicObject::DynamicObjectManager *pom = Gnoll::DynamicObject::DynamicObjectManager::getInstancePtr();

			// Load ogre config
			shared_ptr<Gnoll::DynamicObject::DynamicObject> config = pom->load("ogre_config");

			// Define and init a RenderSystem
			std::string renderSystemAttrName ("Render System");
			Gnoll::DynamicObject::String gRenderSystemAttrName ( renderSystemAttrName );

			shared_ptr<Gnoll::DynamicObject::String> renderSystemAttrValue = config->getAttribute<Gnoll::DynamicObject::String>(renderSystemAttrName);
			Ogre::RenderSystem *rs = mRoot->getRenderSystemByName(*renderSystemAttrValue);

			if(rs == NULL)
			{
				// Ogre throws an exception if there's no RenderSystem defined
				Gnoll::Log::CLogModule::getInstancePtr()->logMessage( "ogre_config file doesn't contain any 'Render System' attribute");
				return;
			}

			/**
			 * Loop through all config object attributes (except "Render System") and passes them to Ogre's Render System
			 */
			Gnoll::DynamicObject::List listAttrNames = config->getAttributesNames();
			for (Gnoll::DynamicObject::List::iterator it = listAttrNames.begin(); it != listAttrNames.end(); it++)
			{
				shared_ptr<Gnoll::DynamicObject::IAttribute> attrNameRaw = *it;
				shared_ptr<Gnoll::DynamicObject::String> attrName = static_pointer_cast<Gnoll::DynamicObject::String>(attrNameRaw);


				if (*attrName != gRenderSystemAttrName)
				{
					shared_ptr<Gnoll::DynamicObject::String> param_value = config->getAttribute< Gnoll::DynamicObject::String >(*attrName);
					rs->setConfigOption(*attrName, *param_value);
				}

			}

			// Use this RenderSystem
			this->mRoot->setRenderSystem(rs);
		}

		void CGraphicModule::loadGui()
		{
			/**
			 * Set up a logger for cegui which will redirect CEGUI's logs to Ogre's one
			 */
			CCEGUILogger* ceguiLogger = new CCEGUILogger();
			mGUIRenderer = new CEGUI::OgreCEGUIRenderer(mwindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mSceneMgr);
			mGUISystem = new CEGUI::System(mGUIRenderer);
			CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);

			/**
			 * Set up the GUI environment using the gGUI file
			 */
			Gnoll::DynamicObject::DynamicObjectManager *pom = Gnoll::DynamicObject::DynamicObjectManager::getInstancePtr();
			shared_ptr<Gnoll::DynamicObject::DynamicObject> guiConfig = pom->load("gGUI");


			shared_ptr<Gnoll::DynamicObject::String> schemeName = guiConfig->getAttribute<Gnoll::DynamicObject::String>("scheme");
			shared_ptr<Gnoll::DynamicObject::String> mouseCursor = guiConfig->getAttribute<Gnoll::DynamicObject::String>("mouseCursor");
			shared_ptr<Gnoll::DynamicObject::String> cursorImage = guiConfig->getAttribute<Gnoll::DynamicObject::String>("cursorImage");
			shared_ptr<Gnoll::DynamicObject::String> fontName = guiConfig->getAttribute<Gnoll::DynamicObject::String>("defaultFont");
			shared_ptr<Gnoll::DynamicObject::String> defaultLayoutName = guiConfig->getAttribute<Gnoll::DynamicObject::String>("defaultLayout");

			Gnoll::Log::CLogModule::getInstancePtr()->logMessage(string("[GUI]   Loading GUI scheme named "+(string)(*schemeName.get())).c_str());
			Gnoll::Log::CLogModule::getInstancePtr()->logMessage(string("[GUI]   Loading mouse cursor named "+(string)(*mouseCursor.get())).c_str());
			Gnoll::Log::CLogModule::getInstancePtr()->logMessage(string("[GUI]   Loading cursor image named "+(string)(*cursorImage.get())).c_str());

			CEGUI::SchemeManager::getSingleton().loadScheme( ((*(schemeName.get()))+Glib::ustring(".scheme")).c_str() );
			mGUISystem->setDefaultMouseCursor( ((string)(*schemeName.get()) ).c_str(), ((string)(*mouseCursor.get()) ).c_str());
			CEGUI::MouseCursor::getSingleton().setImage(((string)(*schemeName.get()) ).c_str(), ((string)(*cursorImage.get()) ).c_str());

			CEGUI::FontManager::getSingleton().createFont(((string)(*fontName.get()) ).c_str());
			Gnoll::Log::CLogModule::getInstancePtr()->logMessage(string("[GUI]   Loading GUI layout from "+(string)(*defaultLayoutName.get())).c_str());

			mRootWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout(((string)(*defaultLayoutName.get()) ).c_str());
			CEGUI::System::getSingleton().setGUISheet(mRootWindow);

			mRootWindow->setAlwaysOnTop(true);

		}

		void CGraphicModule::init()
		{
			this->mRoot = new Root("","");
			this->loadOgrePlugins();
			this->loadOgreConfig();
			this->mwindow = mRoot->initialise(true, PACKAGE_STRING);

			mwindow->setActive(true);
			mSceneMgr = mRoot->createSceneManager("OctreeSceneManager", CGraphicModule::DEFAULT_OGRE_SCENE_MANAGER_NAME());
			mSceneMgr->setAmbientLight( ColourValue( 0.6, 0.6, 0.6 ) );


			/**
			 * Extract all ogre resources location
			 * and add them to Ogre resource manager
			 */
			loadOgreResourcesPath();



			// Create a light
			Light* l = mSceneMgr->createLight("MainLight");
			// Accept default settings: point light, white diffuse, just set position
			// NB I could attach the light to a SceneNode if I wanted it to move automatically with
			//  other objects, but I don't
			l->setPosition(699,380,485);
			l->setType( Light::LT_POINT );

			m_timer = new Timer();
			m_lastframe = m_timer->getMilliseconds ();

			// -----------  Set up GUI system -----------

			this->loadGui();

		}

		void CGraphicModule::process()
		{
			//mRoot->startRendering();

			//! @todo : Read every string defined in the XML GUI layout an replace the variables with their values


			// Get the last FPS and the Tri counts
			float fps = mRoot->getRenderTarget(mRoot->getAutoCreatedWindow()->getName())->getLastFPS();
			size_t tri = mRoot->getRenderTarget(mRoot->getAutoCreatedWindow()->getName())->getTriangleCount();
			std::string s("FPS : ");
			s+=Ogre::StringConverter::toString(fps);
			s+='\n';
			s+="TRI : ";
			s+=Ogre::StringConverter::toString(tri);
			CEGUI::String str(s);
			//mEditorGuiSheet->getChild("fpsText")->setText(str);

			mRoot->renderOneFrame();
		//	mWindow->update();


			unsigned long newframe = m_timer->getMilliseconds ();

			unsigned long time =newframe - m_lastframe;

			shared_ptr<boost::any> tempTime (
					new boost::any (time)
					);
			shared_ptr<CMessage>  mymessage (new CMessage(framerendered, tempTime));

			Gnoll::Core::CMessageModule::getInstancePtr()->getMessageManager()->queueMessage(mymessage);

			m_lastframe = newframe;
		}


		void CGraphicModule::exit()
		{
			delete m_timer;
		}


		CGraphicModule::~CGraphicModule()
		{

			if(mRootWindow)
			{
				CEGUI::WindowManager::getSingleton().destroyWindow(mRootWindow);
			}
			if(mGUISystem)
			{
				delete mGUISystem;
				mGUISystem = 0;
			}
			if(mGUIRenderer)
			{
				delete mGUIRenderer;
				mGUIRenderer = 0;
			}


			mwindow->removeAllViewports();
			mSceneMgr->destroyAllCameras();
			mRoot->destroySceneManager(mSceneMgr);
			mwindow->destroy();
			mRoot->shutdown();
			delete mRoot;

		}

		RenderWindow* CGraphicModule::getRenderWindow()
		{
			return mwindow;
		}

		SceneManager* CGraphicModule::getSceneManager()
		{
			return mSceneMgr;
		}



		size_t CGraphicModule::getWindowHandle()
		{
			size_t windowhnd = 0;

			mwindow->getCustomAttribute( "WINDOW", &windowhnd );

			return windowhnd;
		}
	};
};
