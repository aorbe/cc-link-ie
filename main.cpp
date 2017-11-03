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
#include "triangle.h"


using namespace std;

gsl_matrix* global;
gsl_matrix* inversa;
gsl_permutation* permutation;
gsl_vector *vI, *vP;


map<unsigned int, ELEMENT*> list_elements;
POINT* nodes = NULL;


/******************* FUNCTIONS ***********************/
int showMenu()
{
	int option = -1;
	printf("MENU PRINCIPAL\n");
	printf("1 - Ler arquivo msh\n");
	printf("2 - Mostrar matriz local\n");
	printf("3 - Calcular matriz global\n");
	printf("4 - Mostrar matriz global\n");
	printf("5 - Calcular matriz inversa\n");
	printf("6 - Imprimir Voltagem\n");
	printf("7 - Calcular voltagem\n");
	printf("0 - Sair\n");
	printf("Selecione a opção:");
	scanf(" %d", &option);
	return option;
}

int readFile(const char* filename)
{
	unsigned long size;
    FILE* mshFile;
	int type = 0;
    char line[255];
    double initial_value = 0.25;  // condutividade 0.5ohm/m


    if ( (mshFile = fopen(filename, "r")) == NULL)
    {
        printf("File Open Error\n");
        return -1;
    }


    while(!feof(mshFile))
    {
        if (1 == fscanf(mshFile, " $%s", line))
        {
            if((line[0] == 'E') && (line[1] == 'n') && (line[2] == 'd'))
            {
            	type = 0;
                printf("Finishing %s\n", &line[3]);
            }
            else
            {
            	if (strcmp(line, "Elements") == 0)
            	{
            		type = 1;
            		int size;
            		fscanf(mshFile, "%d", &size);

            		if (!list_elements.empty())
            			list_elements.clear();
            		printf("Lendo %d elementos\n", size);
            	}
            	else
            	{
            		if (strcmp(line, "Nodes") == 0)
            		{
            			type = 2;
                		fscanf(mshFile, "%lu", &size);
                		nodes = (POINT*) malloc(sizeof(POINT) * size);
                		global = gsl_matrix_alloc(size, size);
                		printf("Lendo %lu nodes\n", size);
            		}
            		else
            		{
            			printf("Starting %s Segment\n", line);
            		}

            	}

            }
        }
        else
        {
        	switch(type)
        	{
        	case 1: // Element
        		unsigned int key, type;
        		ELEMENT *element;
        		fscanf(mshFile, "%u %u %*d %*d %*d", &key, &type);
        		if (type == 2)
        		{
        			unsigned int p1, p2, p3;
        			fscanf(mshFile, "%u %u %u", &p1, &p2, &p3);
        			element = new TRIANGLE(key, &nodes[p1-1], &nodes[p2-1], &nodes[p3-1]);
        			element->calcLocalMatrix(initial_value);
        			list_elements.insert(std::pair<unsigned int, ELEMENT*>(key, element));
        		}
        		else
        			printf("Invalid Element\n");
        		break;
        	case 2: // Node
        		unsigned int pos;
        		fscanf(mshFile, "%u", &pos);
        		pos--;
        		nodes[pos].number = pos;
        		fscanf(mshFile, "%lf %lf %lf", &(nodes[pos].x), &(nodes[pos].y), &(nodes[pos].z));
        		break;
        	default:
        		fscanf(mshFile, "%s", line);
        		printf("Lixo: %s\n", line);
        		break;
        	}

        }
    }
    return 0;
}

int main()
{
    int option = -1;
	unsigned long col_ini, col_end, row_ini, row_end;
	int signum = 0;
	char filename[256];

    while(option)
    {
    	option = showMenu();
    	switch(option)
    	{
    	case 0:
    		break;
    	case 1:
		printf("Nome do arquivo: ");
		scanf(" %s", filename);
		readFile(filename);
		//readFile("../malha.msh");
		break;
    	case 3:
    		printf("\nCalculando Matriz Global ....");
    		gsl_matrix_set_zero(global);
		for(std::map<unsigned int, ELEMENT*>::iterator it = list_elements.begin(); it != list_elements.end(); it++)
		{
		((ELEMENT*)it->second)->updateGlobal(global);
		}
		printf("Finished\n\n");
    		break;
    	case 2:
    		if (list_elements.empty())
    		{
    			printf("Não há elementos registrados\n");
    			break;
    		}
    		unsigned int nElemento;
    		printf("\nDigite o número do elemento: ");
    		scanf("%u", &nElemento);
    		if (list_elements.find(nElemento) != list_elements.end())
    		{
    			((ELEMENT *) list_elements[nElemento])->showLocal();
    		}
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

    for(std::map<unsigned int, ELEMENT*>::iterator it = list_elements.begin(); it != list_elements.end(); it++)
    {
    	delete it->second;
    }


    gsl_matrix_free(global);
    list_elements.clear();
    gsl_permutation_free(permutation);


    return 0;
}



