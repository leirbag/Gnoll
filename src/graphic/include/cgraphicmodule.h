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


/*---------------------------cgraphicmodule.h-------------------------------*\
|   The game graphic module                                                  |
|                                                                            |
|   Changelog :                                                              |
|               06/24/2006 - Paf - Initial release                           |
|               03/25/2007 - Paf - Rename CGraphicModule::getGLXWindow() to  |
|                                   CGraphicModule::getWindowHandle()        |
|                                - CGraphicModule::getWindowHandle() handles |
|                                   now both Windows and GNU/Linux plateforms|
|               04/25/2007 - Paf - Added CEGUI                               |
|               05/09/2007 - Paf - Use boost::any due to a change in CMessage|
|               12/17/2007 - Paf - Add private method loadOgreResourcesPath()|
|               02/15/2008 - Bruno Mahe - Need to keep track of Camera's     |
|                                  address, so it can be freed when exiting  |
|               04/10/2006 - Gabriel - Add namespace Gnoll and Graphic       |
|               02/05/2008 - WT - Add private method loadOgrePlugins()       |
|               10/05/2008 - WT - Add private method loadOgreConfig()        |
|		10/09/2008 - Wetneb - Added Ogre 1.6 compatibility	     |
|                                                                            |
\*--------------------------------------------------------------------------*/



#ifndef __CGRAPHICMODULE_H__
#define __CGRAPHICMODULE_H__

#include "../../core/include/cmodule.h"
#include "../../core/include/singleton.h"
#include "../../core/include/cmessagetype.h"

#include "../../scene/include/camera.h"
#include "../../scene/include/abstractcamerafactory.h"
#include "../../scene/include/camerathirdpersonfactory.h"
#include "../../scene/include/camerafirstpersonfactory.h"
#include "../../scene/include/camerasplinefactory.h"
#include "../../scene/include/camerafreeflyfactory.h"

#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>
#include <Ogre.h>
#include <OgreLogManager.h>
#include <OgrePlatform.h>
#include <OgreTimer.h>

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
		class CGraphicModule: public CModule, public Gnoll::Core::Singleton<CGraphicModule>
		{

			private:

				Ogre::Root *mRoot;
				Ogre::RenderWindow* mwindow;
				Ogre::SceneManager* mSceneMgr;
				unsigned long m_lastframe;
				Ogre::Timer* m_timer;

				//CEGUI::Renderer* mGUIRenderer;
				CEGUI::OgreCEGUIRenderer* mGUIRenderer;
				CEGUI::System* mGUISystem;
				CEGUI::Window* mEditorGuiSheet;

				CMessageType framerendered;
				shared_ptr<boost::any> data;


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

			public:

				/**
				 * A constructor
				 */
				CGraphicModule();

				/**
				 * @copydoc CModule::init
				 */
				virtual void init();

				/**
				 * @copydoc CModule::process
				 */
				virtual void process();

				/**
				 * @copydoc CModule::exit
				 */
				virtual void exit();

				/**
				 * @copydoc CModule::~CModule
				 */
				virtual ~CGraphicModule();


				Ogre::RenderWindow* getRenderWindow();

				Ogre::SceneManager* getSceneManager();

				size_t getWindowHandle();
		};
	};
};

#endif // __CGRAPHICMODULE_H__
