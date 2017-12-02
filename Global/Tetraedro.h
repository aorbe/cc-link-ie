/*
 * Tetraedro.h
 *
 *  Created on: 17/08/2016
 *      Author: asantos
 */

#ifndef TETRAEDRO_H_
#define TETRAEDRO_H_

#include "GenericElement.h"

class Tetraedro: public GenericElement
{
  double volume;
	void calcVolume(std::map<unsigned long,Node> *nodes);

public:
	Tetraedro(unsigned long number, unsigned int type, unsigned int nTags);
	~Tetraedro();
	int calcLocalMatrix(std::map<unsigned long,Node> *list_nodes);
	void updateGlobal(gsl_matrix *matrix);
	unsigned long getNumber();
	double getFatorCondut();
	double getDerivCondut();
};

#endif /* TETRAEDRO_H_ */
