/*
 * TensaoMedida.h
 *
 *  Created on: 17/08/2016
 *      Author: asantos
 */
#ifndef TENSAOMEDIDA_H_
#define TENSAOMEDIDA_H_

#include <iostream>
#include <list>
#include <map>

#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_vector.h>

#define FRAME_SIZE	1024

class TensaoMedida
{
	int qtde;
	std::map<int, double*> medidas;
public:
	TensaoMedida(const char* filename);


};
#endif /* TENSAOMEDIDA_H_ */
