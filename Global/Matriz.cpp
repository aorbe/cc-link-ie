/*
 * Matriz.cpp
 *
 *  Created on: 02/12/2017
 *      Author: asantos
 */

#include "Matriz.h"

Matriz Matriz::instance;

Matriz::Matriz() {
	global = NULL;
	jacobian = NULL;
}

Matriz::~Matriz() {
	// TODO Auto-generated destructor stub
}

Matriz* Matriz::get() {
	return &instance;
}

void Matriz::inicializa(unsigned long nElementos) {
	this->nElementos = nElementos;
	global = gsl_matrix_alloc(nElementos, nElementos);

	jacobian = gsl_matrix_alloc(nElementos, nElementos);
}
gsl_matrix* Matriz::Global() {
	return global;
}
gsl_matrix* Matriz::Jacobian() {
	return jacobian;
}

