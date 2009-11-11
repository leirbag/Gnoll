#/***************************************************************************
# *   Copyright (C) 2009 by Huau Gabriel                                    *
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


import SCons
import SCons.Defaults

import os
import os.path
import re
import stat
import shutil
import glob


#
# Write a config file from a template
#
def parseConfigFile(env, filename):
	dest = re.sub(r'\.template$', r'', filename)
	dest = re.sub(r'config/', r'', dest)
	destFile   = file(str(dest), "w")
	sourceFile = file(str(filename), "r")
	destFile.write(sourceFile.read() % env)
	sourceFile.close()
	destFile.close()
	return dest


#
# Generate the documentation
#
def doc(target, source, env):
	if not env.Detect('doxygen'):
		print "Doxygen is not installed"
		exit(-1)

	env.Execute('doxygen')


#
# Install data/binary/config files
#
def install(env):
	# Configure environnement
	env['install_prefix'] = ARGUMENTS.get('prefix')
	if not env['install_prefix']:
		env['install_prefix'] = env['prefix']
		if not env['install_prefix']:
			env['install_prefix'] = "/usr/"
	if env['install_prefix'][-1] != '/' :
		env['install_prefix'] = env['install_prefix'] + '/'

	env['ogre_lib_path'] = ARGUMENTS.get('ogre_lib_path')
	if not env['ogre_lib_path']:
		env['ogre_lib_path'] = "/usr/lib/OGRE/"

	if env['ogre_lib_path'][-1] != '/' :
		env['ogre_lib_path'] = env['ogre_lib_path'] + '/'

	env['install_bin']    = env['install_prefix'] + "bin/"
	env['install_data']   = env['install_prefix'] + 'share/gnoll-example/data/'
	env['install_config'] = env['install_data']

	# Log directory
	logDir = os.path.dirname( configProject['DEFAULT_LOG_FILENAME'] )
	logDir = logDir[1:]

	if not os.path.exists( logDir ):
		try:
			print "Creating destination logs directory [%s]" % (logDir)
			os.makedirs(logDir)
		except OSError, why:
			print "Can't create %s: %s" % (logDir, why)
	else:
		print "No need to create %s. It already exists" % (logDir)


	launcherEnv = env.Clone()
	# If we are building a rpm,
	# install_bin and install_data are prefixed by the
	# rpm script build root
	if os.environ.has_key('RPM_BUILD_ROOT') :
		launcherEnv['install_bin']  = env['install_bin'].replace(os.environ['RPM_BUILD_ROOT'], '')
		launcherEnv['install_data'] = env['install_data'].replace(os.environ['RPM_BUILD_ROOT'], '')

	# Add a pre action to parse config files
	# We generate the list of all config file and delete the
	# extension .template
	files = []
	for i in glob.glob('config/*'):
		if os.path.isfile(i):
			files.append(i)
		elif os.path.isdir(i):
			files.extend(self.gatherConfigFiles(i))

	files2 = []
	for f in files:
		files2.append(parseConfigFile(launcherEnv, f))

	# Create the launcher for each plateform
	launcher_sh = ""
	if(env['PLATFORM'] == 'posix'):
		launcher_sh = """#!/bin/sh\n%(install_bin)sgnoll --load-source-directory="%(install_data)s" --save-source-directory="%(install_data)s" \n"""
	else:
		print "Your platform is not supported to create the launcher, sorry"

	launcher_filename = 'gnoll-example'
	launcher          = file(launcher_filename, "w")
	launcher.write(launcher_sh % (launcherEnv))
	launcher.close()

	# Installation
	env.Alias('install', env.Install(env['install_bin'], 'gnoll'))
	env.Alias('install', env.Install(env['install_bin'], launcher_filename))
	env.Alias('install', env.Install(env['install_data'], Glob('data/*')))
	env.Alias('install', env.Install(env['install_data'], files2))


	# CHMOD the launcher
	os.chmod(launcher_filename, stat.S_IRWXU | stat.S_IRGRP | stat.S_IXGRP | stat.S_IROTH | stat.S_IXOTH)


#
# Construct the project configuration (version, debug/release, if we want to use
# log, etc.)
#
def constructGnollConfigProject(env):
	configProject = {
				"DEBUG"                 : "0",
				"DO_LOG"                : "0",
				"DEFAULT_LOG_FILENAME"  : '"./gnoll.log"',
				"PACKAGE_NAME"          : '"Gnoll"',
				"PACKAGE_VERSION"       : '"0.1.7"' ,
				"PACKAGE_BUGREPORT"     : '"gnoll-devel@lists.gnoll.org"',
				"PACKAGE_STRING"        : '"Gnoll 0.1.7"',
				"PACKAGE_TARNAME"       : '"gnoll"'
			}

	# Change default log path for posix systems
	if (env['PLATFORM'] == 'posix'):
		linuxLogDir = os.path.expanduser("~") + '/.gnoll/logs'
		configProject["DEFAULT_LOG_FILENAME"] = '"' + linuxLogDir + '/gnoll.log' + '"'

	return configProject


#
# Main
#
# Configuration of the environnement
Default()
env                            = Environment(ENV = os.environ)
configProject                  = constructGnollConfigProject(env)
build_dir                      = "Release"
opts = Variables('gnoll.conf')
opts.Add(PathVariable(
						'prefix', 'Directory to install under', '/usr/share', SCons.Variables.PathVariable.PathAccept
				)
		)

opts.Add(
						'debug', 'Set up to 1 if you want debug version', '0'
		)

opts.Add(
						'do_log', 'Set up to 1 if you want to generate log in binary', '0'
		)

opts.Update(env)
opts.Save('gnoll.conf', env)
Help(opts.GenerateHelpText(env))
env['CPPPATH'] = os.getenv ('CPPPATH')
env['LIBPATH'] = os.getenv ('LIBPATH')
env['LIBS']    = os.getenv ('LIBS')

# Do we want to compile in debug mode ?
if ARGUMENTS.get('debug', '0') == '1' or env['debug'] == '1':
	configProject['DEBUG'] = '1'
	build_dir              = "Debug"

# Do we want to activate logging ?
if ARGUMENTS.get('do_log', '0') == '1' or env['do_log'] == '1':
	configProject['DO_LOG'] = '1'
	build_dir              += "_log"

##### BUILD #####
Export('env')
Export('configProject')

env.Alias('check',   SConscript('src/SConscheck', exports=['env']))
env.Alias('build',   Alias('check'))
env.Alias('build',   Alias('gnollTarget'))
env.Alias('tests',   Alias('check'))
env.Alias('install', Alias('build'))

env.AlwaysBuild('check')

env.Alias('doc',   action = doc)

binDir = "./"
if SCons.__version__ < "1.2.0":
	Export('binDir')
	env.Alias('build', SConscript('src/SConsbuild'))
	env.Alias('tests', SConscript('src/SConstests'))
else:
	binDir = "../../"
	Export('binDir')
	env.Alias('build', SConscript('src/SConsbuild', variant_dir="build/" + build_dir, exports=['env']))
	env.Alias('tests', SConscript('src/SConstests', variant_dir="build/" + build_dir, exports=['env']))

env.Alias('install', install(env))
