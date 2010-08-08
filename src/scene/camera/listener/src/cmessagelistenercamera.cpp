#include "../include/cmessagelistenercamera.h"

namespace Gnoll
{
	namespace Scene
	{
		MessageListenerCamera::MessageListenerCamera() :
			Gnoll::Core::Messages::Listener()
		{
		}

		MessageListenerCamera::~MessageListenerCamera()
		{
		}

		void MessageListenerCamera::setCamera(shared_ptr<Camera> pCamera)
		{
			if(pCamera == NULL)
				return;

			m_pCamera = pCamera;
		}

		void MessageListenerCamera::handle ( shared_ptr<Gnoll::Core::Message> message )
		{
		}
	};
};
