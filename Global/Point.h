/*
 * Point.h
 *
 *  Created on: 17/08/2016
 *      Author: asantos
 */

#ifndef POINT_H_
#define POINT_H_

#include "GenericElement.h"

class Point: public GenericElement
{
  
public:
	Point(unsigned long number, unsigned int type, unsigned int nTags) : GenericElement(number, type, nTags) {
    nNodes = 1;
    nodes = new unsigned long[nNodes];
  }
	~Point();
	int calcLocalMatrix(long double condutividade, std::map<unsigned long,Node> *nodes) {(void) condutividade; (void) nodes; return 0;}
	void updateGlobal(gsl_matrix *matrix) {(void) matrix; }
	void showLocal() {};

};





#endif /* POINT_H_ */
