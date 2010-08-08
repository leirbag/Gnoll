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

#ifndef __GRAPHICMODULE_H__
#define __GRAPHICMODULE_H__

#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>
#include <Ogre.h>
#include <OgreLogManager.h>
#include <OgrePlatform.h>
#include <OgreTimer.h>

#include "../../config.h"
#ifndef HAVE_OGRE_1_6 // The following include isn't compatible with Ogre 1.6
//mem probs without this next one
#include <OgreNoMemoryMacros.h>
#endif
#include <CEGUI/CEGUIImageset.h>
#include <CEGUI/CEGUISystem.h>
#include <CEGUI/CEGUILogger.h>
#include <CEGUI/CEGUIFont.h>
#include <CEGUI/CEGUIFontManager.h>
#include <CEGUI/CEGUISchemeManager.h>
#include <CEGUI/CEGUIWindowManager.h>
#include <CEGUI/CEGUIWindow.h>
#include <OGRE/OgreCEGUIRenderer.h>
#include <OGRE/OgreCEGUIResourceProvider.h>
#include <CEGUI/elements/CEGUIPushButton.h>
#ifndef HAVE_OGRE_1_6 // The following include isn't compatible with Ogre 1.6
//regular mem handler
#include <OgreMemoryMacros.h>
#endif

#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#include "../../core/include/module.h"
#include "../../core/include/singleton.h"
#include "../../core/messages/include/messagetype.h"
#include "../../scene/camera/include/camera.h"
#include "../../scene/camera/factory/include/abstractcamerafactory.h"
#include "../../scene/camera/factory/include/camerathirdpersonfactory.h"
#include "../../scene/camera/factory/include/camerafirstpersonfactory.h"
#include "../../scene/camera/factory/include/camerasplinefactory.h"
#include "../../scene/camera/factory/include/camerafreeflyfactory.h"

using namespace std;
using namespace boost;
using namespace Gnoll::Core;

namespace Gnoll
{
	namespace Graphic
	{
		/**
		 *	The game graphic module.
		 */
		class GraphicModule: public Module, public Gnoll::Core::Singleton<GraphicModule>
		{
			public:
				/**
				 * The default ogre scene manager name
				 * @return The default name of the ogre scene manager
				 */
				inline static const char * DEFAULT_OGRE_SCENE_MANAGER_NAME() {return "TSM";}

				/**
				 * A constructor
				 */
				GraphicModule();

				/**
				 * @copydoc Module::~Module
				 */
				virtual ~GraphicModule();

				/**
				 * @copydoc Module::init
				 */
				virtual void init();

				/**
				 * @copydoc Module::process
				 */
				virtual void process();

				/**
				 * @copydoc Module::exit
				 */
				virtual void exit();

				Ogre::RenderWindow* getRenderWindow();

				Ogre::SceneManager* getSceneManager();

				size_t getWindowHandle();
	
			private:
				/**
				 * Load resources path
				 */
				void loadOgreResourcesPath();

				/**
				 * Load plugins path and plugins
				 */
				void loadOgrePlugins();

				/**
				 * Load ogre display config from DO
				 */
				void loadOgreConfig();

				/**
				 * Load GUI layouts from gGUI file
				 */
				void loadGui();

			private:
				Ogre::Root* m_root;

				Ogre::RenderWindow* m_window;

				Ogre::SceneManager* m_sceneMgr;

				unsigned long m_lastframe;

				Ogre::Timer* m_timer;

				CEGUI::OgreCEGUIRenderer* m_GUIRenderer;

				CEGUI::System* m_GUISystem;

				CEGUI::Window* m_rootWindow;

				Messages::MessageType m_framerendered;

				shared_ptr<boost::any> m_data;
		};
	};
};

#endif
