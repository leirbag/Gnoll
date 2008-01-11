bin_PROGRAMS= gnoll

gnoll_SOURCES= src/main.cpp src/input/src/coisinputmodule.cpp src/core/src/cgenericmessagemanager.cpp src/core/src/cmessagetype.cpp src/input/src/coisinputmanager.cpp src/graphic/src/cgraphicmodule.cpp src/graphic/src/cscenefactorystd.cpp src/graphic/src/csceneparser.cpp src/time/src/ccreateperiodiceventlistener.cpp src/time/src/cdestroyperiodiceventlistener.cpp src/time/src/ccreatedelayedeventlistener.cpp src/time/src/cdestroydelayedeventlistener.cpp src/time/src/clinuxtimer.cpp src/time/src/ctimemodule.cpp src/core/src/attributehandlerregistry.cpp src/core/src/filestream.cpp src/core/src/list.cpp src/core/src/string.cpp src/core/src/persistentobject.cpp src/core/src/persistentobjectmanager.cpp src/core/src/sourcefile.cpp src/core/src/camera.cpp src/core/src/camerafreefly.cpp src/core/src/camerafreeflylistener.cpp src/core/src/camerafirstperson.cpp src/core/src/camerafirstpersonlistener.cpp src/core/src/camerathirdperson.cpp  src/core/src/camerathirdpersonlistener.cpp src/core/src/inheritsattributehandler.cpp src/core/src/inherits.cpp src/core/src/cmessagemodule.cpp src/core/src/cameraspline.cpp src/core/src/camerasplinelistener.cpp src/core/src/cpersistentobjectproxy.cpp src/scene/src/cmeshpagerenderer.cpp  src/scene/src/cpage.cpp  src/scene/src/cscenemanager.cpp src/core/src/cpoolthreads.cpp src/core/src/cthreadstopper.cpp src/core/src/cworker.cpp src/sound/src/audiocodecmanager.cpp src/sound/src/csoundmodule.cpp src/sound/src/oggcodechandler.cpp src/sound/src/sound.cpp src/sound/src/soundmanager.cpp src/sound/src/soundplaylistener.cpp src/input/src/cinputeventstranslator.cpp src/input/src/ckeyboardeventstranslator.cpp
AM_LDFLAGS =  `pkg-config OGRE --libs`  `pkg-config OIS --libs`  `pkg-config libxml++-2.6 --libs` `pkg-config CEGUI-OGRE --libs` -lboost_filesystem -lboost_thread -lopenal -lalut -lvorbis -lvorbisfile -lvorbisenc -logg -lsndfile

AM_CPPFLAGS=  `pkg-config OGRE --cflags ` `pkg-config OIS --cflags` `pkg-config libxml++-2.6 --cflags` `pkg-config CEGUI-OGRE --cflags` 


EXTRA_DIST=data/*
	
install-data-am:
	cp -r data/ $(datadir)
