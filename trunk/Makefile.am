bin_PROGRAMS= gnoll

gnoll_SOURCES= src/main.cpp src/input/src/coisinputmodule.cpp src/core/src/cgenericmessagemanager.cpp src/core/src/cmessagetype.cpp src/input/src/coisinputmanager.cpp src/graphic/src/cgraphicmodule.cpp src/graphic/src/cscenefactorystd.cpp src/graphic/src/csceneparser.cpp src/time/src/ccreateperiodiceventlistener.cpp src/time/src/cdestroyperiodiceventlistener.cpp src/time/src/ccreatedelayedeventlistener.cpp src/time/src/cdestroydelayedeventlistener.cpp src/time/src/clinuxtimer.cpp src/time/src/ctimemodule.cpp src/core/src/attributehandlerregistry.cpp src/core/src/filestream.cpp src/core/src/list.cpp src/core/src/string.cpp src/core/src/persistentobject.cpp src/core/src/persistentobjectmanager.cpp src/core/src/sourcefile.cpp src/core/src/camera.cpp src/core/src/camerafreefly.cpp src/core/src/camerafreeflylistener.cpp src/core/src/camerafirstperson.cpp src/core/src/camerafirstpersonlistener.cpp src/core/src/camerathirdperson.cpp  src/core/src/camerathirdpersonlistener.cpp src/core/src/inheritsattributehandler.cpp src/core/src/inherits.cpp src/core/src/cmessagemodule.cpp src/core/src/cameraspline.cpp src/core/src/camerasplinelistener.cpp src/core/src/cpersistentobjectproxy.cpp src/scene/src/cmeshpagerenderer.cpp  src/scene/src/cpage.cpp  src/scene/src/cscenemanager.cpp src/core/src/cpoolthreads.cpp src/core/src/cthreadstopper.cpp src/core/src/cworker.cpp src/sound/src/audiocodecmanager.cpp src/sound/src/csoundmodule.cpp src/sound/src/oggcodechandler.cpp src/sound/src/sound.cpp src/sound/src/soundmanager.cpp src/sound/src/soundplaylistener.cpp src/input/src/cinputeventstranslator.cpp src/input/src/ckeyboardeventstranslator.cpp src/input/src/cmousemotioneventstranslator.cpp src/input/src/cmousebuttoneventstranslator.cpp

AM_LDFLAGS =  `pkg-config OGRE --libs`  `pkg-config OIS --libs`  `pkg-config libxml++-2.6 --libs` `pkg-config CEGUI-OGRE --libs` -lboost_filesystem -lboost_thread -lopenal -lalut -lvorbis -lvorbisfile -lvorbisenc -logg -lsndfile -lboost_program_options

AM_CPPFLAGS=  `pkg-config OGRE --cflags ` `pkg-config OIS --cflags` `pkg-config libxml++-2.6 --cflags` `pkg-config CEGUI-OGRE --cflags` -Wall   


EXTRA_DIST=data/* \
  	config/* \
	src/core/include/*.h \
	src/core/tests/* \
	src/graphic/include/*.h \
	src/input/include/*.h \
	src/input/test/* \
	src/scene/include/*.h \
	src/sound/include/*.h \
	src/time/include/*.h \
	src/time/tests/* 



install-data-am:

	## Copying all data
	for file in `find $(srcdir)/data/ `; \
	do  \
	echo $${file}; \
	destinationfile=`echo $${file} | sed -e "s|$(srcdir)/data|$(pkgdatadir)|" `; \
	destinationdir=`dirname "$${destinationfile}"`; \
	test -z "$${destinationdir}" || mkdir -p -- "$${destinationdir}"; \
	$(INSTALL) $${file} $${destinationfile} \
	; done
	## Creating launch script that will add
	## Data directory to PersistentObject repositories
	echo -e "#!/bin/sh \n\
	\n \
	$(bindir)/gnoll --load-source-directory=$(pkgdatadir) --save-source-directory=$(pkgdatadir)" > $(bindir)/launch-gnoll
	chmod +x $(bindir)/launch-gnoll
	## Replace tokens for files in config/ and copy the file to $DATADIR
	## Remove any trailing '.template' filename extension if there is any
	for file in `find $(srcdir)/config/ -type f `; \
	do  \
	filename=`basename $${file}` ; \
	filename=`echo $${filename} | sed -e s/\.template// ` ; \
	sed -e "s|\*DATADIR\*|$${pkgdatadir}|g" $${file} > $(pkgdatadir)/$${filename} \
	; done


uninstall-hook:
	
	## First delete all files
	for file in `find $(srcdir)/data/ -type f `; \
	do  \
	filetoerase=` echo $${file} | sed -e "s|$(srcdir)/data|$(pkgdatadir)|" `; \
	rm -f "$${filetoerase}";  \
	done
	## Then delete all non-empty directories
	for file in `find $(srcdir)/data/ -depth -type d `; \
	do  \
	filetoerase=` echo $${file} | sed -e "s|$(srcdir)/data|$(pkgdatadir)|" `; \
	rmdir --ignore-fail-on-non-empty "$${filetoerase}";  \
	done
	## Delete launcher
	rm -f $(bindir)/launch-gnoll
	## Finally delete all files from config
	for file in `find $(srcdir)/config/ -type f `; \
	do  \
	filetoerase=` echo $${file} | sed -e "s|$(srcdir)/config|$(pkgdatadir)|" -e s/\.template// `; \
	rm -f "$${filetoerase}";  \
	done



distuninstallcheck:
		@:




