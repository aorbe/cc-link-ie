/*
 * ProblemaDireto.cpp
 *
 *  Created on: 01/12/2017
 *      Author: asantos
 */

#include "ProblemaDireto.h"
#include "Matriz.h"

ProblemaDireto::ProblemaDireto(int mode) {
	mesh = new MeshFile(mode);
}

ProblemaDireto::~ProblemaDireto() {
	gsl_permutation_free(permutation);
}

void ProblemaDireto::calcDecomposicao() {
	int signum = 0;
	printf("\nCalculando Decomposição ...");
	permutation = gsl_permutation_calloc (nElementos);
	gsl_linalg_LU_decomp(Matriz::get()->Global(), permutation, &signum);
  //gsl_linalg_LU_invert(global, permutation, inversa);
	printf("Pronto\n\n");
}

int ProblemaDireto::inicializa(char* filename, unsigned long nEletrodos, unsigned int padrao) {

	this->padrao = padrao;
	this->nEletrodos = nEletrodos;

	mesh->ReadFile(filename);
	nElementos = mesh->getElementSize();

	Matriz::get()->inicializa(nElementos);
	printf("Matriz global de %lu x %lu\n", nElementos, nElementos);

	printf("\nCalculando Matriz Global ....");
	gsl_matrix_set_zero(Matriz::get()->Global());
	condutividade = gsl_vector_alloc(nElementos);
	gsl_vector_set_all(condutividade, 1);
	mesh->calcGlobal(Matriz::get()->Global(), condutividade);
	printf("Done\n");

	printf("\nAlocando Memoria para Correntes e Tensões ....");
	corrente = gsl_matrix_alloc(nElementos, nEletrodos);
	gsl_matrix_set_zero(corrente);
	tensao = gsl_matrix_alloc(nElementos, nEletrodos);
	gsl_matrix_set_zero(tensao);
	printf("Done\n");

	printf("Calculando Matriz de Correntes ....");
	int inpEletrodo = 0;
	int outEletrodo;
	for(unsigned int ele=0; ele <nEletrodos; ele++) {
		outEletrodo = (inpEletrodo + padrao + 1) % nEletrodos;
		gsl_matrix_set(corrente, inpEletrodo, ele, 1);
		gsl_matrix_set(corrente, outEletrodo, ele, -1);
	}
	printf("Done\n");
	return 0;
}

void ProblemaDireto::showGlobal(unsigned long row_ini, unsigned long row_end,
		unsigned long col_ini, unsigned long col_end) {
	for(unsigned long row=row_ini; row<row_end; row++)
	{
		for(unsigned long col=col_ini; col < col_end; col++)
		{
			printf("%f ", gsl_matrix_get(Matriz::get()->Global(), row, col));
		}
		printf("\n");
	}
	FILE* fGlobal;
	fGlobal= fopen("/home/asantos/Downloads/global.txt", "w+");
	gsl_matrix_fprintf(fGlobal,Matriz::get()->Global(), " %f");
	fclose(fGlobal);

}

void ProblemaDireto::showInversa(unsigned long row_ini, unsigned long row_end,
		unsigned long col_ini, unsigned long col_end) {
	for(unsigned long row=row_ini; row<row_end; row++)
	{
		for(unsigned long col=col_ini; col < col_end; col++)
		{
			printf("%f ", gsl_matrix_get(inversa, row, col));
		}
		printf("\n");
	}
}

void ProblemaDireto::solve() {
	for(unsigned int ele=0; ele <nEletrodos; ele++) {
		printf("Calculando Padrao %u\n", ele);
		gsl_vector_view vTensao = gsl_matrix_column(tensao, ele);
		gsl_vector_view vCorrente = gsl_matrix_column(corrente, ele);
		FILE* fTemp;
		fTemp= fopen("/home/asantos/Downloads/global1.txt", "w+");
		gsl_matrix_fprintf(fTemp, Matriz::get()->Global()," %f");
		fclose(fTemp);
		fTemp= fopen("/home/asantos/Downloads/corrente1.txt", "w+");
		gsl_vector_fprintf(fTemp, &vCorrente.vector, " %f");
		fclose(fTemp);
		gsl_linalg_LU_solve(Matriz::get()->Global(), permutation, &vCorrente.vector, &vTensao.vector);
		printf("Done\n");
	}
	printf("Imprimindo Resultados em /home/asantos/Downloads/final.txt ....");
	//gsl_matrix_fprintf(stdout, tensao, "%f");
	FILE *resultado;
	resultado = fopen("/home/asantos/Downloads/final.txt", "w+");
	gsl_matrix_fprintf(resultado, tensao, "%f");
	fclose(resultado);
	printf("Done\n");

}

unsigned long ProblemaDireto::getNumElementos() {
	return nElementos;
}
