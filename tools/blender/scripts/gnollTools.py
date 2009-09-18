
#####################################
#           MAP SUBDIVIDER          #
#####################################
# Antonin Delpeuch <Wetneb>

##### GNOLL INTEGRATING TOOLS #####
# This classes and functions are
# designed to allow scene files
# generating in the Grid Subdivide
# tool. The resulting files can be
# used with Gnoll.

__author__ = 'Antonin <Wetneb> Delpeuch'

from xml.dom import *

# Global used to create a unique identifier for each GOC
gocId = 0

# Get a new unique GOC instance name
def getGocId():
	global gocId
	gocId = gocId + 1
	return "g"+str(gocId)

# Transforms "My-Test-Object.012" to "My-Test-Object" (useful for removing dupplicates)
def getCleanName(origName):
	result = origName
	if not origName.rfind(".0", 0, len(origName)) == -1:
		result = origName.rpartition('.0')[0]
	return result

# Defines an additionnal object on a page
class ObjectDef:
	def __init__(self, name="", version=1):
		# Syntax
		self.version = version

		# Attributes
		self.name = name
		
		# Location
		self.locX = 0.0
		self.locY = 0.0
		self.locZ = 0.0
		
		# Rotation
		self.rotX = 0.0
		self.rotY = 0.0
		self.rotZ = 0.0
		
		# Size
		self.sizeX = 0.0
		self.sizeY = 0.0
		self.sizeZ = 0.0
		
	def write(self, doc):
		if self.version==1:
			rootElem = doc.createElement("cstaticgobject")
			rootElem.setAttribute("instance", "m7_" + self.name)

			meshElem = doc.createElement("attribute")
			meshElem.setAttribute("name", "mesh")
			meshStr = doc.createElement("string")
			meshStr.setAttribute("value", getCleanName(self.name) + ".mesh")
			meshElem.appendChild(meshStr)
			rootElem.appendChild(meshElem)

			if self.locX:
				posXElem = doc.createElement("attribute")
				posXElem.setAttribute("name", "pos_x")
				posXFloat = doc.createElement("float")
				posXFloat.setAttribute("value", str(self.locX))
				posXElem.appendChild(posXFloat)
				rootElem.appendChild(posXElem)

			if self.locY:
				posYElem = doc.createElement("attribute")
				posYElem.setAttribute("name", "pos_y")
				posYFloat = doc.createElement("float")
				posYFloat.setAttribute("value", str(self.locY))
				posYElem.appendChild(posYFloat)
				rootElem.appendChild(posYElem)

			if self.locZ:
				posZElem = doc.createElement("attribute")
				posZElem.setAttribute("name", "pos_z")
				posZFloat = doc.createElement("float")
				posZFloat.setAttribute("value", str(self.locZ))
				posZElem.appendChild(posZFloat)
				rootElem.appendChild(posZElem)
			
			if self.rotX:
				rotXElem = doc.createElement("attribute")
				rotXElem.setAttribute("name", "rot_x")
				rotXFloat = doc.createElement("float")
				rotXFloat.setAttribute("value", str(self.rotX))
				rotXElem.appendChild(rotXFloat)
				rootElem.appendChild(rotXElem)
			
			if self.rotY:
				rotYElem = doc.createElement("attribute")
				rotYElem.setAttribute("name", "rot_y")
				rotYFloat = doc.createElement("float")
				rotYFloat.setAttribute("value", str(self.rotY))
				rotYElem.appendChild(rotYFloat)
				rootElem.appendChild(rotYElem)
			
			if self.rotZ:
				rotZElem = doc.createElement("attribute")
				rotZElem.setAttribute("name", "rot_z")
				rotZFloat = doc.createElement("float")
				rotZFloat.setAttribute("value", str(self.rotZ))
				rotZElem.appendChild(rotZFloat)
				rootElem.appendChild(rotZElem)
			
			if not self.sizeZ == 1:
				sizeZElem = doc.createElement("attribute")
				sizeZElem.setAttribute("name", "scale_z")
				sizeZFloat = doc.createElement("float")
				sizeZFloat.setAttribute("value", str(self.sizeZ))
				sizeZElem.appendChild(sizeZFloat)
				rootElem.appendChild(sizeZElem)
			
			if not self.sizeY == 1:
				sizeYElem = doc.createElement("attribute")
				sizeYElem.setAttribute("name", "scale_y")
				sizeYFloat = doc.createElement("float")
				sizeYFloat.setAttribute("value", str(self.sizeY))
				sizeYElem.appendChild(sizeYFloat)
				rootElem.appendChild(sizeYElem)
			
			if not self.sizeX == 1:
				sizeXElem = doc.createElement("attribute")
				sizeXElem.setAttribute("name", "scale_x")
				sizeXFloat = doc.createElement("float")
				sizeXFloat.setAttribute("value", str(self.sizeX))
				sizeXElem.appendChild(sizeXFloat)
				rootElem.appendChild(sizeXElem)
			
			return rootElem
		elif self.version==2:
			rootElem = doc.createElement("gobject")
			rootElem.setAttribute("instance", "go_" + self.name)

			# Root structure
			goElem = doc.createElement("attribute")
			goElem.setAttribute("name", "components")
			lstGoc = doc.createElement("list")

			# Mesh name
			meshGocElem = doc.createElement("COgreMeshComponent")
			meshGocElem.setAttribute("instance", getGocId())
			meshElem = doc.createElement("attribute")
			meshElem.setAttribute("name", "mesh")
			meshStr = doc.createElement("string")
			meshStr.setAttribute("value", getCleanName(self.name) + ".mesh")
			meshElem.appendChild(meshStr)
			meshGocElem.appendChild(meshElem)
			lstGoc.appendChild(meshGocElem)

			# Position
			posGocElem = doc.createElement("CPositionComponent")
			posGocElem.setAttribute("instance", getGocId())
			posElem = doc.createElement("attribute")
			posElem.setAttribute("name", "position")
			posVec = doc.createElement("vector3")
			posVec.setAttribute("x", str(self.locX))
			posVec.setAttribute("y", str(self.locY))
			posVec.setAttribute("z", str(self.locZ))
			posElem.appendChild(posVec)
			posGocElem.appendChild(posElem)
			lstGoc.appendChild(posGocElem)

			# Rotation
			rotGocElem = doc.createElement("CRotationComponent")
			rotGocElem.setAttribute("instance", getGocId())
			rotElem = doc.createElement("attribute")
			rotElem.setAttribute("name", "rotation")
			rotVec = doc.createElement("vector3")
			rotVec.setAttribute("x", str(self.rotX))
			rotVec.setAttribute("y", str(self.rotY))
			rotVec.setAttribute("z", str(self.rotZ))
			rotElem.appendChild(rotVec)
			rotGocElem.appendChild(rotElem)
			lstGoc.appendChild(rotGocElem)

			# Scale
			scaleGocElem = doc.createElement("CScalingComponent")
			scaleGocElem.setAttribute("instance", getGocId())
			scaleElem = doc.createElement("attribute")
			scaleElem.setAttribute("name", "scale")
			scaleVec = doc.createElement("vector3")
			scaleVec.setAttribute("x", str(self.sizeX))
			scaleVec.setAttribute("y", str(self.sizeY))
			scaleVec.setAttribute("z", str(self.sizeZ))
			scaleElem.appendChild(scaleVec)
			scaleGocElem.appendChild(scaleElem)
			lstGoc.appendChild(scaleGocElem)

			# Close list
			goElem.appendChild(lstGoc)
			rootElem.appendChild(goElem)

			return rootElem

class PageFile:
	def __init__(self, name = "", version=1, size = 1.0):
		# Syntax
		self.version = version

		# Attributes
		self.name = name
		self.size = size
		
		# Neighbours
		self.east = ""
		self.north = ""
		self.west = ""
		self.south = ""
		
		# Additional objects
		self.objects = []
		
	def writeToFile(self, filePath):
		# Create document
		doc = getDOMImplementation().createDocument(None, "dynamicobject", None)
		doc.documentElement.setAttribute("instance", self.name)

		# Create main structure
		pageRenderer = doc.createElement("attribute")
		pageRenderer.setAttribute("name", "PageRenderer")
		mesh = doc.createElement("CMeshPageRenderer")
		mesh.setAttribute("meshName", self.name + ".mesh")
		pageRenderer.appendChild(mesh)
		doc.documentElement.appendChild(pageRenderer)

		# Add the size
		sizeElem = doc.createElement("attribute")
		sizeElem.setAttribute("name", "size")
		sizeValue = doc.createElement("float")
		sizeValue.setAttribute("value", str(self.size))
		sizeElem.appendChild(sizeValue)
		doc.documentElement.appendChild(sizeElem)

		# Add the links
		# East link
		if len(self.east):
			linkElem = doc.createElement("attribute")
			linkElem.setAttribute("name", "eastLink")
			valueElem = doc.createElement("string")
			valueElem.setAttribute("value", self.east)
			linkElem.appendChild(valueElem)
			doc.documentElement.appendChild(linkElem)

		# North link
		if len(self.north):
			linkElem = doc.createElement("attribute")
			linkElem.setAttribute("name", "northLink")
			valueElem = doc.createElement("string")
			valueElem.setAttribute("value", self.north)
			linkElem.appendChild(valueElem)
			doc.documentElement.appendChild(linkElem)

		# West link
		if len(self.west):
			linkElem = doc.createElement("attribute")
			linkElem.setAttribute("name", "westLink")
			valueElem = doc.createElement("string")
			valueElem.setAttribute("value", self.west)
			linkElem.appendChild(valueElem)
			doc.documentElement.appendChild(linkElem)

		# South link
		if len(self.south):
			linkElem = doc.createElement("attribute")
			linkElem.setAttribute("name", "southLink")
			valueElem = doc.createElement("string")
			valueElem.setAttribute("value", self.south)
			linkElem.appendChild(valueElem)
			doc.documentElement.appendChild(linkElem)

		# Add the objects
		if len(self.objects):
			objElem = doc.createElement("attribute")
			if self.version==1:
				objElem.setAttribute("name", "staticObjects")
			elif self.version==2:
				objElem.setAttribute("name", "gameObjects")
			listElem = doc.createElement("list")
			objElem.appendChild(listElem)

			for obj in self.objects:
				listElem.appendChild(obj.write(doc))

			doc.documentElement.appendChild(objElem)

		# Write out the document
		file = open(filePath + "/" + self.name, "w")
		doc.writexml(file, "", "  ", "\n", "UTF-8")

