#!BPY

""" Registration info for Blender menus:
Name: 'Scene for Gnoll engine...'
Blender: 246
Group: 'Export'
Tip: 'Export the scene to Gnoll game engine.'
"""

__author__ = 'Antonin <Wetneb> Delpeuch'
__url__ = ['gnoll.org', 'projetviracocha.free.fr']
__version__ = "1.0"

__bpydoc__ = """\
This script allows you to subdivide a mesh with a grid, and to export the result as a Gnoll scene.

The size of the resulting squares is the "page size". In Object mode, select the ground object,
and define the pages size. You can use the shortcuts buttons 1/2, 1/3 and 1/4 to get quickly a good result.
Enabling the Gnoll export will generate XML files defining the scene for Gnoll. Those files are exported
in the specified folder.

Some informations are available on Gnoll's Wiki : http://wiki.gnoll.org/
"""

#####################################
#          MAP SUBDIVIDER           #
#####################################
# by Antonin Delpeuch <Wetneb>
# antonin@delpeuch.eu
# http://www.antonin.delpeuch.eu/
#
# Based on Blender Knife Tool (see below)
# Released under Blender Artistic Licence
# see http://www.blender.org/
# Written for the Viracocha project :
# see http://projetviracocha.free.fr/
#

##### GUI & MAIN PROCESS #####


# Modules
import Blender
from Blender import *
from Blender import NMesh
from Blender import Mesh
from Blender import Image
from Blender.BGL import *
from Blender.Draw import *
from Blender.Scene import *
import math, string
from math import *
from xml.dom import *
import gnollTools
from gnollTools import *
import knife
from knife import *

# Even if I hate globals,
# GUI vars need to be global...
# Events identifiers(constants)
EVT_NOEVT = 1
EVT_QUIT = 2
EVT_SUBD = 3
EVT_GNOLL = 4
EVT_GNOLL_VERSION = 5
EVT_PATH = 6
EVT_INITIAL = 7
EVT_OGRE = 8
EVT_OGRE3 = 9
EVT_OGRE4 = 10
EVT_SET2 = 11
EVT_SET3 = 12
EVT_SET4 = 13

# Settings (input from the GUI)
SquareSize = Create(1.0)
maxSquareSize = Create(20.0)
Epsilon = 0.00001
genGnollFiles = False
gnollFilesVersion = Create(2)
addInitialPage = False
exportOgre = False
isOgre3 = False
exportPath = 0
filesExportPath = "/home/leirbag/projects/"

# Image used in the GUI
# logo = Blender.Image.Load('logo.png')

# Get and check settings
def getSettings():
	global SquareSize

	popupError = []
	popupError.append("Ok")

	if(SquareSize.val == 0):
		retval = Draw.PupBlock("Error", ("The size of the pages","can't be null."))
		return (-1, -1)

	selectedObj = Object.GetSelected()

	if(len(selectedObj) < 1):
		retval = Draw.PupBlock("Error", ("You must select the","object to subdivide.","Usually, it's the ground object."))
		return (-1, -1)
	elif(len(selectedObj) > 1):
		retval = Draw.PupBlock("Error", ("There are too many selected objects !","Just select one object (the ground)."))
		return (-1, -1)
	elif(selectedObj[0].type != 'Mesh'):
		retval = Draw.PupBlock("Error", ("The object must be a mesh.","Try again !"))
		return (-1, -1)
	else:
		return (SquareSize.val, selectedObj[0])
	Exit()

# Read bounding box informations
# (the way Blender stores bounding boxes informations isn't efficient in this problem)
def getBoundBox(object):
	boundBox = object.getBoundBox()

	# Find the lowest X coordinate
	xmin = boundBox[0].x
	i = 1
	while i < 8:
		if(boundBox[i].x < xmin):
			xmin = boundBox[i].x
		i = i + 1

	# Find the highest X coordinate
	xmax = boundBox[0].x
	i = 1
	while i < 8:
		if(boundBox[i].x > xmax):
			xmax = boundBox[i].x
		i = i + 1

	# Find the lowest Y coordinate
	ymin = boundBox[0].y
	i = 1
	while i < 8:
		if(boundBox[i].y < ymin):
			ymin = boundBox[i].y
		i = i + 1

	# Find the highest Y coordinate
	ymax = boundBox[0].y
	i = 1
	while i < 8:
		if(boundBox[i].y > ymax):
			ymax = boundBox[i].y
		i = i + 1

	# Find the lowest Z coordinate
	zmin = boundBox[0].z
	i = 1
	while i < 8:
		if(boundBox[i].z < zmin):
			zmin = boundBox[i].z
		i = i + 1

	# Find the highest Z coordinate
	zmax = boundBox[0].z
	i = 1
	while i < 8:
		if(boundBox[i].z > zmax):
			zmax = boundBox[i].z
		i = i + 1

	return (xmin, xmax, ymin, ymax, zmin, zmax)

# Generate list of planes (which corresponds to grid's lines)
def getPlanes(stepSize, boundBox):
	xPlanes = []
	yPlanes = []
	(xmin, xmax, ymin, ymax, zmin, zmax) = boundBox

	Window.DrawProgressBar(0, "Planes")
	# Generate planes along the Y axis
	print "   Generate planes along the Y axis..."
	x = xmax - SquareSize.val
	while x > xmin:
		xPlanes.append(((1,0,0),(x,0,0))) #  if (x>0) else x-1
		x = x - SquareSize.val
	print "  ", len(xPlanes), "planes along Y generated."

	Window.DrawProgressBar(0.5, "Planes")

	# Generate places along the X axis
	print "   Generate planes along the X axis..."
	y = ymax - SquareSize.val
	while y > ymin:
		yPlanes.append(((0,1,0),(0,y,0))) #  if (ymin>0) else y-1
		y = y - SquareSize.val

	# Debug
	print "  ", len(yPlanes), "planes along X generated."
	Window.DrawProgressBar(1, "Planes")

	return (xPlanes, yPlanes)

# Move the center of a mesh without moving the vertices
def moveMesh(object, x, y, z):
	# Calculate the translation vector
	vecx = x - object.loc[0]
	vecy = y - object.loc[1]
	vecz = z - object.loc[2]

	# Move the center of the object
	object.setLocation(x, y, z)

	# Move (inversed) the each verts of the mesh
	me = object.getData()
	i = 0
	for vert in me.verts:
		vert.co[0] -= vecx
		vert.co[1] -= vecy
		vert.co[2] -= vecz
		me.verts[i] = vert
		i = i + 1

	# Update it !
	me.update()

# Globalize a mesh (adapted from the Blender Knife Tool)
def globalPosition(P, Obj):
	m = Obj.getMatrix()

	PX = P[0]*m[0][0] + P[1]*m[1][0] + P[2]*m[2][0] + m[3][0]
	PY = P[0]*m[0][1] + P[1]*m[1][1] + P[2]*m[2][1] + m[3][1]
	PZ = P[0]*m[0][2] + P[1]*m[1][2] + P[2]*m[2][2] + m[3][2]
	return (PX, PY, PZ)

def globalize(obj):
	if(obj.type == "Mesh"):
		mesh = obj.getData()

		i = 0
		for v in mesh.verts:
			(v.co[0], v.co[1], v.co[2]) = globalPosition(v.co, obj)
			mesh.verts[i] = v
			i = i + 1

		obj.setLocation(0,0,0)
		obj.rot = (0,0,0)
		obj.setSize(1,1,1)
		mesh.update()

# Rename a mesh from the object's name (for a cleaner scene and a easier conversion to Ogre)
def renameMesh(obj):
	mesh = obj.getData(mesh=1)
	mesh.name = getCleanName(obj.name)

# Deletes the suffix added automaticly by Blender (like Plane.002)
def getCleanName(origName):
	result = ""
	if not origName.rfind(".0", 0, len(origName)) == -1:
		result = origName.rpartition('.0')[0]
	return result

# Cut the mesh into columns (along the Y axis) or into pages (along the X axis)
def createColumns(rootObj, xPlanes, arePages, startingIndex, rootName, boundBox, squareSize):
	columns = []
	sce = Blender.Scene.GetCurrent()

	progress = "Cutting Y"
	if arePages:
		progress = "Cutting X"
	Window.DrawProgressBar(0, progress)

	# Generate names
	finalName = '-Col-'
	tempName = '-Temp-'
	if(arePages):
		finalName = '-Page-'
		tempName = '-TempPg-'

	# Set if the algorithm has to use UV coordinates
	useUVCoords = rootObj.getData().hasFaceUV()

	# Init the loop
	cuttingObject = rootObj
	i = 0

	while i < len(xPlanes):
		if arePages:
			Window.DrawProgressBar((startingIndex+i)/(len(xPlanes)*len(xPlanes)), progress)
		else:
			Window.DrawProgressBar(i/len(xPlanes), progress)

		# Create a new object containing the new part
		newObj = Blender.Object.New('Mesh', rootName + finalName + str(startingIndex+i+1))
		tempObj = Blender.Object.New('Mesh', rootName + tempName + str(startingIndex+i+1))


		# Get the meshes
		newMe = newObj.getData()
		tempMe = tempObj.getData()
		print "Using type : ", type(newMe)
		newMe.hasFaceUV(useUVCoords)
		tempMe.hasFaceUV(useUVCoords)

		# Cut the object
		(CN,CC) = xPlanes[i];
		d0 = CC[0]*CN[0]+CC[1]*CN[1]+CC[2]*CN[2]
		Cut(cuttingObject, CN, d0, newMe, tempMe)

		if arePages:
			print "[", i, "] : ", CN, CC

		# If the coordinates of the plane are under zero, we have to invert newMe and tempMe.
		#if(CN[0] < 0 or CN[1]<0):
		#	if arePages:
		#		print "\tInversion."
		#	invertor = tempMe
		#	tempMe = newMe
		#	newMe = invertor

		# Update the objects
		newMe.update()
		tempMe.update()

		cuttingObject = tempObj
		columns.append(newObj)

		# If it's the last cut, the temp object is also a final object
		if(i == len(xPlanes)-1):

			# Add the temp object to the scene
			columns.append(tempObj)
			tempObj.setName(rootName + finalName + str(startingIndex+i+2))
			#newObj.setName(rootName + finalName + str(startingIndex+i+2))

			if(arePages): # Link to the scene if needed (only if we are generating pages)
				renameMesh(tempObj)
				sce.objects.link(tempObj)

		if(arePages): # Link to the scene if needed (only if we are generating pages)
			renameMesh(newObj)
			sce.objects.link(newObj)

		i = i + 1

	return columns

# Map subdividing (main process)
def subdivide():
	global filesExportPath

	### Print header ###
	print "\nGrid Subdivide Tool v0.1 by Antonin <Wetneb> Delpeuch"
	print "Released under Blender Artistic Licence"
	print "Based on Blender Knife Tool v0.6 by Stefano <S68> Selleri"

	### Check settings and selected object ###
	(stepSize, rootObj) = getSettings()
	if(stepSize == -1):
		return 0

	## Globalize the object ###
	globalize(rootObj)

	### Extract mesh and bounding box. ###
	mesh = rootObj.getData(False, True)
	boundBox = getBoundBox(rootObj)

	### Ensure that we're not generating too much planes ###
	(xmin, xmax, ymin, ymax, zmin, zmax) = boundBox
	if (xmax-xmin)/stepSize > 100 or (ymax-ymin)/stepSize > 100:
		toggle = Draw.Create(0)
		retval = Draw.PupBlock("Error", ("You are about to generate",str(int((ymax-ymin)*(xmax-xmin)/stepSize))+" pages.","Do you really want","to continue ?",("Yes, run anyway.",toggle)))
		if not toggle.val:
			return 0

	### Generate list of planes for cutting ###
	(xPlanes, yPlanes) = getPlanes(stepSize, boundBox)

	### Cut the mesh along the planes ###
	pages = []

	# Create columns
	columns = createColumns(rootObj, xPlanes, False, 0, rootObj.getData(True), boundBox, stepSize)

	# Create pages
	i = 0
	while i < len(columns):
		pages.append(createColumns(columns[i], yPlanes, True, i*(len(yPlanes)+1), rootObj.getData(True), boundBox, stepSize))
		i = i + 1

	# Move pages
	i = 0
	while i < len(columns):
		j = 0
		while j < len(pages[i]):
			moveMesh(pages[i][j], boundBox[1]-(i+0.5)*stepSize, boundBox[3]-(j+0.5)*stepSize, (boundBox[5]+boundBox[4])/2)
			j = j +1
		i = i + 1

	# Generate Gnoll files (if needed)
	if genGnollFiles:

		# Create page files
		nbPages = len(columns)*len(pages[0])
		if addInitialPage:
			nbPages = nbPages + 1
		pageFiles = range(nbPages)
		for i in range(len(columns)):
			for j in range(len(pages[i])):
				index = i*len(pages[i])+j
				pageFiles[index] = PageFile("", gnollFilesVersion.val)

				Window.DrawProgressBar(index/(len(columns)*len(pages[i])), "Gnoll files")

				# Create name
				pageFiles[index].name = rootObj.getData(True) + "-Page-" + str(i*len(pages[i])+j+1)
				pageFiles[index].size = stepSize

				# Find neighbours of the current page
				if i < len(columns)-1:
					pageFiles[index].west  = rootObj.name + "-Page-" + str((i+1)*len(pages[i])+j+1)
				if j < len(pages[i])-1:
					pageFiles[index].north = rootObj.name + "-Page-" + str(i*len(pages[i])+j+2)
				if i:
					pageFiles[index].east  = rootObj.name + "-Page-" + str((i-1)*len(pages[i])+j+1)
				if j:
					pageFiles[index].south = rootObj.name + "-Page-" + str(i*len(pages[i])+j)

		# Bound each scene's object to a page (if possible)
		for obj in Scene.GetCurrent().objects:
			# If this objet isn't part of the ground
			if string.find(obj.name, rootObj.name, 0, len(obj.name)) == -1 and string.find(obj.name, "DONTEXPORT_", 0, len(obj.name)) == -1:
				# Find the corresponding page (if the object is in the grid)
				if obj.LocX > boundBox[0] and obj.LocX < boundBox[1] and obj.LocY > boundBox[2] and obj.LocY < boundBox[3]:
					print "Handling object : ", obj.name

					# Indexes of the corresponding page
					pageX = floor((boundBox[1] - obj.LocX) / stepSize)
					pageY = floor((boundBox[3] - obj.LocY) / stepSize)

					# Create XML definition of the object
					objectDef = ObjectDef(obj.name, gnollFilesVersion.val)

					# Coordinates transformation :
					# The problem is that Blender and Ogre don't use the same way to apply
					# transformations to a mesh :
					# - Blender starts to translate the mesh first, then it rotates it
					# - Gnoll starts to rotate the object, then it translates it along
					#   rotated axis.

					objectDef.locX = -(obj.LocX - pages[int(pageX)][int(pageY)].LocX)
					objectDef.locZ = -(obj.LocY - pages[int(pageX)][int(pageY)].LocY)
					objectDef.locY = obj.LocZ - pages[int(pageX)][int(pageY)].LocZ
					print "Position : ", objectDef.locX, objectDef.locY, objectDef.locZ

					# The others parameters are left untouched

					objectDef.sizeX = obj.SizeX
					objectDef.sizeZ = obj.SizeY
					objectDef.sizeY = obj.SizeZ

					objectDef.rotX = -obj.RotX
					objectDef.rotZ = -obj.RotY
					objectDef.rotY = obj.RotZ

					# Add this object to the corresponding page
					pageFiles[int(pageX*len(pages[0])+pageY)].objects.append(objectDef)

		# Add an initial page if needed (defined by the user)
		if(addInitialPage):
			index = len(columns)*len(pages[0])
			pageFiles[index] = PageFile()

			# Create name
			pageFiles[index].name = rootObj.getData(True) + "-Page-Initial"
			pageFiles[index].size = stepSize

			# Find neighbours of the current page
			pageFiles[index].north = rootObj.name + "-Page-1"
			pageFiles[0].south = rootObj.name + "-Page-Initial"

		# For each page, write the file
		for page in pageFiles:
			# Write XML file
			page.writeToFile(filesExportPath)


	Window.DrawProgressBar(1, "")
	print "Done. You can delete temporary objects (just save and restart Blender)."
	return 0

# GUI Drawing
def draw():
	global SquareSize
	global EVT_NOEVT, EVT_QUIT, EVT_SUBD, EVT_PATH, EVT_OGRE, EVT_OGRE3, EVT_OGRE4, EVT_SET2, EVT_SET3, EVT_SET4
	global logo, genGnollFiles, exportPath, addInitialPage, exportOgre, isOgre4, gnollFilesVersion

	maxSquareSize = 10.0
	selectedObj = Object.GetSelected()

	if(len(selectedObj) == 1):
		if(selectedObj[0].type == 'Mesh'):
			mesh = selectedObj[0].getData(False, True)
			boundBox = getBoundBox(selectedObj[0])
			maxSquareSize = boundBox[1] - boundBox[0]
			if boundBox[3] - boundBox[2] > maxSquareSize :
				maxSquareSize = boundBox[3] - boundBox[2]
			if SquareSize.val > maxSquareSize:
				SquareSize.val = maxSquareSize

	# Preparing
	BGL.glClearColor(0.5, 0.5, 0.5, 1)
	BGL.glColor3f(1.,1.,1.)
	glClear(GL_COLOR_BUFFER_BIT)

	# Drawing logo
	# glEnable(GL_BLEND)
	# glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)

	# Draw.Image(logo, 10, 273)

	# glDisable(GL_BLEND)

	# Drawing text
	glRasterPos2d(10,283)
	Draw.Text("Map integrating tools for Gnoll")
	glRasterPos2d(10,263)
	Draw.Text("by Antonin <Wetneb> Delpeuch")
	glRasterPos2d(10,193)
	Draw.Text("Gnoll settings :")
	glRasterPos2d(10,73)
	Draw.Text("General settings :")
#	glRasterPos2d(10,243)
#	Draw.Text("Ogre settings :")

	# Drawing input controls
	SquareSize = Draw.Slider("Size of pages", EVT_NOEVT, 10, 230, 255, 18, SquareSize.val,
		0.00001, maxSquareSize, 0, "Size of the squared pages.")
	Button("1/2", EVT_SET2, 10, 210, 83, 18)
	Button("1/3", EVT_SET3, 95, 210, 83, 18)
	Button("1/4", EVT_SET4, 180, 210, 83, 18)

	if genGnollFiles:
		initialPage = Draw.Toggle("Add an initial page", EVT_INITIAL, 10, 100, 255, 25, addInitialPage)
#	if exportOgre:
#		ogre3 = Draw.Toggle("Ogre 1.3", EVT_OGRE3, 10, 180, 125, 25, isOgre3)
#		ogre4 = Draw.Toggle("Ogre 1.4", EVT_OGRE4, 140, 180, 125, 25, not(isOgre3))
	if genGnollFiles or exportOgre:
		exportPath = Draw.String("Export path : ", EVT_PATH, 10, 40, 255, 25, filesExportPath, 399)

	useGnoll = Draw.Toggle("Generate Gnoll files", EVT_GNOLL, 10, 160, 255, 25, genGnollFiles)
	if genGnollFiles:
		gnollFilesVersion = Draw.Number("Gnoll files version", EVT_GNOLL_VERSION, 10, 130, 255, 25, gnollFilesVersion.val, 1, 2)

	# Drawing main buttons
	Button("Quit", EVT_QUIT, 10, 10, 125, 25)
	Button("Subdivide", EVT_SUBD, 140, 10, 125, 25)


# Event handling
def event(evt, val):
	if(evt == QKEY and not val):
		Exit()

# Updating
def updateGUI():
	global maxSquareSize

	# Updating the maximal square size

	selectedObj = Object.GetSelected()

	if(len(selectedObj) == 1):
		if(selectedObj[0].type == 'Mesh'):
			mesh = selectedObj[0].getData(False, True)
			boundBox = getBoundBox(selectedObj[0])
			maxSquareSize = boundBox[1] - boundBox[0]
			if boundBox[3] - boundBox[2] > maxSquareSize :
				maxSquareSize = boundBox[3] - boundBox[2]
			if SquareSize.val > maxSquareSize:
				SquareSize.val = maxSquareSize
		else:
			maxSquareSize = 10.0
	else:
		maxSquareSize = 10.0

def bevent(evt):
	global SquareSize, genGnollFiles, exportPath, filesExportPath, isOgre3, exportOgre, addInitialPage, maxSquareSize
	global EVT_NOEVT, EVT_QUIT, EVT_SUBD, EVT_OGRE, EVT_OGRE3, EVT_OGRE4, EVT_SET2, EVT_SET3, EVT_SET4, EVT_INITIAL

	updateGUI()
	if(evt == EVT_QUIT):
		Exit()
	elif(evt == EVT_GNOLL):
		genGnollFiles = not(genGnollFiles)
		Blender.Draw.Draw()
	elif(evt == EVT_OGRE):
		exportOgre = not(exportOgre)
		Blender.Redraw()
	elif(evt == EVT_OGRE3):
		isOgre3 = True
		Blender.Redraw()
	elif(evt == EVT_OGRE4):
		isOgre3 = False
		Blender.Redraw()
	elif(evt == EVT_PATH):
		filesExportPath = exportPath.val
	elif(evt == EVT_SUBD):
		if(subdivide()):
			Exit()
		Blender.Redraw()
	elif(evt == EVT_SET2):
		SquareSize.val = maxSquareSize / 1.9
		Blender.Redraw()
	elif(evt == EVT_SET3):
		SquareSize.val = maxSquareSize / 2.9
		Blender.Redraw()
	elif(evt == EVT_SET4):
		SquareSize.val = maxSquareSize / 3.9
		Blender.Redraw()
	elif(evt == EVT_INITIAL):
		addInitialPage = not(addInitialPage)
		Blender.Draw.Redraw()

Register(draw, event, bevent)
