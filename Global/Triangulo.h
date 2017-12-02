/*
 * Triangulo.h
 *
 *  Created on: 17/08/2016
 *      Author: asantos
 */

#ifndef TRIANGULO_H_
#define TRIANGULO_H_

#include "GenericElement.h"

class Triangulo: public GenericElement
{
  double area;
	void calcArea(std::map<unsigned long,Node> *nodes);

public:
	Triangulo(unsigned long number, unsigned int type, unsigned int nTags);
	~Triangulo();
	int calcLocalMatrix(std::map<unsigned long,Node> *nodes);
	double getFatorCondut();
	double getDerivCondut();
};





#endif /* TRIANGULO_H_ */
