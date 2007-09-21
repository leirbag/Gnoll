all:
	make testfsm
	make testmsg
	make testpo
	make testtimer

testfsm:
	g++ -g -Wall -o tests/testfsm tests/testfsm.cpp src/cfsm.cpp

testmsg:
	g++ -g -Wall -o tests/testmsg tests/testmsg.cpp src/cgenericmessagemanager.cpp src/cmessagetype.cpp

testpo:
	g++ -g -Wall -o tests/testpo src/persistentobject.cpp tests/testpo.cpp src/sourcefile.cpp src/filestream.cpp src/persistentobjectmanager.cpp src/attributehandlerregistry.cpp `pkg-config --cflags --libs libxml++-2.6` `pkg-config --cflags --libs glibmm-2.4`  -lboost_filesystem

testtimer:
	g++ -g -Wall -o tests/testtimer tests/testtimer.cpp src/cgenericmessagemanager.cpp src/cmessagetype.cpp src/clinuxtimer.cpp src/ctimermodule.cpp  src/ccreatetimerlistener.cpp src/cdestroytimerlistener.cpp src/ccreateperiodictimerlistener.cpp src/cdestroyperiodictimerlistener.cpp
