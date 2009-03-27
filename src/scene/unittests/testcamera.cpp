#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../include/camera.h"
#include "../include/camerawrapper.h"

namespace Gnoll
{
	namespace Scene
	{
		class DummyCamera : public Camera
		{
			public:
				DummyCamera(const Glib::ustring& instanceName, shared_ptr<CameraWrapper> wrapper) : Camera(instanceName, wrapper) {}
		};

		class DummyCameraWrapper : public CameraWrapper
		{
			public:
				DummyCameraWrapper(const Glib::ustring& name) : CameraWrapper(name) {}

				void setNearValue(float value) {}

				float getNearValue() { return 0; }

				void setFarValue(float value) {}

				float getFarValue() { return 0; }

				void setFovValue(float value) {}

				float getFovValue() { return 0; }

				void setPosition(const Ogre::Vector3& value) {}

				Ogre::Vector3 getPosition() { return Ogre::Vector3(); }

				void setDirection(const Ogre::Vector3& value) {}

				Ogre::Vector3 getDirection() { return Ogre::Vector3(); }

				void setOrientation(const Ogre::Quaternion& value) {}

				Ogre::Quaternion getOrientation() { return Ogre::Quaternion(); }

				Ogre::Vector3 getUp() { return Ogre::Vector3(); }

				Ogre::Vector3 getRight() { return Ogre::Vector3(); }

				void setAutoTracking(bool autofocus, Ogre::SceneNode* target) {}

				void pitch(Ogre::Radian value) {}

				void yaw(Ogre::Radian value) {}

				void roll(Ogre::Radian value) {}

		};

		BOOST_AUTO_TEST_CASE( PropertyOfCameraWrapper )
		{
			shared_ptr<DummyCameraWrapper> cameraWrapper = shared_ptr<DummyCameraWrapper>(new DummyCameraWrapper("plop"));
			DummyCamera camera("plop", cameraWrapper);

			BOOST_CHECK_EQUAL(cameraWrapper.get(), camera.getCameraWrapper());
		}

		BOOST_AUTO_TEST_CASE( PropertyTarget )
		{
			shared_ptr<DummyCameraWrapper> cameraWrapper = shared_ptr<DummyCameraWrapper>(new DummyCameraWrapper("plop"));
			DummyCamera camera("plop", cameraWrapper);

			camera.setTarget("bob");
			BOOST_CHECK_EQUAL(camera.getTargetName(), "bob");

			camera.setTarget("");
			BOOST_CHECK_EQUAL(camera.getTargetName(), "bob");

			camera.setTarget("marley");
			BOOST_CHECK_EQUAL(camera.getTargetName(), "marley");
		}
	};
};
