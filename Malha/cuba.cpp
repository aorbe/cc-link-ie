#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define INITIAL_FILENAME	"cuba.ini"
#define TEMPORA_FILENAME	"cuba.tmp"
#define MSH_FILENAME 		"cubaXD.msh"
#define GEO_FILENAME		"cuba.geo"

int testFile(const char *fileName)
{
	if (access(fileName, F_OK) != -1)
	{
		char *BkpName;
		BkpName = (char *)malloc(sizeof(fileName) + 4);
		unsigned int count = 1;
		do
		{
			sprintf(BkpName, "%s.%03d", fileName, count);
			count++;
			if (count > 999)
			{
				printf("Não foi possível criar backup do arquivo %s.\n", fileName);
				printf("Apague os arquivos de backup e tente novamente\n");
				free(BkpName);
				return -1;
			}
		} while (access(BkpName, F_OK) != -1);
		rename(fileName, BkpName);
		printf("Arquivo %s existente renomeado para %s.\n", fileName, BkpName);
		free(BkpName);
	}
	return 0;
}

int main(int argc, char *argv[])
{
	FILE *fgeo;
	unsigned int qtde = 32;
	double diametro = 303;
	double largura = 10;
	double altura = 50;
	double ele_size = 5.0;
	int mode = 2;
	int eleSimples = 1;

	int args = argc - 1;
	if (args)
	{
		if (0 == strcmp(argv[argc - args], "-h"))
		{
			printf("Usage:\n");
			printf("cuba <N_ELE> <D_CUBA> <W_ELE> <H_CUBA> <ELE_K>\n");
			printf("N_ELE \t Numero de Elementos\n");
			printf("D_CUBA \t Diametro da Cuba\n");
			printf("W_ELE \t Largura do Eletrodo\n");
			printf("H_CUBA \t Altura da Cuba\n");
			printf("ELE_K \t Tamanho do Elemento (Utilizado pelo Gmsh)\n");
			return 0;
		}
		if (1 != sscanf(argv[argc - args], "%d", &qtde))
		{
			printf("Quantidade de Eletrodos Inválida.\n");
			return -1;
		}
		if ((qtde < 2) || (qtde > 999))
		{
			printf("A quantidade de eletrodos deve estar entre 2 e 999.\n");
			return -1;
		}
		args--;
	}
	if (args)
	{
		if (1 != sscanf(argv[argc - args], "%lf", &diametro))
		{
			printf("Diâmetro da Cuba Inválido Inválido.\n");
			return -1;
		}
		args--;
	}
	if (args)
	{
		if (1 != sscanf(argv[argc - args], "%lf", &largura))
		{
			printf("Largura do Eletrodo Inválida.\n");
			return -1;
		}
		args--;
	}
	if (args)
	{
		if (1 != sscanf(argv[argc - args], "%lf", &altura))
		{
			printf("Altura da Cuba Inválida.\n");
			return -1;
		}
		args--;
	}
	if (args)
	{
		if (1 != sscanf(argv[argc - args], "%lf", &ele_size))
		{
			printf("Tamanho Elemento Inválido.\n");
			return -1;
		}
		args--;
	}

	printf("Parâmetros\n");
	printf("Quantidade de Eletrodos = %d\n", qtde);
	printf("Diâmetro da Cuba = %.3lf\n", diametro);
	printf("Altura da Cuba = %.3lf\n", altura);
	printf("Largura do Eletrodo = %.3lf\n", largura);
	printf("Tamanho do Elemento = %.3lf\n", ele_size);

	int result = testFile(GEO_FILENAME);
	if (result)
		return result;

	fgeo = fopen(GEO_FILENAME, "w+");

	unsigned int iPonto = qtde + 1;
	unsigned int iLinha = 1;
	unsigned int iEletrodo = 1;
	unsigned int iObj = 1;
	unsigned int iPlano = 1;
	char eName[] = "ELETRODO000";
	double raio = diametro / 2;
	double eleAng = largura / raio;
	double eleDist = 0.5;

	fprintf(fgeo, "Point(%d) = {0.0, 0.0, 0.0, %f};\n", iPonto++, ele_size);

	if (!eleSimples)
	{
		for (double ang = 0; ang < 2 * M_PI; ang = ang + (2 * M_PI) / (double)qtde)
		{
			// Pontos do Eletrodo

			if (mode == 3)
			{
				fprintf(fgeo, "Point(%d) = {%f, %f, %f, %f};\n", iPonto++, raio * sin(ang), raio * cos(ang), altura, ele_size);
				fprintf(fgeo, "Point(%d) = {%f, %f, %f, %f};\n", iPonto++, raio * sin(ang + eleAng), raio * cos(ang + eleAng), altura, ele_size);
			}
			fprintf(fgeo, "Point(%d) = {%f, %f, 0.0, %f};\n", iPonto++, raio * sin(ang), raio * cos(ang), ele_size);
			fprintf(fgeo, "Point(%d) = {%f, %f, 0.0, %f};\n", iPonto++, raio * sin(ang + eleAng), raio * cos(ang + eleAng), ele_size);

			if (mode == 3)
			{
				fprintf(fgeo, "Line(%d) = {%d, %d};\n", iLinha++, iPonto - 3, iPonto - 4);
				fprintf(fgeo, "Line(%d) = {%d, %d};\n", iLinha++, iPonto - 1, iPonto - 3);
				fprintf(fgeo, "Line(%d) = {%d, %d};\n", iLinha++, iPonto - 4, iPonto - 2);
			}
			fprintf(fgeo, "Line(%d) = {%d, %d};\n", iLinha++, iPonto - 1, iPonto - 2);

			// Eletrodo
			if (mode == 3)
			{
				fprintf(fgeo, "Point(%d) = {%f, %f, %f, %f};\n", iEletrodo++, (raio + eleDist) * sin(ang + eleAng / 2), (raio + eleDist) * cos(ang + eleAng / 2), altura / 2, ele_size);
				fprintf(fgeo, "Line(%d) = {%d, %d};\n", iLinha++, iPonto - 2, iEletrodo - 1, ele_size);
				fprintf(fgeo, "Line(%d) = {%d, %d};\n", iLinha++, iPonto - 1, iEletrodo - 1, ele_size);
				fprintf(fgeo, "Line(%d) = {%d, %d};\n", iLinha++, iPonto - 4, iEletrodo - 1, ele_size);
				fprintf(fgeo, "Line(%d) = {%d, %d};\n", iLinha++, iPonto - 3, iEletrodo - 1, ele_size);
			}
			else
			{
				fprintf(fgeo, "Point(%d) = {%f, %f, 0.0, %f};\n", iEletrodo++, (raio + eleDist) * sin(ang + eleAng / 2), (raio + eleDist) * cos(ang + eleAng / 2), ele_size);
				fprintf(fgeo, "Line(%d) = {%d, %d};\n", iLinha++, iEletrodo - 1, iPonto - 1, ele_size);
				fprintf(fgeo, "Line(%d) = {%d, %d};\n", iLinha++, iEletrodo - 1, iPonto - 2, ele_size);
				fprintf(fgeo, "Line Loop(%d) = {-%d, -%d, %d};\n", iObj++, iLinha - 3, iLinha - 2, iLinha - 1);
				fprintf(fgeo, "Plane Surface(%d) = {%d};\n", iObj - 1, iObj - 1);
			}
		}
	}
	else
	{
		double ang = 0;
		fprintf(fgeo, "Point(1) = {%f, %f, 0.0, %f};\n", raio * sin(ang), raio * cos(ang), ele_size);
		for (iEletrodo = 2; iEletrodo <= qtde; iEletrodo++)
		{
			ang += (2 * M_PI) / qtde;
			fprintf(fgeo, "Point(%d) = {%f, %f, 0.0, %f};\n", iEletrodo, raio * sin(ang), raio * cos(ang), ele_size);
			fprintf(fgeo, "Line(%d) = {%d, %d};\n", iLinha++, iEletrodo - 1, iEletrodo);
		}
		fprintf(fgeo, "Line(%d) = {%d, 1};\n", iLinha++, iEletrodo - 1);
	}

	// Arcos entre eletrodos
	unsigned int idxContorno = iLinha;
	unsigned int iCircleIncr = 2;
	if (!eleSimples)
	{
		if (mode == 3)
			iCircleIncr = 4;
		for (unsigned int pos = 3; pos < (qtde * iCircleIncr) + 1; pos += iCircleIncr)
		{
			fprintf(fgeo, "Circle(%d) = {%d, %d, %d};\n", iLinha++, pos + qtde + 1, qtde + 1, pos + qtde);
		}
		fprintf(fgeo, "Circle(%d) = {%d, %d, %d};\n", iLinha++, qtde + 2, qtde + 1, qtde * iCircleIncr + qtde + 1);

		// Contorno da Cuba
		fprintf(fgeo, "Line Loop (%d) = {%d, %d", iObj, 1, idxContorno);
		int iPassoLinha = 3;
		if (mode == 3)
		{
			iPassoLinha += 2;
		}
		for (unsigned int pos = idxContorno + 1; pos < idxContorno + qtde; pos++)
		{
			fprintf(fgeo, ", %d, %d", (pos - idxContorno) * iPassoLinha + 1, pos);
		}
		fprintf(fgeo, "};\n");
	}
	else
	{
		fprintf(fgeo, "Line Loop (%d) = {1 ", iObj);
		for (unsigned int pos = 2; pos <= qtde; pos++)
		{
			fprintf(fgeo, ", %d", pos);
		}
		fprintf(fgeo, "};\n");
	}

	// Plano
	fprintf(fgeo, "Plane Surface(%d) = {%d};\n", iObj, iObj);
	fprintf(fgeo, "Point{%d} In Surface{%d};\n", qtde + 1, iObj);
	// fprintf(fgeo, "Transfinite Surface{%d}={1", iObj);
	// for (unsigned int pos = 2; pos <= 32; pos++)
	// {
	// 	fprintf(fgeo, ", %d", pos);
	// }
	// fprintf(fgeo, "};\n");

	if (mode == 3)
		fprintf(fgeo, "Extrude {0, 0, %f} {Surface{%d};}\n", altura, iObj);

	iObj++;

	fclose(fgeo);

	char outFileName[] = MSH_FILENAME;
	if (mode == 3)
		outFileName[4] = '3';
	else
		outFileName[4] = '2';

	if (testFile(outFileName))
		return -1;

	char *strMesh = (char *)malloc(strlen(GEO_FILENAME) + strlen(INITIAL_FILENAME) + 32);
	if (mode == 3)
		sprintf(strMesh, "gmsh %s -3 -o %s -format msh2", GEO_FILENAME, INITIAL_FILENAME);
	else
		sprintf(strMesh, "gmsh %s -2 -o %s -format msh2", GEO_FILENAME, INITIAL_FILENAME);
	
	if (fork()== 0) {
        system(strMesh);
		return 0;
	}
	printf("Aguardando mesh.\n");
	wait(NULL); 
	printf("Fase 1 terminada\nCorrigindo arquivo gerado\n"); 

	FILE *mshIn, *mshOut, *mshTmp;

	mshIn = fopen(INITIAL_FILENAME, "r+");
	mshOut = fopen(outFileName, "w+");
	mshTmp = fopen(TEMPORA_FILENAME, "w+");

	char line[256], tmpLine[256];
	char elementsString[655360] = "";
	unsigned long value, type;
	int pos;
	int actualKey = 0;
	unsigned long elements_size = 0, elements_count = 0;
	printf("Analisando Mesh\n");
	while (!feof(mshIn))
	{
		if (NULL == fgets(line, sizeof(line), mshIn))
			continue;

		if (strncmp(line, "$Elements", 9) == 0)
		{
			actualKey = 1;
			fputs(line, mshOut);
			continue;
		}
		if (actualKey==1 && strncmp(line, "$EndElements", 12) == 0)
		{
			actualKey = 2;
			fseek(mshTmp, 0, SEEK_SET);
			fprintf(mshOut, "%ld\n", elements_count);
			printf("Ao final foram detectados %ld elementos\n", elements_count);
			while (!feof(mshTmp)) {
				fgets(tmpLine, sizeof(tmpLine), mshTmp);
				fputs(tmpLine, mshOut);
			}				
			fputs(line, mshOut);
			continue;
		}
		if (actualKey == 1)
		{
			unsigned int ret = sscanf((const char *)line, (const char *)"%ld %n %d", &value, &pos, &type);
			if (ret == 1 && 0 == elements_size)
			{
				elements_size = value;
				printf("Inicialmente foram detectados %ld elementos\n", elements_size);
			}
			else if (ret == 2 && 0 != elements_size)
			{
				if (type == 2)
				{
					elements_count++;
					fprintf(mshTmp, "%ld %s", elements_count, &line[pos]);
				}
			}
			continue;
		}
		fputs(line, mshOut);
	}
	fclose(mshIn);
	fclose(mshOut);
	fclose(mshTmp);
	
	return 0;
}
