/*
 * main.cpp
 *
 *  Created on: 11/08/2016
 *      Author: asantos
 */

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
#include "Node.h"
#include "MeshFile.h"

using namespace std;

gsl_matrix* global;
gsl_matrix* inversa;
gsl_permutation* permutation;
gsl_vector *vI, *vP;


/******************* FUNCTIONS ***********************/
int showMenu()
{
	int option = -1;
	printf("MENU PRINCIPAL\n");
	printf("1 - Ler arquivo msh\n");
	printf("2 - Calcular matriz global\n");
	printf("3 - Mostrar matriz local\n");
	printf("4 - Mostrar matriz global\n");
	printf("5 - Calcular matriz inversa\n");
	printf("6 - Imprimir Voltagem\n");
	printf("7 - Calcular voltagem\n");
	printf("0 - Sair\n");
	printf("Selecione a opção:");
	scanf(" %d", &option);
	return option;
}


int main(int argc, char** argv)
{
  int option = -1;
	unsigned long col_ini, col_end, row_ini, row_end;
	int signum = 0, mode = 2;
  unsigned long num;
	char filename[256];

  if(argc == 2) {
    if (!strcmp("3D", argv[1]))
      mode = 3;
    if (!strcmp("2D", argv[1]))
      mode = 2;
  }

  MeshFile mesh(mode);

  while(option)
  {
  	option = showMenu();
  	switch(option)
  	{
  	case 0:
  		break;

  	case 1:
      if (argc > 1) {
        strcpy(filename, argv[1]);
        printf("Nome do arquivo: %s\n", filename);
      } else {
	      printf("Nome do arquivo: ");
	      scanf(" %s", filename);
      }
	    mesh.ReadFile(filename);
      printf("\nTotal de %d nodes\n", mesh.getNodesSize());
      global = gsl_matrix_alloc(mesh.getNodesSize(), mesh.getNodesSize());
      printf("Matriz global de %d x %d\n", mesh.getNodesSize(), mesh.getNodesSize());
	    break;

  	case 2:
  		printf("\nCalculando Matriz Global ....");
  		gsl_matrix_set_zero(global);
      mesh.calcGlobal(global, 1);
	    printf("Finished\n\n");
  		break;

  	case 3:
      printf("Digite o número do Elemento:");
      scanf("%lu", &num);
      printf("Buscando Elemento %lu\n", num);
      mesh.showLocal(num);
  		break;

  	case 4:
  		printf("Linha Inicial:");    	scanf("%lu", &row_ini);
  		printf("Linha Final:");    		scanf("%lu", &row_end);
  		printf("Coluna Inicial:");   	scanf("%lu", &col_ini);
  		printf("Coluna Final:");    	scanf("%lu", &col_end);
  		printf("\nMatriz Global\n");
  		for(unsigned long row=row_ini; row<row_end; row++)
  		{
  			for(unsigned long col=col_ini; col < col_end; col++)
  			{
  				printf("%f ", gsl_matrix_get(global, row, col));
  			}
  			printf("\n");
  		}
  		FILE* fGlobal;
  		fGlobal= fopen("/home/asantos/Downloads/global.txt", "w+");
  		gsl_matrix_fprintf(fGlobal,global, " %f");
  		fclose(fGlobal);

  		break;

  	case 5:
  		printf("\nCalculando Decomposição ...");
  		permutation = gsl_permutation_calloc (global->size1);
  		gsl_linalg_LU_decomp(global, permutation, &signum);
      //gsl_linalg_LU_invert(global, permutation, inversa);
  		printf("Pronto\n\n");
  		break;

  	case 6:
  		printf("Linha Inicial:");    	scanf("%lu", &row_ini);
  		printf("Linha Final:");    		scanf("%lu", &row_end);
  		printf("Coluna Inicial:");   	scanf("%lu", &col_ini);
  		printf("Coluna Final:");    	scanf("%lu", &col_end);
  		printf("\nMatriz Inversa\n");
  		for(unsigned long row=row_ini; row<row_end; row++)
  		{
  			for(unsigned long col=col_ini; col < col_end; col++)
  			{
  				printf("%f ", gsl_matrix_get(inversa, row, col));
  			}
  			printf("\n");
  		}
  		break;
  	case 7:
  	{
  		unsigned long nodeInp, nodeOut;
  		double input;
  		printf("\nAlocando vetores de tamanho %lu\n", global->size1);
  		vI = gsl_vector_alloc(global->size1);
  		vP = gsl_vector_alloc(global->size1);
  		gsl_vector_set_all(vI, 0);
  		printf("\nCorrente de Entrada: ");
  		scanf("%lf", &input);
  		printf("Input Node: ");
  		scanf("%lu", &nodeInp);
  		printf("Output Node: ");
  		scanf("%lu", &nodeOut);
  		gsl_vector_set(vI, nodeInp - 1, input);
  		gsl_vector_set(vI, nodeOut - 1, -input);
  		gsl_linalg_LU_solve(global, permutation, vI, vP);
  		gsl_vector_fprintf(stdout, vP, "%f");
  		FILE *resultado;
  		resultado = fopen("/home/asantos/Downloads/final.txt", "w+");
  		gsl_vector_fprintf(resultado, vP, "%f");
  		fclose(resultado);

  	}
  		break;

  	}
  }


  gsl_matrix_free(global);
  gsl_permutation_free(permutation);


  return 0;
}



