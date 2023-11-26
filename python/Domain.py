import meshio
from Element import GenericElement, Tetrahedron
import numpy as np

class Domain:

    def __init__(self, filename, nEletrodes) -> None:
        self.mesh = meshio.read(filename)
        self.mapNodes = []
        self.nEletrodes = nEletrodes
        self.nNodes = len(self.mesh.points)
        self.m_global = np.zeros((self.nNodes, self.nNodes))

        for element in self.mesh.cells:
            if (element.type == 'tetra'):
                for x, e in zip(range(0,len(element.data)), element.data):
                    obj = Tetrahedron(x, element.type, e)
                    for y, p in zip(range(0, len(e.data)), e.data):
                        obj.setPoint(y, self.mesh.points[p])

                    obj.calcLocalMatrix()
                    self.mapNodes.append(obj)
        self.nElements = len(self.mapNodes)
    
    def findCenter(self):
        self.pos_max = np.array([np.max(self.mesh.points[:,0]), np.max(self.mesh.points[:,1]) , np.max(self.mesh.points[:,2])])
        self.pos_min = np.array([np.min(self.mesh.points[:,0]), np.min(self.mesh.points[:,1]) , np.min(self.mesh.points[:,2])])
        self.pos_span = self.pos_max - self.pos_min
        self.pos_center = self.pos_span/2.0 + self.pos_min
        self.min_dist = np.linalg.norm(self.pos_span)
        for x in range(0,self.nNodes):
            dist = np.linalg.norm(self.mesh.points[x]-self.pos_center)
            if (dist < self.min_dist):
                self.center_node = x
                self.min_dist = dist


    def updateGlobal(self, condutividade):
        self.m_global = np.zeros((self.nNodes, self.nNodes))
        for x, element in zip(range(0,self.nElements), self.mapNodes):
            element.updateGlobal(self.m_global, condutividade[x])

    def saveNodeValues(self, filename, values):
        f = open(filename, "w")
        f.write('$MeshFormat\n2 0 8\n$EndMeshFormat\n$NodeData\n1\n"Tensao (V)"\n1\n0.0\n3\n0\n1\n')
        f.write('{}\n'.format(self.nNodes))
        for x in range(0, self.nNodes):
            f.write('{} {}\n'.format(x+1, values[x]))
        f.write('$EndNodeData\n')
        f.close()