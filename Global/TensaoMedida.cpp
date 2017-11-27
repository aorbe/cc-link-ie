#include "TensaoMedida.h"
#include <string.h>
#include <functional>
#include <map>
#include <iostream>

TensaoMedida::TensaoMedida(const char* filename)
{
	int result;

	medidas.clear();
	qtde = 0;

	FILE* hFile = fopen(filename, "r");
	if (hFile == NULL) {
		printf("Erro na Abertura do Arquivo de Medidas\n");
		return;
	}

	while (!feof(hFile)) {
		double *values = new double[FRAME_SIZE];
		for(int x=0; x<FRAME_SIZE; x++) {
			result = fscanf(hFile,"%le", &values[x]);
			if (feof(hFile)) {
				if (x==0) {
					printf("Fim da Leitura.\n %d conjuntos de medidas\n", qtde);
					return;
				}
				else {
					printf("Falha de Leitura\n");
				}
			}
			if (result != 1) {
				printf("Erro de Leitura\n");
				return;
			}
		}
		medidas[qtde] = values;
		qtde++;
	}



}


