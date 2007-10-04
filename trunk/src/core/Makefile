all:
	make testfsm
	make testmsg
	make testpo

testfsm:
	g++ -g -Wall -o tests/testfsm tests/testfsm.cpp src/cfsm.cpp

testmsg:
	g++ -g -Wall -o tests/testmsg tests/testmsg.cpp src/cgenericmessagemanager.cpp src/cmessagetype.cpp

testpo:
	g++ -g -Wall -o tests/testpo src/persistentobject.cpp tests/testpo.cpp src/sourcefile.cpp src/filestream.cpp src/persistentobjectmanager.cpp src/attributehandlerregistry.cpp src/list.cpp src/string.cpp `pkg-config --cflags --libs libxml++-2.6` `pkg-config --cflags --libs glibmm-2.4`  -lboost_filesystem

