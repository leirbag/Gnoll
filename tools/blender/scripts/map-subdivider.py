 #####################################
#         MAP SUBDIVIDER            #
#####################################
#
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
#####################################
#               USING               #
#####################################
#
# This script cuts a mesh along a grid.
# Your mesh's propreties must be blank :
# No translation, rotation or scale.
# If not, the object's center will be
# moved at the center of the world,
# applying all the transformations to the
# mesh (as if it were done in edit mode).
#
# 1. Run the script
# 2. Select only one object (the one you
#    want to cut)
# 3. Press Subdivide !
# 4. Try again : change the SquareSize
#    parameter.
#
# I'm a newbie in Plug-in developping for
# Blender. This script can be improved :
# Optimize it, fix some bugs, add settings
# to control the cut in a better way...
# If you have any trouble using this script,
# just mail me.
#
# Enjoy !
#
#####################################
#             ALGORITHM             #
#####################################
#
# Requirements : we need to cut a mesh
# into equal squares. Each square must
# be a unique object and each object's
# center has to be at the center of the
# cutting square (even if the square
# isn't fully filled with faces).
#
# Steps :
# 1 - The script generates a list of
#     "planes" : each plane corresponds
#     to a cut line. I called it "planes"
#     because Blender Knife Tool needs
#     a plane to cut an object.
#
# 2 - Then it cuts the mesh into "columns" :
#     it just cuts the mesh with many
#     lines along the Y axis (which are
#     defined by "xPlanes" because the
#     only coordinate that change is the
#     X coordinate).
#     The columns aren't linked to the
#     scene since we don't want to keep it.
#
# 3 - Then it cuts each column into "pages":
#     A page is a grid's square (same process
#     as upper).
#
# 4 - Then it moves each object's center at
#     the right position (see requirements).

# Modules
import Blender
from Blender import *
from Blender import NMesh
from Blender import Mesh
from Blender.BGL import *
from Blender.Draw import *
from Blender.Scene import *
import math
from math import *

# Events
EVT_NOEVT = 1
EVT_QUIT = 2
EVT_SUBD = 3

# Settings
SquareSize = Create(1.0)
Epsilon = 0.00001

# Get and check settings
def getSettings():
	global SquareSize
	if(SquareSize.val == 0):
		print "Error : The size of the pages can't be null."
		Exit()

	selectedObj = Object.GetSelected()

	if(len(selectedObj) < 1):
		print "Error : You must select the object to subdivide."
		Exit()
	elif(len(selectedObj) > 1):
		print "Error : There are too many selected objects."
		Exit()
	elif(selectedObj[0].type != 'Mesh'):
		print "Error : The object must be a mesh."
	else:
		return (SquareSize.val, selectedObj[0])
	Exit()

# Globalize the mesh
def globalize(obj):
	if(obj.type == "Mesh"):
		mesh = obj.getData()

		i = 0
		for v in mesh.verts:
			(v.co[0], v.co[1], v.co[2]) = GlobalPosition(v.co, obj)
			mesh.verts[i] = v
			i = i + 1

		obj.setLocation(0,0,0)
		obj.rot = (0,0,0)
		obj.setSize(1,1,1)
		mesh.update()

# Read bounding box informations
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

	# Generate planes along the Y axis
	print "   Generate planes along the Y axis..."
	x = xmax - SquareSize.val
	while x > xmin:
		xPlanes.append(((x+1,0,0),(x,0,0)))
		print "      ", x
		x = x - SquareSize.val
 	print "  ", len(xPlanes), "planes along Y generated."

	# Generate places along the X axis
	print "   Generate planes along the X axis..."
	y = ymax - SquareSize.val
	while y > ymin:
		yPlanes.append(((0,y+1,0),(0,y,0)))
		print "      ", y
		y = y - SquareSize.val

	# Debug
	print "  ", len(yPlanes), "planes along X generated."

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

# Cut the mesh into columns (along the Y axis) or into pages (along the X axis)
def createColumns(rootObj, xPlanes, arePages, startingIndex, rootName, boundBox, squareSize):
	columns = []
	sce = Blender.Scene.GetCurrent()

	# Generate names
	finalName = '-Col-'
	tempName = '-Temp-'
	if(arePages):
		finalName = '-Page-'
		tempName = '-TempPg-'

	# Set if the algorithm has tu use UV coordinates
	useUVCoords = rootObj.getData().hasFaceUV()

	# Init the loop
	cuttingObject = rootObj
	i = 0

	while i < len(xPlanes):
		# Create a new object containing the new part
		newObj = Blender.Object.New('Mesh', rootName + finalName + str(startingIndex+i+1))
		tempObj = Blender.Object.New('Mesh', rootName + tempName + str(startingIndex+i+1))

		# Get the meshes
		newMe = newObj.getData()
		tempMe = tempObj.getData()
		newMe.hasFaceUV(useUVCoords)
		tempMe.hasFaceUV(useUVCoords)

		# Cut the object
		(CN,CC) = xPlanes[i];
		d0 = CC[0]*CN[0]+CC[1]*CN[1]+CC[2]*CN[2]
		Cut(cuttingObject, CN, d0, newMe, tempMe)

		# Update the objects
		newMe.update()
		tempMe.update()

		# If it's the last cut, the temp object is also a final object
		if (i == len(xPlanes)-1):
			# Add the temp object to the scene
			columns.append(tempObj)
			tempObj.setName(rootName + finalName + str(startingIndex+i+2))

			if(arePages): # Link to the scene if needed (only if we are generating pages)
				sce.objects.link(tempObj)

			i = i + 1
		if True:#else:
			cuttingObject = tempObj
			columns.append(newObj)

			if(arePages): # Link to the scene if needed (only if we are generating pages)
				sce.objects.link(newObj)

		i = i + 1

	return columns

# Map subdividing (main process)
def subdivide():
	### Print header ###
	print "
Grid Subdivide Tool v0.1 by Antonin <Wetneb> Delpeuch"
	print "Released under Blender Artistic Licence"
	print "Based on Blender Knife Tool v0.6 by Stefano <S68> Selleri"

	### Check settings and selected object ###
	(stepSize, rootObj) = getSettings()

	### Extract mesh and bounding box. ###
	globalize(rootObj)
	mesh = rootObj.getData(False, True)
	boundBox = getBoundBox(rootObj)

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

	print "Done. You can delete temporary objects (just save and restart Blender)."
	print "You should also remove all the double vertices using W -> Remove doubles in edit mode."
	return 0

# GUI Drawing
def draw():
	global SquareSize
	global EVT_NOEVT, EVT_QUIT, EVT_SUBD

	# Preparing
	glClear(GL_COLOR_BUFFER_BIT)

	# Drawing input controls
	SquareSize = Number("Size of pages: ", EVT_NOEVT, 10, 55, 170, 18, SquareSize.val,
		0.00001, 20, "Size of the squared pages.");

	# Drawing main buttons
	Button("Quit", EVT_QUIT, 10, 35, 80, 18)
	Button("Subdivide", EVT_SUBD, 100, 35, 80, 18)


# Event handling
def event(evt, val):
	if(evt == QKEY and not val):
		Exit()

def bevent(evt):
	global SquareSize
	global EVT_NOEVT, EVT_QUIT, EVT_SUBD

	if(evt == EVT_QUIT):
		Exit()
	elif(evt == EVT_SUBD):
		if(subdivide()):
			Exit()
		Blender.Redraw()

Register(draw, event, bevent)

#############################################################
#                                                           #
# Blender Knife Tool                                        #
#                                                           #
# (C) December 2002 Stefano <S68> Selleri                   #
# Released under the Blender Artistic Licence (BAL)         #
# See www.blender.org                                       #
#                                                           #
#############################################################
# History                                                   #
# V: 0.0.0 - 08-12-02 - The script starts to take shape, an #
#                       history is now deserved :)          #
#    0.0.1 - 09-12-02 - The faces are correctly selected and#
#                       assigned to the relevant objects    #
#                       now the hard (splitting) part...    #
#    0.0.2 - 14-12-02 - Still hacking on the splitting...   #
#                       It works, but I have to de-globalize#
#                       the intersection coordinates        #
#    0.0.3 - 15-12-02 - First Alpha version                 #
#    0.0.4 - 17-12-02 - Upgraded accordingly to eeshlo tips #
#                       Use Matrices for coordinate transf. #
#                       Add a GUI                           #
#                       Make it Run on 2.23                 #
#    0.0.5 - 17-12-02 - Eeshlo solved some problems....     #
#                       Theeth too adviced me               #
#    0.0.6 - 18-12-02 - Better error messages               #
#############################################################

# Note: I (wetneb) added some stuff so the UV coordinates are
# correctly handled.

import Blender
from Blender import *
from math import *

Epsilon = 0.00001
msg = ''

BL_VERSION = Blender.Get('version')
if (BL_VERSION<=223):
	import Blender210

#############################################################
# SUBS from LOCAL to GLOBAL coordinates                     #
#############################################################

def GlobalPosition(P, Obj):
	#
	# Prende un punto nelle coordinate locali (Vertice di Mesh)
	# E lo trasporta in coordinate globali
	#
	if (BL_VERSION<=223):
		m = Obj.matrix
	else:
		m = Obj.getMatrix()

	PX = P[0]*m[0][0] + P[1]*m[1][0] + P[2]*m[2][0] + m[3][0]
	PY = P[0]*m[0][1] + P[1]*m[1][1] + P[2]*m[2][1] + m[3][1]
	PZ = P[0]*m[0][2] + P[1]*m[1][2] + P[2]*m[2][2] + m[3][2]
	return (PX, PY, PZ)

def LocalPosition(P, Obj):
	#
	# Prende un punto nelle coordinate globali
	# E lo trasporta in coordinate locali a un ggetto  (Vertice di Mesh)
	#
	if (BL_VERSION<=223):
		m = Blender210.getObject(Obj.name).inverseMatrix
	else:
		m = Obj.getInverseMatrix()

	PX = P[0]*m[0][0] + P[1]*m[1][0] + P[2]*m[2][0] + m[3][0]
	PY = P[0]*m[0][1] + P[1]*m[1][1] + P[2]*m[2][1] + m[3][1]
	PZ = P[0]*m[0][2] + P[1]*m[1][2] + P[2]*m[2][2] + m[3][2]
	return (PX, PY, PZ)

#############################################################
# SUBS Extracts Cutting Plane Data                          #
#############################################################

def CutData(Plane):
	#
	# Traduce il secondo oggetto in un punto (Il suo centro)
	# ed in una direzione (la normale alla faccia 0)
	#
	if (BL_VERSION<=223):
		PlaneCenter = Plane.loc
	else:
		PlaneCenter = Plane.getLocation()

	PlaneMesh   = NMesh.GetRawFromObject(Plane.name)

	if (len(PlaneMesh.faces)>1):
		msg =  "ERROR: Active object must be a single face plane"
		return ((0,0,0),(0,0,0))
	else:
		if (len(PlaneMesh.verts)<3):
			msg = "ERROR: 3 vertices needed to define a plane"
			return ((0,0,0),(0,0,0))
		else:
			print "# OK:    Active object is a single face plane -> Getting Cut Data"
			v0 = GlobalPosition(PlaneMesh.faces[0].v[0].co, Plane)
			v1 = GlobalPosition(PlaneMesh.faces[0].v[1].co, Plane)
			v2 = GlobalPosition(PlaneMesh.faces[0].v[2].co, Plane)

			NormX   = (v1[1]-v0[1])*(v2[2]-v0[2]) - (v1[2]-v0[2])*(v2[1]-v0[1])
			NormY   = (v1[2]-v0[2])*(v2[0]-v0[0]) - (v1[0]-v0[0])*(v2[2]-v0[2])
			NormZ   = (v1[0]-v0[0])*(v2[1]-v0[1]) - (v1[1]-v0[1])*(v2[0]-v0[0])
			NormLen = sqrt(NormX*NormX+NormY*NormY+NormZ*NormZ)
			NormX  /= NormLen
			NormY  /= NormLen
			NormZ  /= NormLen

			return PlaneCenter, (NormX, NormY, NormZ)

#############################################################
# SUBS giving position with respect to Cut Plane            #
#############################################################

def Distance(P, N, d0):
	#
	# Ok, quanto dista un punto dal piano?
	#
	return P[0]*N[0] + P[1]*N[1] + P[2]*N[2]-d0

def FacePosition(f, Obj, N, d0):
	#
	# Valuta se una faccia Ã¨ tutta da una parte, da un'altra, o Ã¨ intersecata
	#
	np, nn, nz = 0, 0, 0

	for v in f.v:
		V = GlobalPosition(v.co, Obj)
		d = Distance(V, N, d0)

		if (d>0):
			np += 1
		elif (d<0):
			nn += 1
		else:
			nz += 1

	if (np==0):
		return -1
	if (nn==0):
		return 1
	return 0

#############################################################
# SUBS Appending existing faces or creating new faces for   #
#      cutted objects                                       #
#############################################################

def FaceAppend(me, f):
	#
	# Aggiunge una faccia senza nessuna elaborazione
	#
	idx = len(me.verts)
	for v in f.v:
		me.verts.append(NMesh.Vert(v.co[0], v.co[1], v.co[2]))

	nf = NMesh.Face()
	for i in range(len(f.v)):
		nf.v.append(me.verts[idx+i])
	for i in range(len(f.uv)):
		nf.uv.append(f.uv[i])
	if(f.image):
		nf.image = f.image
	me.faces.append(nf)

def FaceMake(me, vl, vu, image):
	#
	# Crea una o due nuove facce da un elenco di vertici
	#
	idx = len(me.verts)

	for v in vl:
		me.verts.append(NMesh.Vert(v[0], v[1], v[2]))

 	if (len(vl)<=4):
		nf = NMesh.Face()
		for i in range(len(vl)):
			nf.v.append(me.verts[idx+i])
			if(len(vu)):
				nf.uv.append(vu[i])

		if(image):
			nf.image = image
		me.faces.append(nf)
	else:
		nf = NMesh.Face()
		nf.v.append(me.verts[idx])
		nf.v.append(me.verts[idx+1])
		nf.v.append(me.verts[idx+2])
		nf.v.append(me.verts[idx+3])
		if(image):
			nf.image = image
		if(len(vu)):
			nf.uv.append(vu[0])
			nf.uv.append(vu[1])
			nf.uv.append(vu[2])
			nf.uv.append(vu[3])
		me.faces.append(nf)

		nf = NMesh.Face()
		nf.v.append(me.verts[idx+3])
		nf.v.append(me.verts[idx+4])
		nf.v.append(me.verts[idx])
		if(image):
			nf.image = image
		if(len(vu)):
			nf.uv.append(vu[3])
			nf.uv.append(vu[4])
			nf.uv.append(vu[0])
		me.faces.append(nf)

#############################################################
# SUBS Generating vertex lists for splitting faces          #
#############################################################

def Split(Obj, f, N, d0):
	#
	# Genera una lista di vertici da una parte e dall'altra e relativi punti intermedi
	#

	i = 0
	V = []
	d = []
	v = f.v

	vp = []
	vpu = []
	vn = []
	vnu = []

	for vl in v:
		V.append(GlobalPosition(vl.co, Obj))
		d.append(Distance(V[i], N, d0))
		i += 1

	for i in range(0,len(d)):
		if (i==0):
			dim1 = d[len(d)-1]
			Vim1 = V[len(V)-1]

			# UV Coords...
			if(len(f.uv)):
				Viu1 = f.uv[len(f.uv)-1]
		else:
			dim1 = d[i-1]
			Vim1 = V[i-1]

			# UV Coords...
			if(len(f.uv)):
				Viu1 = f.uv[i-1]

		if (abs(d[i])<Epsilon):
			# Appartiene a entrambi
			vp.append(tuple(v[i].co))
			vn.append(tuple(v[i].co))

			# UV Coords...
			if(len(f.uv)):
				vpu.append(tuple(f.uv[i]))
				vnu.append(tuple(f.uv[i]))
		else:
			if (abs(dim1)<Epsilon):
				# Quello di prima era sul contorno
				if (d[i]>0):
					vp.append(tuple(v[i].co))
					if(len(f.uv)):
						vpu.append(tuple(f.uv[i]))
				else:
					vn.append(tuple(v[i].co))
					if(len(f.uv)):
						vnu.append(tuple(f.uv[i]))
			else:
				if (d[i]*dim1>0):
					# Nessun cambio netto, nessuna intersezione
					if (d[i]>0):
						vp.append(tuple(v[i].co))
						if(len(f.uv)):
							vpu.append(tuple(f.uv[i]))
					else:
						vn.append(tuple(v[i].co))
						if(len(f.uv)):
							vnu.append(tuple(f.uv[i]))
				else:
					# INTERSEZIONE!!!
					Den = (Vim1[1]-V[i][1])*N[1] + (Vim1[0]-V[i][0])*N[0] + (Vim1[2]-V[i][2])*N[2]

					Vi = []
					Vi.append (- ((Vim1[0]*V[i][1]-Vim1[1]*V[i][0])*N[1]+(Vim1[0]*V[i][2]-Vim1[2]*V[i][0])*N[2]+(V[i][0]-Vim1[0])*d0)/Den)
					Vi.append (- ((Vim1[1]*V[i][0]-Vim1[0]*V[i][1])*N[0]+(Vim1[1]*V[i][2]-Vim1[2]*V[i][1])*N[2]+(V[i][1]-Vim1[1])*d0)/Den)
					Vi.append (- ((Vim1[2]*V[i][0]-Vim1[0]*V[i][2])*N[0]+(Vim1[2]*V[i][1]-Vim1[1]*V[i][2])*N[1]+(V[i][2]-Vim1[2])*d0)/Den)

					ViL = LocalPosition(Vi, Obj)

					vp.append(ViL)
					vn.append(ViL)

					# UV Coords managment
					if(len(f.uv)):
						cutRatio = fabs(dim1)/(fabs(dim1)+fabs(d[i]))
						Viu = (Viu1[0] + (f.uv[i][0] - Viu1[0])*cutRatio),(Viu1[1] + (f.uv[i][1] - Viu1[1])*cutRatio)
						vpu.append(Viu)
						vnu.append(Viu)

					# E il punto stesso...
					if (d[i]>0):
						vp.append(tuple(v[i].co))
						if(len(f.uv)):
							vpu.append(tuple(f.uv[i]))
					else:
						vn.append(tuple(v[i].co))
						if(len(f.uv)):
							vnu.append(tuple(f.uv[i]))

	return ((vp, vpu),(vn, vnu))

#############################################################
# SUBS Splitting a face intersected by the Cut Plane        #
#############################################################

def FaceSplit(Obj, mp, mn, f, N, d0):
	#
	# Divide la faccia in due pezi e crea le facce necessarie
	#

	# Crea le liste dei vertici
	((vlp,vlpu), (vln, vlnu)) = Split(Obj, f, N, d0)

	# Crea nuove facce coi nuovi vertici
	FaceMake(mp, vlp, vlpu, f.image)
	FaceMake(mn, vln, vlnu, f.image)

#############################################################
# SUBS Appending to two new meshes all the faces of the     #
# original one, splitting the intersected ones              #
#############################################################

def Cut(Obj, Normal, d0, MeshPos, MeshNeg):
	#
	# VabbuÃ², scandiamoci tutte le faccine...
	#
	if BL_VERSION<=223:
		ObjMesh = Obj.data
	else:
		ObjMesh = Obj.getData()

	for oface in ObjMesh.faces:
		fp = FacePosition(oface, Obj, Normal, d0)
		if (fp>0):
			# Tutta nel positivo
			FaceAppend(MeshPos, oface)
		if (fp<0):
			# Tutta nel negativo
			FaceAppend(MeshNeg, oface)
		if (fp==0):
			# Un po' qua un po' lÃ
			FaceSplit(Obj, MeshPos, MeshNeg, oface, Normal, d0)
