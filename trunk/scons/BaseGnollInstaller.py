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




import glob
import re
import os.path
import os

class BaseGnollInstaller:


	def __init__(self):
		pass

	def install(self, env, config):
		self.installConfig(env, config)


	def parseConfigFile(self, env, config, filename):


		dest = re.sub(r'\.template$', r'', filename)
		dest = re.sub(r'config/', r'', dest)
		dest = env['install_data'] + str(dest)

		print "Generating [%s] from [%s]" % (dest, filename)

		destFile = file(str(dest), "w")
		sourceFile = file(str(filename), "r")

		myEnv = env
		if os.environ.has_key('RPM_BUILD_ROOT') :
				myEnv['install_bin'].replace(os.environ['RPM_BUILD_ROOT'], '')
				myEnv['install_data'].replace(os.environ['RPM_BUILD_ROOT'], '')


		destFile.write(sourceFile.read() % myEnv)

		sourceFile.close()
		destFile.close()


	def gatherConfigFiles(self, path):

		files = []
		for i in glob.glob(path + '/*'):
			if os.path.isfile(i):
				files.append(i)
			elif os.path.isdir(i):
				files.extend(self.gatherConfigFiles(i))
		return files


	def installConfig(self, env, config):

		files = self.gatherConfigFiles('config')
		print "Template(s) to parse : " + str(files)

		for file in files:
			self.parseConfigFile(env, config, file)


