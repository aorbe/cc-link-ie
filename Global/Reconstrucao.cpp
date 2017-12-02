/*
 * Reconstrucao.cpp
 *
 *  Created on: 01/12/2017
 *      Author: asantos
 */

#include "Reconstrucao.h"
#include "Matriz.h"

Reconstrucao::Reconstrucao(ProblemaDireto *direto) {
	this->direto = direto;
	M1 = gsl_matrix_alloc(direto->getNumElementos(), direto->getNumElementos());
	gsl_matrix_set_zero(M1);
}

Reconstrucao::~Reconstrucao() {
	// TODO Auto-generated destructor stub
}

#define JACOBIAN	(Matriz::get()->Jacobian())

void Reconstrucao::run() {
	// solucao = solucao + alfa (J^T J +
	double lambda = 1;
	gsl_blas_dgemm(CblasTrans, CblasNoTrans, 1, JACOBIAN, JACOBIAN, lambda*lambda, M1);


}

void Reconstrucao::calcJacobian() {

}
