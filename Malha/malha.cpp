#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	FILE *fgeo;
	char fgeoName[] = "malha.geo";
	unsigned int qtdeEletrodos = 2; 
	unsigned int distEletrodos = 3;
	
	int args = argc - 1;
	if (args) {
		if (1 != sscanf(argv[1], "%d", &qtdeEletrodos)) {
			printf("Quantidade de Eletrodos Inválida.\n");
			return -1;
		}
		args--;
	}
	if (args) {
		if (1 != sscanf(argv[1], "%d", &distEletrodos)) {
			printf("Distância entre Eletrodos Inválida.\n");
			return -1;
		}
		args--;
	}

	printf("Parâmetros\n");
	printf("Quantidade de Eletrodos = %d\n", qtdeEletrodos);
	printf("Distância entre Eletrodos = %d\n", distEletrodos);
	
	if( access( fgeoName, F_OK ) != -1 ) {
		char* BkpName;
		BkpName = (char *)malloc(sizeof(fgeoName) + 4);
		unsigned int count = 1;
		do {
			sprintf(BkpName, "malha.geo.%03d", count);
			count++;
			if (count > 999) {
				printf("Não foi possível criar backup do arquivo %s.\n", fgeoName);
				printf("Apague os arquivos de backup e tente novamente\n");	
				free(BkpName);
				return 0;
			}
		} while(access(BkpName, F_OK ) != -1);
		rename(fgeoName, BkpName);
    		printf("Arquivo %s existente renomeado para %s.\n", fgeoName, BkpName);
		free(BkpName);
		// file exists
	}

	fgeo = fopen(fgeoName, "w+");

	
	unsigned int iPonto = 1;
	unsigned int iLinha = 1;
	unsigned int iObj = 1;
	char eName[] = "ELETRODO000";
	
	for(unsigned int ele=1; ele <= qtdeEletrodos; ele++) {
		fprintf(fgeo, "Point(%d) = {%.2f, -1.0, 0.0};\n", iPonto++,  distEletrodos*(ele-1) + 0.5);
		fprintf(fgeo, "Point(%d) = {%.2f,  1.0, 0.0};\n", iPonto++,  distEletrodos*(ele-1) + 0.5);
		fprintf(fgeo, "Point(%d) = {%.2f, -1.5, 0.0};\n", iPonto++,  distEletrodos*(ele-1) + 1.0);
		fprintf(fgeo, "Point(%d) = {%.2f, -1.0, 0.0};\n", iPonto++,  distEletrodos*(ele-1) + 1.0);
		fprintf(fgeo, "Point(%d) = {%.2f,  0.0, 0.0};\n", iPonto++,  distEletrodos*(ele-1) + 1.0);
		fprintf(fgeo, "Point(%d) = {%.2f,  1.0, 0.0};\n", iPonto++,  distEletrodos*(ele-1) + 1.0);
		fprintf(fgeo, "Point(%d) = {%.2f,  1.5, 0.0};\n", iPonto++,  distEletrodos*(ele-1) + 1.0);
		fprintf(fgeo, "Point(%d) = {%.2f, -1.0, 0.0};\n", iPonto++,  distEletrodos*(ele-1) + 1.5);
		fprintf(fgeo, "Point(%d) = {%.2f,  1.0, 0.0};\n", iPonto++,  distEletrodos*(ele-1) + 1.5);
		
		fprintf(fgeo, "Line(%d) = {%d, %d};\n", iLinha++, iPonto-8, iPonto-9);
		fprintf(fgeo, "Line(%d) = {%d, %d};\n", iLinha++, iPonto-7, iPonto-3);
		fprintf(fgeo, "Line(%d) = {%d, %d};\n", iLinha++, iPonto-2, iPonto-1);

		fprintf(fgeo, "Circle(%d) = {%d, %d, %d};\n", iLinha++, iPonto-9, iPonto-6, iPonto-2);	
		fprintf(fgeo, "Circle(%d) = {%d, %d, %d};\n", iLinha++, iPonto-1, iPonto-4, iPonto-8);
	
		fprintf(fgeo, "Line Loop(%d) = {%d, %d, %d, %d};\n", iObj, iLinha-5, iLinha-2, iLinha-3, iLinha-1);
		fprintf(fgeo, "Plane Surface(%d) = {%d};\n", iObj, iObj);
		sprintf(eName, "ELETRODO%03d", ele);
		fprintf(fgeo, "Physical Surface(\"%s\") = {%d};\n", eName, iObj++);
	}

fprintf(fgeo, "Point(%d) = { -1 , -2, 0};\n", iPonto++);
fprintf(fgeo, "Point(%d) = {%d, -2, 0};\n", iPonto++, distEletrodos*qtdeEletrodos);
fprintf(fgeo, "Point(%d) = {%d,  2, 0};\n", iPonto++, distEletrodos*qtdeEletrodos);
fprintf(fgeo, "Point(%d) = { -1 , 2, 0};\n", iPonto++);

fprintf(fgeo, "Line(%d) = {%d, %d};\n", iLinha++, iPonto-4, iPonto-1);
fprintf(fgeo, "Line(%d) = {%d, %d};\n", iLinha++, iPonto-3, iPonto-4);
fprintf(fgeo, "Line(%d) = {%d, %d};\n", iLinha++, iPonto-2, iPonto-3);
fprintf(fgeo, "Line(%d) = {%d, %d};\n", iLinha++, iPonto-1, iPonto-2);

fprintf(fgeo, "Line Loop(%d) = {%d, %d, %d, %d};\n", iObj, iLinha-1, iLinha-2, iLinha-3, iLinha-4);
fprintf(fgeo, "Plane Surface(%d) = {1", iObj);
for(unsigned int objs=2; objs<=iObj; objs++) {
	fprintf(fgeo, ", %d", objs);
}
fprintf(fgeo, "};\n"); 
sprintf(eName, "CINTA");
fprintf(fgeo, "Physical Surface(\"%s\") = {%d};\n", eName, iObj++);

fprintf(fgeo, "Point(%d) = {5, 5, 0};\n", iPonto++);
fprintf(fgeo, "Point(%d) = {3, 5, 0};\n", iPonto++);
fprintf(fgeo, "Point(%d) = {3, 3, 0};\n", iPonto++);
fprintf(fgeo, "Point(%d) = {5, 3, 0};\n", iPonto++);

fprintf(fgeo, "Spline(%d) = {%d, %d, %d, %d, %d};\n", iLinha++, iPonto-4, iPonto-3, iPonto-2, iPonto-1, iPonto-4);

fprintf(fgeo, "Extrude {Surface{%d}} Using Wire{Spline{%d}};\n", iObj-1, iLinha-1);



fclose(fgeo);



//+

	
}
