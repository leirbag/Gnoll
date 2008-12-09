#include "../include/cmessagelistenercamera.h"

namespace Gnoll
{
	namespace Scene
	{
		CMessageListenerCamera::CMessageListenerCamera() :
			Gnoll::Core::CMessageListener()
		{
		}

		CMessageListenerCamera::~CMessageListenerCamera()
		{
		}

		void CMessageListenerCamera::setCamera(shared_ptr<Camera> pCamera)
		{
			if(pCamera == NULL)
				return;

			m_pCamera = pCamera;
		}

		void CMessageListenerCamera::handle ( shared_ptr<Gnoll::Core::CMessage> message )
		{
		}
	};
};
