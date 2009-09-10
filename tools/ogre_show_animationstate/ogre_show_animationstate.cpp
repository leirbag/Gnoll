#include <Ogre.h>
#include <OgreDefaultHardwareBufferManager.h>
#include <string>

int main(int argc, char **argv)
{
		if(argc <= 1 || argc > 2)
		{
			std::cout << "Invalid argument, it sould be : ./ogre_show_animationstate /path/name.mesh !\n";
			return -1;
		}
		
		std::string nameMesh(argv[1]);
		std::string pathMesh;

		int index = nameMesh.size();
		while(nameMesh[index] != '/')
		{
			index--;
			if(index == 0)
			{
				pathMesh = "./";
				break;
			}
		}

		if(index != 0)
			pathMesh = nameMesh.substr(0, index);

		// Init
		Ogre::Root* mRoot = new Ogre::Root();
		new Ogre::DefaultHardwareBufferManager();

		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./", "FileSystem", "General");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(pathMesh, "FileSystem", "General");
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		// load mesh
		Ogre::MeshPtr mesh =
		Ogre::MeshManager::getSingleton().load(nameMesh, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		// create & load a SkeletonInstance
		Ogre::SkeletonInstance* skelInst = new Ogre::SkeletonInstance(mesh->getSkeleton());
		skelInst->load();

		// create & load an AnimationStateSet
		Ogre::AnimationStateSet* animStateSet = new Ogre::AnimationStateSet();
		mesh->_initAnimationState(animStateSet);

		// show animation state
		std::cout << "Animation states : \n";
		Ogre::AnimationStateIterator iter = animStateSet->getAnimationStateIterator();
		while(iter.hasMoreElements())
		{
				Ogre::AnimationState* animationState = iter.getNext();
				std::cout << "\t- " << animationState->getAnimationName() << "\n";
		}

		return 0;
}
