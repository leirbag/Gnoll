#/***************************************************************************
# *   Copyright (C) 2008 by Bruno Mahe                                      *
# *                                                                         *
# *   This program is free software; you can redistribute it and/or modify  *
# *   it under the terms of the GNU General Public License as published by  *
# *   the Free Software Foundation; either version 2 of the License, or     *
# *   (at your option) any later version.                                   *
# *                                                                         *
# *   This program is distributed in the hope that it will be useful,       *
# *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
# *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
# *   GNU General Public License for more details.                          *
# *                                                                         *
# *   You should have received a copy of the GNU General Public License     *
# *   along with this program; if not, write to the                         *
# *   Free Software Foundation, Inc.,                                       *
# *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
# ***************************************************************************/



class BaseGnollBuilder:

	def checkBoost(self, env, config):

		configProject = {}

		if not config.CheckCXXHeader('boost/shared_ptr.hpp'):
			print "Boost::shared_ptr not found."
			exit(-1)

		if not config.CheckCXXHeader('boost/program_options.hpp'):
			print "Boost::program_options not found"
			exit(-1)

		config.CheckLib('boost_program_options', None, None, 'c++')

		for header in ['boost/filesystem/path.hpp', 'boost/filesystem/convenience.hpp', 'boost/filesystem/operations.hpp']:
			if not config.CheckCXXHeader(header):
				print "Boost::filesystem not found"
				exit(-1)

		config.CheckLib('boost_filesystem', None, None, 'c++')


		for header in ['boost/thread/mutex.hpp', 'boost/thread/thread.hpp', 'boost/thread/condition.hpp', 'boost/thread/xtime.hpp']:
			if not config.CheckCXXHeader(header):
				print "Boost::thread not found"
				exit(-1)

		config.CheckLib('boost_thread', None, None, 'c++')

		configProject['HAVE_BOOST_SHARED_PTR'] = '1'
		configProject['HAVE_BOOST_PROGRAM_OPTIONS'] = '1'
		configProject['HAVE_BOOST_FILESYSTEM'] = '1'
		configProject['HAVE_BOOST_THREAD'] = '1'
		return configProject


	def checkOpenAL(self, env, config):

		configProject = {}

		for header in ['AL/al.h', 'AL/alc.h']:
			if not config.CheckCXXHeader(header):
				print "OpenAL not found"
				exit(-1)

		config.CheckLib('openal', None, None, 'c++')
		config.CheckLib('alut', None, None, 'c++')

		configProject['HAVE_OPENAL'] = '1'
		return configProject



	def checkVorbis(self, env, config):

		configProject = {}

		for header in ['vorbis/vorbisfile.h']:
			if not config.CheckCXXHeader(header):
				print "Vorbis library not found"
				exit(-1)

		config.CheckLib('vorbis', None, None, 'c++')
		config.CheckLib('vorbisenc', None, None, 'c++')
		config.CheckLib('vorbisfile', None, None, 'c++')
		config.CheckLib('ogg', None, None, 'c++')

		configProject['HAVE_OGG_VORBIS'] = '1'
		return configProject

	def checkOgre(self, env, config):

		configProject = {}

		for header in ['Ogre.h', 'OgreLogManager.h', 'OgrePlatform.h', 'OgreTimer.h', 'OgreMemoryMacros.h']:
			if not config.CheckCXXHeader(header):
				print "Ogre library not found"
				exit(-1)

		config.CheckLib('OgreMain', None, None, 'c++')

		configProject['HAVE_OGRE'] = '1'
		return configProject


	def checkOIS(self, env, config):

		configProject = {}

		for header in ['OISMouse.h', 'OISKeyboard.h', 'OISJoyStick.h', 'OISInputManager.h']:
			if not config.CheckCXXHeader(header):
				print "OIS library not found"
				exit(-1)

		config.CheckLib('OIS', None, None, 'c++')

		if config.CheckLibWithHeader( 'OIS', 'OISInputManager.h', 'c++', 'OIS::InputManager::createInputSystem(0)->numKeyBoards();' ):
			configProject['HAVE_OIS_1RC'] = '1'


		configProject['HAVE_OIS'] = '1'
		return configProject


	def checkLibXmlpp(self, env, config):

		configProject = {}

		for header in ['libxml++/libxml++.h', 'glibmm/ustring.h', 'glibmm/unicode.h', 'glib/gunicode.h', 'glib/gmacros.h']:
			if not config.CheckCXXHeader(header):
				print "libxml++2.6 library not found"
				exit(-1)

		config.CheckLib('xml++-2.6', None, None, 'c++')
		config.CheckLib('xml2', None, None, 'c++')
		config.CheckLib('glibmm-2.4', None, None, 'c++')
		config.CheckLib('gobject-2.0', None, None, 'c++')
		config.CheckLib('sigc-2.0', None, None, 'c++')
		config.CheckLib('glib-2.0', None, None, 'c++')


		configProject['HAVE_LIB_XML_PP'] = '1'
		configProject['HAVE_XML2'] = '1'
		configProject['HAVE_GLIBMM'] = '1'
		configProject['HAVE_GOBJECT'] = '1'
		configProject['HAVE_SIGC'] = '1'
		configProject['HAVE_GLIB'] = '1'

		return configProject


	def checkCEGUI(self, env, config):

		configProject = {}

		for header in ['OGRE/OgreCEGUIRenderer.h', 'CEGUI/CEGUIImageset.h', 'CEGUI/CEGUISystem.h', 'CEGUI/CEGUILogger.h', 'CEGUI/CEGUIFont.h', 'CEGUI/CEGUIFontManager.h', 'CEGUI/CEGUISchemeManager.h', 'CEGUI/CEGUIWindowManager.h', 'CEGUI/CEGUIWindow.h']:
			if not config.CheckCXXHeader(header):
				print "CEGUI library not found"
				exit(-1)

		config.CheckLib('CEGUIBase', None, None, 'c++')
		config.CheckLib('CEGUIOgreRenderer', None, None, 'c++')

		configProject['HAVE_CEGUI'] = '1'
		configProject['HAVE_CEGUI_OGRE'] = '1'

		return configProject


	def checkGnoll(self, env, config):

		configProject = {}

		boostConfig    = self.checkBoost(env, config)
		openALConfig   = self.checkOpenAL(env, config)
		vorbisConfig   = self.checkVorbis(env, config)
		ogreConfig     = self.checkOgre(env, config)
		oisConfig      = self.checkOIS(env, config)
		libXmlppConfig = self.checkLibXmlpp(env, config)
		ceguiConfig    = self.checkCEGUI(env, config)


		env.MergeFlags([ '-Wall',
							'-I ./src/'])

					#		  '-lsndfile'])


		for config in (boostConfig, openALConfig, vorbisConfig, ogreConfig, oisConfig, libXmlppConfig, ceguiConfig):
			for (key, value) in config.iteritems():
				if not configProject.has_key(key):
					configProject[key] = value

		return configProject



	def gatherCoreSourceFiles(self, env, config):

		coreSourceFiles = [
			'src/core/src/cgenericmessagemanager.cpp',
			'src/core/src/cmessagetype.cpp',
			'src/core/src/filestream.cpp',
			'src/core/src/sourcefile.cpp',
			'src/core/src/cmessagemodule.cpp',
			'src/core/src/cpoolthreads.cpp',
			'src/core/src/cthreadstopper.cpp',
			'src/core/src/cworker.cpp',
			]


		return coreSourceFiles


	def gatherDynamicObjectSourceFiles(self, env, config):

		dynamicObjectSourceFiles = [
			'src/dynamicobject/src/attributehandlerregistry.cpp',
			'src/dynamicobject/src/list.cpp',
			'src/dynamicobject/src/string.cpp',
			'src/dynamicobject/src/dynamicobject.cpp',
			'src/dynamicobject/src/dynamicobjectmanager.cpp',
			'src/dynamicobject/src/inheritsattributehandler.cpp',
			'src/dynamicobject/src/inherits.cpp',
			'src/dynamicobject/src/cdynamicobjectproxy.cpp',
			'src/dynamicobject/src/set.cpp'
			]


		return dynamicObjectSourceFiles


	def gatherTimeSourceFiles(self, env, config):

		timeSourceFiles = [
			'src/time/src/ccreateperiodiceventlistener.cpp',
			'src/time/src/cdestroyperiodiceventlistener.cpp',
			'src/time/src/ccreatedelayedeventlistener.cpp',
			'src/time/src/cdestroydelayedeventlistener.cpp',
			'src/time/src/ctimemodule.cpp'
			]

		return timeSourceFiles


	def gatherGraphicSourceFiles(self, env, config):

		graphicSourceFiles = [
			'src/graphic/src/cgraphicmodule.cpp',
			'src/graphic/src/cscenefactorystd.cpp',
			'src/graphic/src/csceneparser.cpp',
			'src/graphic/src/cceguilogger.cpp',
			]

		return graphicSourceFiles


	def gatherInputSourceFiles(self, env, config):

		inputSourceFiles = [
			'src/input/src/coisinputmodule.cpp',
			'src/input/src/coisinputmanager.cpp',
			'src/input/src/cinputeventstranslator.cpp',
			'src/input/src/ckeyboardeventstranslator.cpp',
			'src/input/src/ckeyboardstatetranslator.cpp',
			'src/input/src/cmousemotioneventstranslator.cpp',
			'src/input/src/cmousebuttoneventstranslator.cpp',
			'src/input/src/cmousebuttonstatetranslator.cpp',
			'src/input/src/ckeyboardeventstrigger.cpp',
			'src/input/src/cmousebuttoneventstrigger.cpp'
			]

		return inputSourceFiles


	def gatherSceneSourceFiles(self, env, config):

		sceneSourceFiles = [
			'src/scene/src/cmeshpagerenderer.cpp',
			'src/scene/src/cpage.cpp',
			'src/scene/src/cscenemanager.cpp',
			'src/scene/src/camera.cpp',
			'src/scene/src/camerafixe.cpp',
			'src/scene/src/camerafixefactory.cpp',
			'src/scene/src/defaultcamerathirdpersonlistener.cpp',
			'src/scene/src/camerathirdperson.cpp',
			'src/scene/src/camerathirdpersonfactory.cpp',
			'src/scene/src/cmessagelistenercamera.cpp',
			]

		return sceneSourceFiles

	def gatherStatsSourceFiles(self, env, config):

		statsSourceFiles = [
			'src/stats/src/cstatsmodule.cpp',
			'src/stats/src/statsmodulelistener.cpp'
			]

		return statsSourceFiles

	def gatherSoundSourceFiles(self, env, config):

		soundSourceFiles = [
			'src/sound/src/audiocodecmanager.cpp',
			'src/sound/src/csoundmodule.cpp',
			'src/sound/src/oggcodechandler.cpp',
			'src/sound/src/sound.cpp',
			'src/sound/src/soundmanager.cpp',
			'src/sound/src/soundplaylistener.cpp'
			]

		return soundSourceFiles

	def gatherSourceFiles(self, env, config):


		sourceFiles = []

		sourceFiles.extend( self.gatherSoundSourceFiles(env, config) )
		sourceFiles.extend( self.gatherSceneSourceFiles(env, config) )
		sourceFiles.extend( self.gatherInputSourceFiles(env, config) )
		sourceFiles.extend( self.gatherStatsSourceFiles(env, config) )
		sourceFiles.extend( self.gatherGraphicSourceFiles(env, config) )
		sourceFiles.extend( self.gatherTimeSourceFiles(env, config) )
		sourceFiles.extend( self.gatherDynamicObjectSourceFiles(env, config) )
		sourceFiles.extend( self.gatherCoreSourceFiles(env, config) )
		sourceFiles.append( 'src/main.cpp' )


		return sourceFiles


	def buildGnoll(self, env, config):

		sourceFiles = self.gatherSourceFiles(env, config)

		env.Program('gnoll', sourceFiles)



