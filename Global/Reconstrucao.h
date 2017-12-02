/*
 * Reconstrucao.h
 *
 *  Created on: 01/12/2017
 *      Author: asantos
 */

#ifndef RECONSTRUCAO_H_
#define RECONSTRUCAO_H_

#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_vector.h>

#include "ProblemaDireto.h"

class Reconstrucao {
	gsl_vector* solucao;
	ProblemaDireto* direto;
	gsl_matrix *M1;
public:
	Reconstrucao(ProblemaDireto *direto);
	virtual ~Reconstrucao();
	void run();
	void calcJacobian();
};

#endif /* RECONSTRUCAO_H_ */
