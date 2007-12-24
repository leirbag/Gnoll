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
|		   12/24/2007 - Gabriel - change spline camera to third person      |
|                                 camera                                    |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include "../../core/include/camera.h"
#include "../../core/include/camerafixe.h"
#include "../../core/include/camerafreefly.h"
#include "../../core/include/camerathirdperson.h"
#include "../../core/include/camerafirstperson.h"
#include "../../core/include/cameraspline.h"

#include "../include/cgraphicmodule.h"
#include "../../config.h"

#include "../../core/include/cmessage.h"
#include "../../core/include/cmessagetype.h"
#include "../../core/include/cmessagemanager.h"
#include "../../core/include/cmessagemodule.h"
#include "../../core/include/cmessagelistener.h"

#include "../../core/include/persistentobject.h"
#include "../../core/include/persistentobjectmanager.h"


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

void CGraphicModule::loadOgreResourcesPath()
{

	Gnoll::Core::PersistentObjectManager *pom = Gnoll::Core::PersistentObjectManager::getInstancePtr();

	// Load resource paths from config file
	shared_ptr<Gnoll::Core::PersistentObject> resources = pom->load("ogre_resources");


	typedef list< shared_ptr<IAttribute> >::iterator ListIterator;

	Gnoll::Core::List attributesNames = resources->getAttributesNames();
	
	/**
	 * For each resource type (zip, filesystem...)
	 */
	for (ListIterator itAttrs = attributesNames.begin(); itAttrs != attributesNames.end(); itAttrs++)
	{
		
		if (shared_ptr<Gnoll::Core::String> attrName = dynamic_pointer_cast<Gnoll::Core::String>(*itAttrs))
		{

			/**
			 * Get a list of resources for this resource type
			 */
			shared_ptr< Gnoll::Core::List > listResources = resources->getAttribute< Gnoll::Core::List > (string(*attrName));

			for (ListIterator itRes = listResources->begin(); itRes != listResources->end(); itRes++)
			{

				/**
				 * Add each resource path
				 */
				if (shared_ptr<Gnoll::Core::String> res = dynamic_pointer_cast<Gnoll::Core::String>(*itRes))
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

void CGraphicModule::init()
{
	this->mRoot = new Root("plugins.cfg","display.cfg","log.txt"); 
	this->mRoot->showConfigDialog();
	this->mwindow = mRoot->initialise(true, PACKAGE_STRING);

	mwindow->setActive(true);
	mSceneMgr = mRoot->createSceneManager("TerrainSceneManager", "TSM");

	// Create and configure the camera
	Gnoll::Core::Camera* pCamera = new Gnoll::Core::CameraThirdPerson("camTP");

	//pCamera->setEye(Vector3(780, 25, 590));
	//pCamera->setLookAt(Vector3(0, 10, 0));
	pCamera->setNearDistance(5);
	pCamera->setFarDistance(1000);

	// Create one viewport, entire window
	Viewport* vp = mwindow->addViewport(&pCamera->getOgreCamera());
	vp->setBackgroundColour(ColourValue(0.5,1,0));

	// Alter the camera aspect ratio to match the viewport
	pCamera->setFov( Real(vp->getActualWidth()) / Real(vp->getActualHeight()) );

	mSceneMgr->setAmbientLight( ColourValue( 0.6, 0.6, 0.6 ) );
	mSceneMgr->setShadowTechnique( SHADOWTYPE_TEXTURE_ADDITIVE );
	mSceneMgr->setShadowFarDistance(5000);
	mSceneMgr->setShadowTextureSize(2048);
	mSceneMgr->setShadowTextureCount(4);



	/**
	 * Extract all ogre resources location 
	 * and add them to Ogre resource manager
	 */
	loadOgreResourcesPath();


	ISceneFactory * sf = new CSceneFactoryStd("scene.xml", this->mRoot);
	sf->loadScene();



    // Create a light
	Light* l = mSceneMgr->createLight("MainLight");
    // Accept default settings: point light, white diffuse, just set position
    // NB I could attach the light to a SceneNode if I wanted it to move automatically with
    //  other objects, but I don't
	l->setPosition(699,380,485);
	l->setType( Light::LT_POINT );


	// Infinite far plane?
	if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(RSC_INFINITE_FAR_PLANE))
	{
		pCamera->setFarDistance(0);
	}

	RaySceneQuery* raySceneQuery = mSceneMgr->createRayQuery(
	Ray(pCamera->getEye(), Vector3::NEGATIVE_UNIT_Y));

	Ray updateRay;
	updateRay.setOrigin(pCamera->getEye());
	updateRay.setDirection(Vector3::NEGATIVE_UNIT_Y);
	raySceneQuery->setRay(updateRay);
	RaySceneQueryResult& qryResult = raySceneQuery->execute();
	RaySceneQueryResult::iterator i = qryResult.begin();
	if (i != qryResult.end() && i->worldFragment)
	{
		pCamera->setEye(Vector3(pCamera->getEye().x, 
		i->worldFragment->singleIntersection.y + 5, 
		pCamera->getEye().z));
	}


	Entity *ent1 = mSceneMgr->createEntity( "Robot", "ninja.mesh" );
	SceneNode *node1 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "RobotNode");

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
    }

	//delete raySceneQuery;
	node1->scale(0.2, 0.2, 0.2);
	node1->attachObject( ent1 );
	ent1->setCastShadows( true );
	
	AnimationState * anim = ent1->getAnimationState( "Idle1" );
	anim->setLoop( true );
	anim->setEnabled( true );

	m_timer = new Timer();
	m_lastframe = m_timer->getMilliseconds ();

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
	btn->setPosition(CEGUI::UVector2(CEGUI::UDim(0.035f, 0.0f), CEGUI::UDim(0.035f, 0.0f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.036f)));
	//btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Demo4Sample::handleQuit, this));
	btn->setAlwaysOnTop(true);

	CEGUI::Window* eb = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Editbox","fpsText");
	mEditorGuiSheet->addChildWindow(eb);
	eb->setText("FPS :");
	eb->setPosition(CEGUI::UVector2(cegui_reldim(0.025f), cegui_reldim(0.85f)));
	eb->setSize(CEGUI::UVector2(cegui_absdim(100), cegui_absdim(70)));
	eb->setAlwaysOnTop(true);

	static_cast<Gnoll::Core::CameraThirdPerson*>(pCamera)->setTarget(mSceneMgr->getSceneNode("RobotNode"));
	static_cast<Gnoll::Core::CameraThirdPerson*>(pCamera)->setOffset(100.0f);
	static_cast<Gnoll::Core::CameraThirdPerson*>(pCamera)->setLimitRotationX(180.0f);
	static_cast<Gnoll::Core::CameraThirdPerson*>(pCamera)->setLimitRotationY(180.0f);
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

	if (Gnoll::Core::CMessageModule::getInstancePtr()->getMessageManager()->queueMessage(mymessage) == false)
		cout << "Message pas ajoute !!!" << endl;
	
	m_lastframe = newframe;
}


void CGraphicModule::exit()
{
    delete m_timer;
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

	mwindow->getCustomAttribute( "WINDOW", &windowhnd );

	return windowhnd;
}
