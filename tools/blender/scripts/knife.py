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

# Note : I (wetneb) added some stuff so the UV coordinates are
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

###########################
######## UNUSED ###########
###########################

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
	# Valuta se una faccia a tutta da una parte, da un'altra, o a intersecata
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

	if(str(type(me)) == "<type 'Blender NMesh'>"):
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
	elif(str(type(me)) == "<type 'Blender Mesh'>"):
		me.verts.extend(f.v)

		if len(f.v) == 3:
			me.faces.extend(f.v[0], f.v[1], f.v[2])
		elif len(f.v) == 4:
			me.faces.extend(f.v[0], f.v[1], f.v[2], f.v[3])
		nf = me.faces[len(me.faces)-1]
		if len(f.uv):
			nf.uv = f.uv
		if f.image:
			nf = f.image

def FaceMake(me, vl, vu, image):
	#
	# Crea una o due nuove facce da un elenco di vertici
	#
	if(str(type(me)) == "<type 'Blender NMesh'>"):
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
	elif(str(type(me)) == "<type 'Blender Mesh'>"):
		me.verts.extend(vl)

		if (len(vl)<=4):
			me.faces.extend(vl)
			nf = me.faces[len(me.faces)-1]
			if(len(vu)):
				nf.uv.extend(vu)
			if(image):
				nf.image = image
		else:
			vl1 = (vl[0], vl[1], vl[2], vl[3])
			me.faces.extend(vl1)
			nf1 = me.faces[len(me.faces)-1]
			nf1.uv = (vu[0], vu[1], vu[2], vu[3])

			vl2 = (vl[3], vl[4], vl[0])
			me.faces.extend(vl2)
			nf2 = me.faces[len(me.faces)-1]
			nf2.uv = (vu[3], vu[4], vu[0])

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
	# Vabbua, scandiamoci tutte le faccine...
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
			# Un po' qua un po' la
			FaceSplit(Obj, MeshPos, MeshNeg, oface, Normal, d0)

