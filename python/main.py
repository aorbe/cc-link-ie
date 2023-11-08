# Processamento de Malha

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_vector.h>
#include <map>
#include "triangle.h"

# pip install gmshparser
import pymesh
import meshio
import numpy as np

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

	list_nodes = {}
	for node in mesh.points:
		print(node)
		# print("{} {}".format(node.get_tag(), node.get_number_of_nodes()))
		# if (node.get_tag()== 2):
			# for n in node.get_nodes():
			# 	list_nodes.insert(n.. n.get_coordinates())

	min_node = 10000
	max_node = 0
	dict_node = {}
	elems = None
	for element in mesh.cells:
		if (element.type == 'triangle'):
			elems = element
			for e in element.data:
				for num in e:
					if (dict_node.get(num) == None):
						dict_node[num] = num
					if (num > max_node):
						max_node = num
					if (num < min_node):
						min_node = num
	print("Max {} Min {}".format(max_node, min_node))
	num_nodes = len(dict_node)
				
	print("Num Elementos: {}".format(num_nodes))
	mGlobal = np.ndarray([num_nodes, num_nodes])


	for e in elems.data:
		local_nodes = [mesh.points[e[2]], mesh.points[e[1]], mesh.points[e[0]]]
		area = 0
		area =  local_nodes[0][0] * local_nodes[1][1] - local_nodes[1][0] * local_nodes[0][1]
		area += local_nodes[2][0] * local_nodes[0][1] - local_nodes[0][0] * local_nodes[2][1]
		area += local_nodes[1][0] * local_nodes[2][1] - local_nodes[2][0] * local_nodes[1][1]
		if (area <= 0):
			print("Erro Elemento {} (A ({})<= 0)".format(1, area))
			#print(" Nos {%lu} {%lu} {%lu}\n".format(e));
		



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

