all:
	make testfsm
	make testmsg
	make testmt

testfsm:
	g++ -g -Wall -o tests/testfsm tests/testfsm.cpp src/cfsm.cpp

testmsg:
	g++ -g -Wall -o tests/testmsg tests/testmsg.cpp src/cgenericmessagemanager.cpp src/cmessagetype.cpp src/cmessagemodule.cpp -lboost_thread

testmt:
	g++ -g -Wall -o tests/testmt tests/testmt.cpp src/cworker.cpp src/cpoolthreads.cpp src/cthreadstopper.cpp -lboost_thread

