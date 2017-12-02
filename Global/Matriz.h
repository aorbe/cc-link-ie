/*
 * Matriz.h
 *
 *  Created on: 02/12/2017
 *      Author: asantos
 */

#ifndef MATRIZ_H_
#define MATRIZ_H_
#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_vector.h>

class Matriz {
private:
	Matriz();
	gsl_matrix* global;
	gsl_matrix* jacobian;
	unsigned long nElementos;
	static Matriz instance;
public:
	static Matriz* get();
	void inicializa(unsigned long nElementos);
	virtual ~Matriz();
	gsl_matrix* Global();
	gsl_matrix* Jacobian();

};

#endif /* MATRIZ_H_ */
