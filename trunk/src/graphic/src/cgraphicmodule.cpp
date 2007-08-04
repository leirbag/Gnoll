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
|          03/25/2007 - Paf - Rename CGraphicModule::getGLXWindow() to      |
|                              CGraphicModule::getWindowHandle()            |
|                           - CGraphicModule::getWindowHandle() handles     |
|                              now both Windows and GNU/Linux               |
|                                  plateforms                               |
|          04/01/2007 - WT  - Add a new SceneNode for the camera            |
|          04/01/2007 - Paf - Change the window creation code to display    |
|                              the PACKAGE_STRING constant defined in       |
|                              config.h                                     |
|          04/05/2007 - Paf - Lower the camera's height                     |
|          04/25/2007 - Paf - Added CEGUI objects                           |
|          04/29/2007 - WT  - Add to the gui the current FPS and the        |
|                               number of triangles curently displayed      |
|          05/09/2007 - Paf - Everything related to CMessage has been       |
|                               updated                                     |
|          05/27/2007 - Paf - r40 - Replace SHADOWTYPE_STENCIL_MODULATIVE   |
|                               by SHADOWTYPE_TEXTURE_MODULATIVE            |
|                             r41 - Improvement of the shadows by tweaking  |
|                               some parameters                             |
|                                                                           |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../include/cgraphicmodule.h"
#include "../../config.h"

using namespace Ogre;


CGraphicModule::CGraphicModule() : mwindow(0), framerendered("GRAPHIC_FRAME_RENDERED")
{
/*unsigned int gni = (unsigned int) this;
LogManager::getSingleton().logMessage("GRApHICMANAGER : " + string(gni) );*/
	unsigned long time = 0;
	data = shared_ptr<boost::any>(new boost::any(time));

	mGUIRenderer = 0;
	mGUISystem = 0;
	mEditorGuiSheet = 0;


}


void CGraphicModule::init()
{
	this->mRoot = new Root("plugins.cfg","display.cfg","log.txt"); 

	// Show the configuration dialog and initialise the system
	// You can skip this and use root.restoreConfig() to load configuration
	// settings if you were sure there are valid ones saved in ogre.cfg
	if(mRoot->showConfigDialog())
	{
		// If returned true, user clicked OK so initialise
		// Here we choose to let the system create a default rendering window by passing 'true'
		//mWindow = mRoot->initialise(true);
	}

	this->mwindow = mRoot->initialise(true, PACKAGE_STRING);
//	mWindow->setAutoUpdated(true);
	mwindow->setActive(true);

	mSceneMgr = mRoot->createSceneManager("TerrainSceneManager", "TSM");

	Camera *Camera = mSceneMgr->createCamera("PlayerCam");
	// Position it at 500 in Z direction
	Camera->setPosition(Vector3(-50, 25, 0));
	// Look back along -Z
	Camera->lookAt(Vector3(0, 10, 0));
	Camera->setNearClipDistance(5);
	//Camera->setFarClipDistance(1000);

	// Create one viewport, entire window
	Viewport* vp = mwindow->addViewport(Camera);
	vp->setBackgroundColour(ColourValue(0.5,1,0));


	// Alter the camera aspect ratio to match the viewport
	Camera->setAspectRatio( Real(vp->getActualWidth()) / Real(vp->getActualHeight()) );

	mSceneMgr->setAmbientLight( ColourValue( 0.6, 0.6, 0.6 ) );
	mSceneMgr->setShadowTechnique( SHADOWTYPE_TEXTURE_ADDITIVE );
	mSceneMgr->setShadowFarDistance(5000);
	mSceneMgr->setShadowTextureSize(2048);
	mSceneMgr->setShadowTextureCount(8);

        // Load resource paths from config file
        ConfigFile cf;
        cf.load("resources.cfg");

        // Go through all sections & settings in the file
        ConfigFile::SectionIterator seci = cf.getSectionIterator();

        String secName, typeName, archName;
        while (seci.hasMoreElements())
        {
            secName = seci.peekNextKey();
            ConfigFile::SettingsMultiMap *settings = seci.getNext();
            ConfigFile::SettingsMultiMap::iterator i;
            for (i = settings->begin(); i != settings->end(); ++i)
            {
                typeName = i->first;
                archName = i->second;
                ResourceGroupManager::getSingleton().addResourceLocation(
                    archName, typeName, secName);
            }
        }

// Initialise, parse scripts etc
		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        // Create a skybox
        mSceneMgr->setSkyDome(true, "Examples/CloudySky", 1, 15);//setSkyBox(true, "Examples/SpaceSkyBox", 50 );


        // Create a light
        Light* l = mSceneMgr->createLight("MainLight");
        // Accept default settings: point light, white diffuse, just set position
        // NB I could attach the light to a SceneNode if I wanted it to move automatically with
        //  other objects, but I don't
        l->setPosition(699,380,485);
		l->setType( Light::LT_POINT );

    	    // Fog
        // NB it's VERY important to set this before calling setWorldGeometry 
        // because the vertex program picked will be different
   //     ColourValue fadeColour(0.93, 0.86, 0.76);
   //     mSceneMgr->setFog( FOG_LINEAR, fadeColour, .001, 500, 1000);
   //     mWindow->getViewport(0)->setBackgroundColour(fadeColour);




        std::string terrain_cfg("terrain.cfg");
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
        terrain_cfg = mResourcePath + terrain_cfg;
#endif
        mSceneMgr -> setWorldGeometry( terrain_cfg );
        // Infinite far plane?
        if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(RSC_INFINITE_FAR_PLANE))
        {
            Camera->setFarClipDistance(0);
        }

        // Set a nice viewpoint
        //Camera->setPosition(699,2500,485);
//        Camera->setPosition(699,0,485);
        //Camera->setOrientation(Quaternion(-0.3486, 0.0122, 0.9365, 0.0329));
        //mRoot -> showDebugOverlay( true );

        RaySceneQuery* raySceneQuery = mSceneMgr->createRayQuery(
            Ray(Camera->getPosition(), Vector3::NEGATIVE_UNIT_Y));

        Ray updateRay;
        updateRay.setOrigin(Camera->getPosition());
        updateRay.setDirection(Vector3::NEGATIVE_UNIT_Y);
        raySceneQuery->setRay(updateRay);
        RaySceneQueryResult& qryResult = raySceneQuery->execute();
        RaySceneQueryResult::iterator i = qryResult.begin();
        if (i != qryResult.end() && i->worldFragment)
        {
            Camera->setPosition(Camera->getPosition().x, 
                i->worldFragment->singleIntersection.y + 5, 
                Camera->getPosition().z);
        }

			//delete raySceneQuery;


Entity *ent1 = mSceneMgr->createEntity( "Robot", "robot.mesh" );
	SceneNode *node1 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "RobotNode");
//	ent1->setCastShadows( true );

        raySceneQuery = mSceneMgr->createRayQuery(
            Ray(Vector3(780,2500,590) , Vector3::NEGATIVE_UNIT_Y));

  
        updateRay.setOrigin( Vector3(780,2500,590) );
        updateRay.setDirection(Vector3::NEGATIVE_UNIT_Y);
        raySceneQuery->setRay(updateRay);
        qryResult = raySceneQuery->execute();
        i = qryResult.begin();
        if (i != qryResult.end() && i->worldFragment)
        {
            node1->setPosition(780, 
                i->worldFragment->singleIntersection.y , 
                590);
	//	Ogre::LogManager::getSingleton().logMessage(Ogre::String((float)(i->worldFragment->singleIntersection.y)) );
        }

	//delete raySceneQuery;
	node1->scale(0.2, 0.2, 0.2);
	node1->attachObject( ent1 );
	ent1->setCastShadows( true );
	
	AnimationState * anim = ent1->getAnimationState( "Idle" );
	anim->setLoop( true );
	anim->setEnabled( true );

	m_timer = PlatformManager::getSingleton().createTimer();
	m_lastframe = m_timer->getMilliseconds ();

	SceneNode *node2 = node1->createChildSceneNode( "Camera" );
	node2->attachObject( Camera );


	ISceneFactory * sf = new CSceneFactoryStd("test.xml", this->mRoot);
	sf->loadScene();





	// Set up GUI system
	mGUIRenderer = new CEGUI::OgreCEGUIRenderer(mwindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mSceneMgr);
	mGUISystem = new CEGUI::System(mGUIRenderer);
	CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);

	CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLook.scheme");
	mGUISystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
	CEGUI::MouseCursor::getSingleton().setImage("TaharezLook", "MouseMoveCursor");
	//mGUISystem->setDefaultFont((CEGUI::utf8*)"DejaVuSans-10.font");

	CEGUI::FontManager::getSingleton().createFont("Commonwealth-10.font");


	mEditorGuiSheet= CEGUI::WindowManager::getSingleton().createWindow((CEGUI::utf8*)"DefaultWindow", (CEGUI::utf8*)"Sheet");  
	mGUISystem->setGUISheet(mEditorGuiSheet);

	CEGUI::PushButton* btn = static_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "QuitButton"));
	mEditorGuiSheet->addChildWindow(btn);
	btn->setText("Quit!");
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.035f), cegui_reldim( 0.0f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.036f)));
	//btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Demo4Sample::handleQuit, this));
	btn->setAlwaysOnTop(true);

	CEGUI::Window* eb = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Editbox","fpsText");
	mEditorGuiSheet->addChildWindow(eb);
	eb->setText("FPS :");
	eb->setPosition(CEGUI::UVector2(cegui_reldim(0.025f), cegui_reldim(0.85f)));
	eb->setSize(CEGUI::UVector2(cegui_absdim(100), cegui_absdim(70)));
	eb->setAlwaysOnTop(true);



}


void CGraphicModule::process()
{	
	//mRoot->startRendering();


	// Get the last FPS and the Tri counts
	float fps = mRoot->getRenderTarget(mRoot->getAutoCreatedWindow()->getName())->getLastFPS();
	size_t tri = mRoot->getRenderTarget(mRoot->getAutoCreatedWindow()->getName())->getTriangleCount();
	std::string s("FPS : ");
	s+=Ogre::StringConverter::toString(fps);
	s+='\n';
	s+="TRI : ";
	s+=Ogre::StringConverter::toString(tri);
	CEGUI::String str(s);
	mEditorGuiSheet->getChild("fpsText")->setText(str);



	
	mRoot->renderOneFrame();
//	mWindow->update();


	unsigned long newframe = m_timer->getMilliseconds ();

	unsigned long time =newframe - m_lastframe;
	
	shared_ptr<boost::any> tempTime (
			new boost::any (time)
			);
	shared_ptr<CMessage>  mymessage (new CMessage(framerendered, tempTime));

	if (CGenericMessageManager::getInstance().queueMessage(mymessage) == false)
		cout << "Message pas ajoute !!!" << endl;
	
	m_lastframe = newframe;
}


void CGraphicModule::exit()
{
  PlatformManager::getSingleton().destroyTimer( m_timer );
}


CGraphicModule::~CGraphicModule()
{
	if(mEditorGuiSheet)
	{
		CEGUI::WindowManager::getSingleton().destroyWindow(mEditorGuiSheet);
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

	// Get window handle
	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32

	   mwindow->getCustomAttribute( "HWND", &windowhnd );

	#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX

		mwindow->getCustomAttribute( "GLXWINDOW", &windowhnd );

	#endif

	return windowhnd;
}
