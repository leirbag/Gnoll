#include "../include/cmessagelistenercamera.h"

namespace Gnoll
{
	namespace Core
	{
		CMessageListenerCamera::CMessageListenerCamera() :
			CMessageListener()
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

		void CMessageListenerCamera::handle ( shared_ptr<CMessage> message )
		{
		}
	};
};
