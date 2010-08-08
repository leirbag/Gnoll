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

#include "../include/graphicmodule.h"

#include <string>

#include <OgreException.h>

#include "../../config.h"
#include "../../log/include/ceguilogger.h"
#include "../../log/include/logmacros.h"
#include "../../core/include/message.h"
#include "../../core/include/messagemodule.h"
#include "../../core/messages/include/messenger.h"
#include "../../core/messages/include/listener.h"
#include "../../core/messages/include/messagetype.h"
#include "../../dynamicobject/include/dynamicobject.h"
#include "../../dynamicobject/include/dynamicobjectmanager.h"

using namespace Ogre;

namespace Gnoll
{
	namespace Graphic
	{
		GraphicModule::GraphicModule() : 
			m_window(0), 
			m_framerendered("GRAPHIC_FRAME_RENDERED")
		{
			unsigned long time = 0;
			m_data = shared_ptr<boost::any>(new boost::any(time));

			m_GUIRenderer = 0;
			m_GUISystem = 0;
			m_rootWindow = 0;
			m_sceneMgr = 0;
		}


		void GraphicModule::loadOgreResourcesPath()
		{
			Gnoll::DynamicObject::DynamicObjectManager* pom;
			pom = Gnoll::DynamicObject::DynamicObjectManager::getInstancePtr();

			// Load resource paths from config file
			shared_ptr<Gnoll::DynamicObject::DynamicObject> resources = pom->load("ogre_resources");

			typedef list< shared_ptr<Gnoll::DynamicObject::AbstractAttribute> >::iterator ListIterator;

			Gnoll::DynamicObject::List attributesNames = resources->getAttributesNames();

			/**
			 * For each resource type (zip, filesystem...)
			 */
			for(ListIterator itAttrs = attributesNames.begin(); itAttrs != attributesNames.end(); itAttrs++)
			{
				if(shared_ptr<Gnoll::DynamicObject::String> attrName = dynamic_pointer_cast<Gnoll::DynamicObject::String>(*itAttrs))
				{
					/**
					 * Get a list of resources for this resource type
					 */
					shared_ptr<Gnoll::DynamicObject::List> listResources;
					listResources = resources->getAttribute<Gnoll::DynamicObject::List>(string(*attrName));
					for(ListIterator itRes = listResources->begin(); itRes != listResources->end(); itRes++)
					{
						/**
						 * Add each resource path
						 */
						if(shared_ptr<Gnoll::DynamicObject::String> res = dynamic_pointer_cast<Gnoll::DynamicObject::String>(*itRes))
							ResourceGroupManager::getSingleton().addResourceLocation(string(*res), (*attrName));
					}
				}
			}

			// Initialise, parse scripts etc
			ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		}

		void GraphicModule::loadOgrePlugins()
		{
			Gnoll::DynamicObject::DynamicObjectManager *pom = Gnoll::DynamicObject::DynamicObjectManager::getInstancePtr();

			// Load plugins paths from config file
			shared_ptr<Gnoll::DynamicObject::DynamicObject> plugins = pom->load("ogre_plugins");

			typedef list< shared_ptr<Gnoll::DynamicObject::AbstractAttribute> >::iterator ListIterator;

			// Get the list of plugins' paths
			shared_ptr<Gnoll::DynamicObject::List> pluginList = plugins->getAttribute<Gnoll::DynamicObject::List>("PluginList");

			// For each plugin path
			for(ListIterator itAttrs = pluginList->begin(); itAttrs != pluginList->end(); itAttrs++)
			{
				if(shared_ptr<Gnoll::DynamicObject::String> path = dynamic_pointer_cast<Gnoll::DynamicObject::String>(*itAttrs))
				{
					string pathStr(*path);
					GNOLL_LOG().logMessage( "Loading Ogre Plugin : " + pathStr);
					m_root->loadPlugin( (*path) );
				}
			}
		}

		void GraphicModule::loadOgreConfig()
		{
			Gnoll::DynamicObject::DynamicObjectManager *pom = Gnoll::DynamicObject::DynamicObjectManager::getInstancePtr();

			// Load ogre config
			shared_ptr<Gnoll::DynamicObject::DynamicObject> config = pom->load("ogre_config");

			// Define and init a RenderSystem
			std::string renderSystemAttrName ("Render System");
			Gnoll::DynamicObject::String gRenderSystemAttrName ( renderSystemAttrName );

			shared_ptr<Gnoll::DynamicObject::String> renderSystemAttrValue;
			renderSystemAttrValue = config->getAttribute<Gnoll::DynamicObject::String>(renderSystemAttrName);
			Ogre::RenderSystem *rs = m_root->getRenderSystemByName(*renderSystemAttrValue);

			if(rs == NULL)
			{
				// Ogre throws an exception if there's no RenderSystem defined
				GNOLL_LOG().logMessage( "ogre_config file doesn't contain any 'Render System' attribute");
				return;
			}

			/**
			 * Loop through all config object attributes (except "Render System") and passes them to Ogre's Render System
			 */
			Gnoll::DynamicObject::List listAttrNames = config->getAttributesNames();
			for(Gnoll::DynamicObject::List::iterator it = listAttrNames.begin(); 
				 it != listAttrNames.end(); it++)
			{
				shared_ptr<Gnoll::DynamicObject::AbstractAttribute> attrNameRaw = *it;
				shared_ptr<Gnoll::DynamicObject::String> attrName;
				attrName = static_pointer_cast<Gnoll::DynamicObject::String>(attrNameRaw);

				if(*attrName != gRenderSystemAttrName)
				{
					shared_ptr<Gnoll::DynamicObject::String> param_value;
					param_value= config->getAttribute< Gnoll::DynamicObject::String >(*attrName);

					// Catch option unknow against to kill the application
					// We don't output anything cause the method do it for
					// us ...
					try
					{
						rs->setConfigOption(*attrName, *param_value);
					}
					catch(const Ogre::Exception& e)
					{
					}
				}
			}

			// Use this RenderSystem
			this->m_root->setRenderSystem(rs);
		}

		void GraphicModule::loadGui()
		{
			/**
			 * Set up a logger for cegui which will redirect CEGUI's logs to Ogre's one
			 */
			// CCEGUILogger* ceguiLogger = new CCEGUILogger();
			m_GUIRenderer = new CEGUI::OgreCEGUIRenderer(m_window, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, m_sceneMgr);
			m_GUISystem = new CEGUI::System(m_GUIRenderer);
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

			GNOLL_LOG().logMessage(string("[GUI]   Loading GUI scheme named "+(string)(*schemeName.get())).c_str());
			GNOLL_LOG().logMessage(string("[GUI]   Loading mouse cursor named "+(string)(*mouseCursor.get())).c_str());
			GNOLL_LOG().logMessage(string("[GUI]   Loading cursor image named "+(string)(*cursorImage.get())).c_str());

			CEGUI::SchemeManager::getSingleton().loadScheme( ((*(schemeName.get()))+Glib::ustring(".scheme")).c_str() );
			m_GUISystem->setDefaultMouseCursor( ((string)(*schemeName.get()) ).c_str(), ((string)(*mouseCursor.get()) ).c_str());
			CEGUI::MouseCursor::getSingleton().setImage(((string)(*schemeName.get()) ).c_str(), ((string)(*cursorImage.get()) ).c_str());

			CEGUI::FontManager::getSingleton().createFont(((string)(*fontName.get()) ).c_str());
			GNOLL_LOG().logMessage(string("[GUI]   Loading GUI layout from "+(string)(*defaultLayoutName.get())).c_str());

			m_rootWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout(((string)(*defaultLayoutName.get()) ).c_str());
			CEGUI::System::getSingleton().setGUISheet(m_rootWindow);

			m_rootWindow->setAlwaysOnTop(true);
		}

		void GraphicModule::init()
		{
			this->m_root = new Root("", "");
			this->loadOgrePlugins();
			this->loadOgreConfig();
			this->m_window = m_root->initialise(true, PACKAGE_STRING);

			m_window->setActive(true);
			m_sceneMgr = m_root->createSceneManager("OctreeSceneManager", GraphicModule::DEFAULT_OGRE_SCENE_MANAGER_NAME());
			m_sceneMgr->setAmbientLight(ColourValue(0.6, 0.6, 0.6));

			/**
			 * Extract all ogre resources location
			 * and add them to Ogre resource manager
			 */
			loadOgreResourcesPath();

			// Create a light
			Light* l = m_sceneMgr->createLight("MainLight");
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

		void GraphicModule::process()
		{
			//m_root->startRendering();
			//! @todo : Read every string defined in the XML GUI layout an replace the variables with their values
			// Get the last FPS and the Tri counts
			float fps = m_root->getRenderTarget(m_root->getAutoCreatedWindow()->getName())->getLastFPS();
			size_t tri = m_root->getRenderTarget(m_root->getAutoCreatedWindow()->getName())->getTriangleCount();
			std::string s("FPS : ");
			s+=Ogre::StringConverter::toString(fps);
			s+='\n';
			s+="TRI : ";
			s+=Ogre::StringConverter::toString(tri);
			CEGUI::String str(s);
			//mEditorGuiSheet->getChild("fpsText")->setText(str);

			m_root->renderOneFrame();
			m_window->update();

			unsigned long newframe = m_timer->getMilliseconds ();

			unsigned long time =newframe - m_lastframe;

			shared_ptr<boost::any> tempTime (
					new boost::any (time)
					);
			shared_ptr<Message> mymessage (new Message(m_framerendered, tempTime));

			Gnoll::Core::MessageModule::getInstancePtr()->getMessageManager()->queueMessage(mymessage);

			m_lastframe = newframe;
		}

		void GraphicModule::exit()
		{
			delete m_timer;
		}

		GraphicModule::~GraphicModule()
		{
			if(m_rootWindow)
				CEGUI::WindowManager::getSingleton().destroyWindow(m_rootWindow);
			if(m_GUISystem)
			{
				delete m_GUISystem;
				m_GUISystem = 0;
			}
			if(m_GUIRenderer)
			{
				delete m_GUIRenderer;
				m_GUIRenderer = 0;
			}

			m_window->removeAllViewports();
			m_sceneMgr->destroyAllCameras();
			m_root->destroySceneManager(m_sceneMgr);
			m_window->destroy();
			m_root->shutdown();
			delete m_root;
		}

		RenderWindow* GraphicModule::getRenderWindow()
		{
			return m_window;
		}

		SceneManager* GraphicModule::getSceneManager()
		{
			return m_sceneMgr;
		}

		size_t GraphicModule::getWindowHandle()
		{
			size_t windowhnd = 0;
			m_window->getCustomAttribute( "WINDOW", &windowhnd );
			return windowhnd;
		}
	};
};
