#include "../include/cameraspline.h"

Gnoll::Core::CameraSpline::CameraSpline(const Glib::ustring& instanceName, Ogre::SceneManager* pSM) : Gnoll::Core::Camera(instanceName, pSM)
{
	Ogre::SceneNode* camNode = pSM->getRootSceneNode()->createChildSceneNode();
	camNode->attachObject(m_ogreCamera);

	m_lenght  = 0;
	m_pTarget = NULL;
	m_pAnim = pSM->createAnimation(instanceName, 10);
	m_pAnim->setInterpolationMode(Ogre::Animation::IM_SPLINE);
	m_pNodeAT = m_pAnim->createNodeTrack(0, camNode);

	m_pAnimState = pSM->createAnimationState(instanceName);

	m_listenerUpdate = shared_ptr<CMessageListener>(new Gnoll::Core::UpdateCameraSplineListener(this));
    CGenericMessageManager::getInstancePtr()->addListener ( m_listenerUpdate, CMessageType("GRAPHIC_FRAME_RENDERED") );
}

void Gnoll::Core::CameraSpline::addPoint(const Ogre::Vector3& vec3, unsigned long frame)
{
	if(frame > m_lenght)
		return;

	Ogre::TransformKeyFrame* key = m_pNodeAT->createNodeKeyFrame(frame);
	key->setTranslate(vec3);
	m_mapAnim[frame] = vec3;
}

const Ogre::Vector3* Gnoll::Core::CameraSpline::getPoint(unsigned long frame)
{
	if(frame > m_lenght)
		return NULL;

	return &m_mapAnim[frame];
}