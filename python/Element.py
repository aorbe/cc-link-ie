import logging
logging.basicConfig(level=logging.WARNING)
import numpy as np

class GenericElement:
	number = 0
	type = 0
	nNodes = 0
	local = None
	condutividade = 0.0
	nodes = None

	def __init__(self, number, type, nodes):
		self.number = number
		self.type = type
		self.nodes = nodes
		self.nNodes = len(self.nodes)

	def __del__(self):
		None

	def determinante(self, va, vb, vc):
		volume = 0.0
		volume  = va[0]*vb[1]*vc[2] + va[1]*vb[2]*vc[0] + va[2]*vb[0]*vc[1]
		volume -= va[2]*vb[1]*vc[0] + va[1]*vb[0]*vc[2] + va[0]*vb[2]*vc[1]
		return volume

	def showLocal(self):
		x = 0
		for r in range(0,self.nNodes):
			for c in range (0, self.nNodes):
				print("{0:6.5f} ".format(self.local[r][c]), end='')
			print("")


	def updateGlobal(self, matrix : np.matrix, condutividade):
		self.condutividade = condutividade
		logging.debug("updating global [{0:7d}]".format(self.number))

		for x, r in zip(range(0,self.nNodes),self.nodes):
			for y, c in zip(range(0,self.nNodes),self.nodes):
				d = matrix[r,c]
				d += (1/(self.condutividade)* self.local[x,y])
				matrix[r,c] = d

	def setPoint(self, n, point):
		self.points[n,:] = point

	def getDerivCondut(self):
		return -1/(self.condutividade*self.condutividade)

	def type_name(self):
		return self.type

	def getNumberNodes(self):
		return self.nNodes

	def getNode(self, idx: int):
		if (idx < self.nNodes):
			return self.nodes[idx]
		return 0

	def getNumber(self):
		return self.number

	def getType(self):
		return self.type

class Tetrahedron(GenericElement):
	volume = 0.0
	
	def calcVolume(self):
		M = np.ndarray([3,3])
		M[0,:] = self.points[1,:] - self.points[0,:]
		M[1,:] = self.points[2,:] - self.points[0,:]
		M[2,:] = self.points[3,:] - self.points[0,:]
		self.volume = 1/6 * np.linalg.det(M)
		if (self.volume < 0):
			logging.error("Volume do Tetraedo {} = {}".format(self.number, self.volume))

	def __init__(self, number, type, nodes) :
		GenericElement.__init__(self,number, type, nodes)
		self.points = np.ndarray([self.nNodes, 3])
		self.local = np.ndarray([self.nNodes, self.nNodes])
		self.jacob = np.ndarray(self.nNodes)
	
	def __del__():
		GenericElement.__del__()

	def calcLocalMatrix(self):
		logging.info("starting local matrix calculation [{}]".format(self.number))
		self.calcVolume()

		# u1 = list_nodes[self.nodes[0]]
		# u2 = list_nodes[self.nodes[1]]
		# u3 = list_nodes[self.nodes[2]]
		# u4 = list_nodes[self.nodes[3]]

		a1 = np.ndarray(shape=(3,3), buffer=np.array(self.points[1:4,0:3]))
		a2 = np.ndarray(shape=(3,3), buffer=np.array([self.points[0,0:3], self.points[2,0:3], self.points[3,0:3]]))
		a3 = np.ndarray(shape=(3,3), buffer=np.array([self.points[0,0:3], self.points[1,0:3], self.points[3,0:3]]))
		a4 = np.ndarray(shape=(3,3), buffer=np.array(self.points[0:3,0:3]))

		# b1 = self.points[2,1]*self.points[1,2] + self.points[1,1]*self.points[3,2] + self.points[3,1]*self.points[2,2] - \
		# 	 self.points[2,1]*self.points[3,2] - self.points[1,1]*self.points[2,2] - self.points[2,1]*self.points[1,2]
		# b2 = self.points[2,1]*self.points[1,2] + self.points[1,1]*self.points[3,2] + self.points[3,1]*self.points[2,2] - \
		# 	 self.points[2,1]*self.points[3,2] - self.points[1,1]*self.points[2,2] - self.points[2,1]*self.points[1,2]
		# b3 = self.points[2,1]*self.points[1,2] + self.points[1,1]*self.points[3,2] + self.points[3,1]*self.points[2,2] - \
		# 	 self.points[2,1]*self.points[3,2] - self.points[1,1]*self.points[2,2] - self.points[2,1]*self.points[1,2]
		# b4 = self.points[2,1]*self.points[1,2] + self.points[1,1]*self.points[3,2] + self.points[3,1]*self.points[2,2] - \
		# 	 self.points[2,1]*self.points[3,2] - self.points[1,1]*self.points[2,2] - self.points[2,1]*self.points[1,2]
		b1 = np.linalg.det(np.ndarray(shape=(3,3), buffer=np.array([np.ones(3), a1[0:3,2], a1[0:3,1]])))
		c1 = np.linalg.det(np.ndarray(shape=(3,3), buffer=np.array([np.ones(3), a1[0:3,0], a1[0:3,2]])))
		d1 = np.linalg.det(np.ndarray(shape=(3,3), buffer=np.array([np.ones(3), a1[0:3,1], a1[0:3,0]])))
		b2 = np.linalg.det(np.ndarray(shape=(3,3), buffer=np.array([np.ones(3), a2[0:3,1], a2[0:3,2]])))
		c2 = np.linalg.det(np.ndarray(shape=(3,3), buffer=np.array([np.ones(3), a2[0:3,2], a2[0:3,0]])))
		d2 = np.linalg.det(np.ndarray(shape=(3,3), buffer=np.array([np.ones(3), a2[0:3,0], a2[0:3,1]])))
		b3 = np.linalg.det(np.ndarray(shape=(3,3), buffer=np.array([np.ones(3), a3[0:3,2], a3[0:3,1]])))
		c3 = np.linalg.det(np.ndarray(shape=(3,3), buffer=np.array([np.ones(3), a3[0:3,0], a3[0:3,2]])))
		d3 = np.linalg.det(np.ndarray(shape=(3,3), buffer=np.array([np.ones(3), a3[0:3,1], a3[0:3,0]])))
		b4 = np.linalg.det(np.ndarray(shape=(3,3), buffer=np.array([np.ones(3), a4[0:3,1], a4[0:3,2]])))
		c4 = np.linalg.det(np.ndarray(shape=(3,3), buffer=np.array([np.ones(3), a4[0:3,2], a4[0:3,0]])))
		d4 = np.linalg.det(np.ndarray(shape=(3,3), buffer=np.array([np.ones(3), a4[0:3,0], a4[0:3,1]])))

		
		self.local[0,0] = 1/(36*self.volume)*(b1*b1+c1*c1+d1*d1)
		self.local[0,1] = 1/(36*self.volume)*(b1*b2+c1*c2+d1*d2)
		self.local[0,2] = 1/(36*self.volume)*(b1*b3+c1*c3+d1*d3)
		self.local[0,3] = 1/(36*self.volume)*(b1*b4+c1*c4+d1*d4)
		self.local[1,0] = self.local[0,1]
		self.local[1,1] = 1/(36*self.volume)*(b2*b2+c2*c2+d2*d2)
		self.local[1,2] = 1/(36*self.volume)*(b2*b3+c2*c3+d2*d3)
		self.local[1,3] = 1/(36*self.volume)*(b2*b4+c2*c4+d2*d4)
		self.local[2,0] = self.local[0,2]
		self.local[2,1] = self.local[1,2]
		self.local[2,2] = 1/(36*self.volume)*(b3*b3+c3*c3+d3*d3)
		self.local[2,3] = 1/(36*self.volume)*(b3*b4+c3*c4+d3*d4)
		self.local[3,0] = self.local[0,3]
		self.local[3,1] = self.local[1,3]
		self.local[3,2] = self.local[2,3]
		self.local[3,3] = 1/(36*self.volume)*(b4*b4+c4*c4+d4*d4)
		logging.info('finish local matrix calculation ({})'.format(self.number))
		return 0

