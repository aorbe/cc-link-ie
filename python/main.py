# Processamento de Malha

import logging
logging.basicConfig(level=logging.DEBUG)

# pip install gmshparser
import pymesh
import meshio
import numpy as np
from Element import GenericElement, Tetrahedron
import matplotlib.pyplot as plt


# using namespace std;

mesh = []
mGlobal = []
#mGlobal 
# gsl_matrix* global;
# gsl_matrix* inversa;
# gsl_permutation* permutation;
# gsl_vector *vI, *vP;


# map<unsigned int, ELEMENT*> list_elements;
# POINT* nodes = NULL;



def showMenu():
	option = -1
	print("MENU PRINCIPAL\n");
	print("1 - Ler arquivo msh\n");
	print("2 - Mostrar matriz local\n");
	print("3 - Calcular matriz global\n");
	print("4 - Mostrar matriz global\n");
	print("5 - Calcular matriz inversa\n");
	print("6 - Imprimir Voltagem\n");
	print("7 - Calcular voltagem\n");
	print("0 - Sair\n");
	print("Selecione a opção:");
	option = int(input())
	return option


def readFile(filename):
	mesh = meshio.read(filename)
	initial_value = 0.25;  # condutividade 0.5ohm/m
	print(mesh)
	return mesh

def calcArea(nodes):
	return

def main():
	global mesh
	option = -1
# 	unsigned long col_ini, col_end, row_ini, row_end;
# 	int signum = 0;
# 	char filename[256];
	mesh = readFile("D:\\dev\\reconstrucao\\Malha\\cuba1.msh")
	print(mesh)
	print("Calculando Matriz Global ....")

	mapNodes = []
	index_nodes = 0

	# list_nodes = {}
	# for node in mesh.points:
	# 	print(node)
		# print("{} {}".format(node.get_tag(), node.get_number_of_nodes()))
		# if (node.get_tag()== 2):
			# for n in node.get_nodes():
			# 	list_nodes.insert(n.. n.get_coordinates())

	nNodes = len(mesh.points)
	m_global = np.zeros((nNodes, nNodes))
	for element in mesh.cells:
		if (element.type == 'tetra'):
			for x, e in zip(range(0,len(element.data)), element.data):
				obj = Tetrahedron(x, element.type, e)
				for y, p in zip(range(0, len(e.data)), e.data):
					obj.setPoint(y, mesh.points[p])
				obj.calcLocalMatrix()
				# obj.showLocal()
				obj.updateGlobal(m_global, condutividade=10)
				mapNodes.append(obj)
	
	m_inverse = np.linalg.inv(m_global)
	
	m_current = np.zeros((nNodes))
	m_current[0] = 1
	m_current[3] = -1

	m_voltage = np.linalg.solve(m_global, m_current)
	# print(m_voltage)
	# plt.plot(m_voltage)
	# plt.show()

	# f = open("voltage.msh", "w")
	# f.write('$MeshFormat\n2 0 8\n$EndMeshFormat\n$NodeData\n1\n"Tensao (V)"\n1\n0.0\n3\n0\n1\n')
	# f.write('{}\n'.format(len(mesh.points)))
	# for x in range(0, len(mesh.points)):
	# 	f.write('{} {}\n'.format(x+1, m_voltage[x]))
	# f.write('$EndNodeData\n')
	# f.close()
	# # print(min_num)	
	# print(len(mapNodes))
	# for e in mapNodes:
	# 	local_nodes = [mesh.points[e[2]], mesh.points[e[1]], mesh.points[e[0]]]
	# 	area = 0
	# 	area =  local_nodes[0][0] * local_nodes[1][1] - local_nodes[1][0] * local_nodes[0][1]
	# 	area += local_nodes[2][0] * local_nodes[0][1] - local_nodes[0][0] * local_nodes[2][1]
	# 	area += local_nodes[1][0] * local_nodes[2][1] - local_nodes[2][0] * local_nodes[1][1]
	# 	if (area <= 0):
	# 		print("Erro Elemento {} (A ({})<= 0)".format(1, area))
	# 		#print(" Nos {%lu} {%lu} {%lu}\n".format(e));
		



	# while (option):
	# 	option = showMenu()
	# 	if option == 0:
	# 		break
	# 	elif option == 1:
	# 		filename = input("Nome do Arquivo: ")
	# 		mesh = readFile(filename)
	# 		# readFile("Malha\\teste.msh")
	# 	elif option == 3:

#     		if (list_elements.empty())
#     		{
#     			printf("Não há elementos registrados\n");
#     			break;
#     		}
#     		unsigned int nElemento;
#     		printf("\nDigite o número do elemento: ");
#     		scanf("%u", &nElemento);
#     		if (list_elements.find(nElemento) != list_elements.end())
#     		{
#     			((ELEMENT *) list_elements[nElemento])->showLocal();
#     		}
#     		break;
#     	case 4:
#     		printf("Linha Inicial:");    	scanf("%lu", &row_ini);
#     		printf("Linha Final:");    		scanf("%lu", &row_end);
#     		printf("Coluna Inicial:");   	scanf("%lu", &col_ini);
#     		printf("Coluna Final:");    	scanf("%lu", &col_end);
#     		printf("\nMatriz Global\n");
#     		for(unsigned long row=row_ini; row<row_end; row++)
#     		{
#     			for(unsigned long col=col_ini; col < col_end; col++)
#     			{
#     				printf("%f ", gsl_matrix_get(global, row, col));
#     			}
#     			printf("\n");
#     		}
#     		FILE* fGlobal;
#     		fGlobal= fopen("/home/asantos/Downloads/global.txt", "w+");
#     		gsl_matrix_fprintf(fGlobal,global, " %f");
#     		fclose(fGlobal);

#     		break;
#     	case 5:
#     		printf("\nCalculando Decomposição ...");
#     		permutation = gsl_permutation_calloc (global->size1);
#     		gsl_linalg_LU_decomp(global, permutation, &signum);
#     		//gsl_linalg_LU_invert(global, permutation, inversa);
#     		printf("Pronto\n\n");
#     		break;

#     	case 6:
#     		printf("Linha Inicial:");    	scanf("%lu", &row_ini);
#     		printf("Linha Final:");    		scanf("%lu", &row_end);
#     		printf("Coluna Inicial:");   	scanf("%lu", &col_ini);
#     		printf("Coluna Final:");    	scanf("%lu", &col_end);
#     		printf("\nMatriz Inversa\n");
#     		for(unsigned long row=row_ini; row<row_end; row++)
#     		{
#     			for(unsigned long col=col_ini; col < col_end; col++)
#     			{
#     				printf("%f ", gsl_matrix_get(inversa, row, col));
#     			}
#     			printf("\n");
#     		}
#     		break;
#     	case 7:
#     	{
#     		unsigned long nodeInp, nodeOut;
#     		double input;
#     		printf("\nAlocando vetores de tamanho %lu\n", global->size1);
#     		vI = gsl_vector_alloc(global->size1);
#     		vP = gsl_vector_alloc(global->size1);
#     		gsl_vector_set_all(vI, 0);
#     		printf("\nCorrente de Entrada: ");
#     		scanf("%lf", &input);
#     		printf("Input Node: ");
#     		scanf("%lu", &nodeInp);
#     		printf("Output Node: ");
#     		scanf("%lu", &nodeOut);
#     		gsl_vector_set(vI, nodeInp - 1, input);
#     		gsl_vector_set(vI, nodeOut - 1, -input);
#     		gsl_linalg_LU_solve(global, permutation, vI, vP);
#     		gsl_vector_fprintf(stdout, vP, "%f");
#     		FILE *resultado;
#     		resultado = fopen("/home/asantos/Downloads/final.txt", "w+");
#     		gsl_vector_fprintf(resultado, vP, "%f");
#     		fclose(resultado);

#     	}
#     		break;

#     	}
#     }

#     for(std::map<unsigned int, ELEMENT*>::iterator it = list_elements.begin(); it != list_elements.end(); it++)
#     {
#     	delete it->second;
#     }


#     gsl_matrix_free(global);
#     list_elements.clear();
#     gsl_permutation_free(permutation);


#     return 0;
# }

if __name__ == "__main__":
    main()

