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




from BaseGnollBuilder import *


class LinuxGnollBuilder(BaseGnollBuilder):

	def checkOgre(self, env, config):

		env.MergeFlags([ '!pkg-config OGRE --cflags --libs'])

		return BaseGnollBuilder.checkOgre(self, env, config)



	def checkOIS(self, env, config):

		env.MergeFlags([ '!pkg-config OIS --cflags --libs'])

		return BaseGnollBuilder.checkOIS(self, env, config)



	def checkLibXmlpp(self, env, config):

		env.MergeFlags([ '!pkg-config libxml++-2.6 --cflags --libs'])

		return BaseGnollBuilder.checkLibXmlpp(self, env, config)


	def checkCEGUI(self, env, config):

		env.MergeFlags([ '!pkg-config CEGUI-OGRE --cflags --libs'])

		return BaseGnollBuilder.checkCEGUI(self, env, config)


	def gatherTimeSourceFiles(self, env, config):
		timeSourceFiles = BaseGnollBuilder.gatherTimeSourceFiles(self, env, config)
		timeSourceFiles.append('src/time/src/clinuxtimer.cpp')

		return timeSourceFiles


	def __init__(self):
		pass
