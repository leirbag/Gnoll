all:
	make testfsm
	make testmsg
	make testpo
	make testmt

testfsm:
	g++ -g -Wall -o tests/testfsm tests/testfsm.cpp src/cfsm.cpp

testmsg:
	g++ -g -Wall -o tests/testmsg tests/testmsg.cpp src/cgenericmessagemanager.cpp src/cmessagetype.cpp src/cmessagemodule.cpp -lboost_thread

testpo:
	g++ -g -Wall -o tests/testpo src/persistentobject.cpp tests/testpo.cpp src/sourcefile.cpp src/filestream.cpp src/persistentobjectmanager.cpp src/attributehandlerregistry.cpp src/list.cpp src/string.cpp src/inherits.cpp src/inheritsattributehandler.cpp `pkg-config --cflags --libs libxml++-2.6` `pkg-config --cflags --libs glibmm-2.4`  -lboost_filesystem -lboost_thread  `pkg-config OGRE --cflags --libs`  `pkg-config OIS --cflags --libs` `pkg-config CEGUI-OGRE --cflags --libs`

testmt:
	g++ -g -Wall -o tests/testmt tests/testmt.cpp src/cworker.cpp src/cpoolthreads.cpp src/cthreadstopper.cpp -lboost_thread

testcpo:
	g++ -g -Wall -o tests/testcpo src/persistentobject.cpp src/cderivablepersistentobject.cpp tests/testcpo.cpp src/sourcefile.cpp src/filestream.cpp src/persistentobjectmanager.cpp src/attributehandlerregistry.cpp src/list.cpp src/string.cpp src/inherits.cpp src/inheritsattributehandler.cpp `pkg-config --cflags --libs libxml++-2.6` `pkg-config --cflags --libs glibmm-2.4`  -lboost_filesystem -lboost_thread -I /usr/include/OGRE/  `pkg-config OGRE --cflags --libs`  `pkg-config OIS --cflags --libs` `pkg-config CEGUI-OGRE --cflags --libs` ../scene/src/cmeshpagerenderer.cpp ../scene/src/cpage.cpp ../scene/src/cscenemanager.cpp src/cpersistentobjectproxy.cpp ../graphic/src/cgraphicmodule.cpp src/cpoolthreads.cpp src/cworker.cpp src/camerafirstperson.cpp src/camerafreefly.cpp src/cameraspline.cpp src/camerathirdperson.cpp src/cthreadstopper.cpp src/cmessagemodule.cpp ../graphic/src/cscenefactorystd.cpp  ../graphic/src/csceneparser.cpp src/cgenericmessagemanager.cpp src/cmessagetype.cpp
