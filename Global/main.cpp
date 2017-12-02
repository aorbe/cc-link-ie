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

using namespace std;
#include "TensaoMedida.h"
#include "ProblemaDireto.h"
#include "Reconstrucao.h"

/******************* FUNCTIONS ***********************/
int showMenu()
{
	int option = -1;
	printf("\n\nMENU PRINCIPAL\n");
	printf("1 - Ler arquivo msh e Calcular matriz global\n");
	printf("2 - Ler arquivo de medidas\n");
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
	int argp = 1;
	int option = -1;
	unsigned long col_ini, col_end, row_ini, row_end;
	int signum = 0, mode = 2;
	unsigned long num, size = 0;
	char filename[256] = {};
	TensaoMedida* tensaoMedida;
	ProblemaDireto *direto;
	Reconstrucao* reconstrucao;

	if(argc > argp) {
		if (!strcmp("3D", argv[argp]))
		  mode = 3;
		if (!strcmp("2D", argv[argp]))
		  mode = 2;
		argp++;
	}

	if(argc > argp) {
		strcpy(filename, argv[argp]);
		argp++;
	}

	while(option)
	{
		option = showMenu();
		switch(option)
		{
		case 0:
			break;

		case 1:
			if (strlen(filename) > 0) {
				printf("Nome do arquivo: %s\n", filename);
			} else {
				printf("Nome do arquivo: ");
				scanf(" %s", filename);
			}
			direto = new ProblemaDireto(mode);
			direto->inicializa(filename, 32, 5);
			break;


		case 2: {
			tensaoMedida = new TensaoMedida("/home/asantos/Downloads/aa_demodulacao/ampl.txt");
			break;
		}

		case 3:
			//printf("Digite o número do Elemento:");
			//scanf("%lu", &num);
			//printf("Buscando Elemento %lu\n", num);
			//mesh.showLocal(num);
			break;

		case 4:
			printf("Linha Inicial:");    	scanf("%lu", &row_ini);
			printf("Linha Final:");    		scanf("%lu", &row_end);
			printf("Coluna Inicial:");   	scanf("%lu", &col_ini);
			printf("Coluna Final:");    	scanf("%lu", &col_end);
			printf("\nMatriz Global\n");
			direto->showGlobal(row_ini, row_end, col_ini, col_end);
			break;

		case 5:
			direto->calcDecomposicao();
			printf("\n\n");
			break;

		case 6:
			printf("Linha Inicial:");    	scanf("%lu", &row_ini);
			printf("Linha Final:");    		scanf("%lu", &row_end);
			printf("Coluna Inicial:");   	scanf("%lu", &col_ini);
			printf("Coluna Final:");    	scanf("%lu", &col_end);
			printf("\nMatriz Inversa\n");
			direto->showInversa(row_ini, row_end, col_ini, col_end);
			break;

		case 7:
			direto->solve();
			reconstrucao = new Reconstrucao(direto);
			reconstrucao->run();
			printf("\n\n");
			break;
		}
	}
	return 0;
}



