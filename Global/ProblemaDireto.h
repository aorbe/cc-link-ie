/*
 * ProblemaDireto.h
 *
 *  Created on: 01/12/2017
 *      Author: asantos
 */

#ifndef PROBLEMADIRETO_H_
#define PROBLEMADIRETO_H_

#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_vector.h>
#include <map>
#include "Node.h"
#include "MeshFile.h"
#include "TensaoMedida.h"

class ProblemaDireto {
	gsl_matrix* inversa;
	gsl_permutation* permutation;
	gsl_matrix *corrente, *tensao;
	gsl_vector *condutividade;
	MeshFile* mesh;
	unsigned long padrao, nEletrodos, nElementos;

public:
	void calcDecomposicao();
	ProblemaDireto(int mode);
	virtual ~ProblemaDireto();
	int inicializa(char* filename, unsigned long nEletrodos, unsigned int padrao);
	void showGlobal(unsigned long row_ini, unsigned long row_end,
			unsigned long col_ini, unsigned long col_end);
	void showInversa(unsigned long row_ini, unsigned long row_end,
			unsigned long col_ini, unsigned long col_end);
	void solve();
	unsigned long getNumElementos();

};

#endif /* PROBLEMADIRETO_H_ */
