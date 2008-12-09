#!/usr/bin/python

#  Copyright (C) 2008 by Bruno Mahe
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the
#  Free Software Foundation, Inc.,
#  59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.



import subprocess
import sys
import os
from optparse import OptionParser
import tempfile

# All account informations are stored in ~/.netrc file
import netrc

import smtplib

from email.mime.text import MIMEText


# Global variables
buffer = []

server = None

tmpPath = tempfile.mkdtemp()
srcPath = tmpPath + '/gnoll'
dstPath = None

repository = None





def configure():
	global server, srcPath, dstPath, repository
	parser = OptionParser(usage="""\
	Update Doxygen doc on server and send an email to admins.

	Usage: %prog [options]
	""")
	parser.add_option('-s', '--server',
							type='string', action='store',
							help="""Server where the documentation will be uploaded.""")
	parser.add_option('--dest-path',
							type='string', action='store',
							help="""Directory on the server where the documentation will be uploaded.""")
	parser.add_option('--repository',
							type='string', action='store',
							help="""Repository where the project is located.""")
	opts, args = parser.parse_args()

	if not opts.server or not opts.dest_path or not opts.repository:
		parser.print_help()
		sys.exit(1)

	server = opts.server
	dstPath = opts.dest_path
	repository = opts.repository



def createDoc():
	global srcPath, dstPath, repository, buffer
	retcode = None

	try:
		if srcPath[-1] != '/':
			srcpath = "%s/" % (srcPath)

		p = subprocess.Popen("doxygen", shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, cwd="%s/"%(srcPath))
		for i in p.stdout:
			buffer.append(i)
			sys.stdout.write( i )
		retcode = p.wait()

		if retcode < 0:
			print >>sys.stderr, "Child was terminated by signal", -retcode
		else:
			print >>sys.stderr, "Child returned", retcode
	except OSError, e:
		print >>sys.stderr, "Execution failed:", e
	except KeyboardInterrupt:
		print >>sys.stderr, "Exiting."

	buffer.append("\n")
	buffer.append( '-' * 25)
	buffer.append("\n")

	if retcode != 0:
		buffer.append( "ERROR: Couldn't create doxygen doc. Exiting\n\n")

	return retcode

# ssh-agent should be configured so no password is required to run this tool
def uploadDoc():
	global srcPath, dstPath, repository, buffer
	nr = netrc.netrc()

	retcode = 0
	#print "Server : %s" % (server)
	(login, account, password) = nr.authenticators(server) or (None, None, None)

	#print "Login : %s" % (login)
	#print "Account : %s" % (account)
	#print "Password : %s" % (password)

	try:
		rsync = "rsync"

		if srcPath[-1] != '/':
			srcPath = srcPath + '/'

		docPath = "%s/doc/" % (srcPath)

		rsyncArgs = " -zvvruclt --delete --chmod=ugo+r,u+x,u+w,Dugo+x --rsh=ssh %s* %s@%s:%s" % (docPath, login, server, dstPath)

		buffer.append( "Executing  : rsync\n" )
		p = subprocess.Popen( rsync + rsyncArgs, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, stdin=subprocess.PIPE)

		for i in p.stdout:
			buffer.append(i)
			sys.stdout.write(i)

		retcode = p.wait()

		buffer.append("\n")
		buffer.append( '-' * 25)
		buffer.append("\n")

		if retcode < 0:
			buffer.append( "Child was terminated by signal " + str(-retcode))
		else:
			buffer.append( "Child returned" +  str(retcode))
	except OSError, e:
		buffer.append( "Execution failed: " +  str(e))
	except KeyboardInterrupt:
		buffer.append("Exiting.")


	if retcode != 0:
		buffer.append("ERROR: Couldn't upload doc. Exiting\n")

	return retcode



def downloadProject():
	global srcPath, dstPath, repository, buffer
	try:
		buffer.append( "Executing  : [git clone %s %s]\n" % (repository, srcPath) )
		p = subprocess.Popen("git clone %s %s" %(repository, srcPath), shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
		for i in p.stdout:
			buffer.append(i)
			sys.stdout.write( i )
		retcode = p.wait()

		if retcode < 0:
			print >>sys.stderr, "Child was terminated by signal", -retcode
		else:
			print >>sys.stderr, "Child returned", retcode
	except OSError, e:
		print >>sys.stderr, "Execution failed:", e
	except KeyboardInterrupt:
		print >>sys.stderr, "Exiting."

	buffer.append("\n")
	buffer.append( '-' * 25)
	buffer.append("\n")

	if retcode != 0:
		buffer.append("ERROR: Couldn't create doxygen doc. Exiting")

	return retcode

def cleanup():

	buffer.append( "Cleaning everything under [%s]" % (srcPath) )

	for root, dirs, files in os.walk(srcPath, topdown=False):
		for name in files:
			buffer.append( "Removing file %s" % (os.path.join(root, name)) )
			os.remove(os.path.join(root, name))
		for name in dirs:
			buffer.append( "Removing directory %s" % (os.path.join(root, name)))
			os.rmdir(os.path.join(root, name))


	buffer.append( "Removing directory %s" % (srcPath))
	os.rmdir(srcPath)

	buffer.append( "Removing directory %s" % (tmpPath))
	os.rmdir(tmpPath)

	return 0

def sendEmail(error):

	buffer.append("\n")
	buffer.append("%d error(s) encountered" % (error))

	msg = MIMEText(''.join(buffer))

	recipient = 'gnoll-bots@lists.gnoll.org'
	fromField = "bot@gnoll.org"

	msg['Subject'] = '[BOT] Documentation updated'
	msg['From'] = fromField
	msg['To'] = recipient

	s = smtplib.SMTP()
	s.connect()
	s.sendmail(fromField, [recipient], msg.as_string())
	s.close()

def main():
	configure()
	error = 0
	error = error + downloadProject()
	error = error + createDoc()
	error = error + uploadDoc()
	error = error + cleanup()
	sendEmail(error)

main()
