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




from BaseGnollInstaller import *
import os.path
import re
import SCons.Action
import SCons.Builder
import SCons.Environment
import SCons.PathList
import SCons.Sig
import SCons.Subst
import SCons.Tool


class LinuxGnollInstaller (BaseGnollInstaller):

	def createLauncher(self, env, config):

		launcher_filename = 'launch-gnoll'

		env.Alias('install_bin_launcher', env.Install(env['install_bin'], 'launch-gnoll'))
		env.Alias('install_bin', ['install_bin_launcher'])


		if os.path.isfile(launcher_filename):
			return

		print "Creating launcher..."
		launcher_sh = """#!/bin/sh\n%(install_bin)sgnoll --load-source-directory="%(install_data)s" --save-source-directory="%(install_data)s" \n"""

		launcher = file( launcher_filename, "w")

		launcher.write(launcher_sh % (env))

		launcher.close()




	def __init__(self):
		pass


	def install(self, env, config):
		self.createLauncher(env, config)
