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
	int calcLocalMatrix(long double condutividade, std::map<unsigned long,Node> *nodes);
};





#endif /* TRIANGULO_H_ */
