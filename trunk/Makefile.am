bin_PROGRAMS= gnoll
gnoll_SOURCES= src/main.cpp src/input/src/coisinputmodule.cpp src/core/src/cgenericmessagemanager.cpp src/core/src/cmessagetype.cpp src/input/src/coisinputmanager.cpp src/graphic/src/cgraphicmodule.cpp src/graphic/src/cscenefactorystd.cpp src/graphic/src/csceneparser.cpp src/time/src/ccreateperiodiceventlistener.cpp src/time/src/cdestroyperiodiceventlistener.cpp src/time/src/ccreatedelayedeventlistener.cpp src/time/src/cdestroydelayedeventlistener.cpp src/time/src/clinuxtimer.cpp src/time/src/ctimemodule.cpp
AM_LDFLAGS =  `pkg-config OGRE --libs`  -lOIS  `pkg-config libxml++-2.6 --libs` `pkg-config CEGUI-OGRE --libs`  
AM_CPPFLAGS=  `pkg-config OGRE --cflags `  `pkg-config libxml++-2.6 --cflags` -I/usr/local/include/OIS `pkg-config CEGUI-OGRE --cflags`  
