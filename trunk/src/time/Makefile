all:
	make testtimer

testtimer:
	g++ -g -Wall -o tests/testtimer tests/testtimer.cpp ../core/src/cgenericmessagemanager.cpp ../core/src/cmessagetype.cpp src/clinuxtimer.cpp src/ctimemodule.cpp  src/ccreatedelayedeventlistener.cpp src/cdestroydelayedeventlistener.cpp src/ccreateperiodiceventlistener.cpp src/cdestroyperiodiceventlistener.cpp -lboost_thread
