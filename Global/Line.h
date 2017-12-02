/*
 * Tetraedro.h
 *
 *  Created on: 17/08/2016
 *      Author: asantos
 */

#ifndef LINE_H_
#define LINE_H_

#include "GenericElement.h"

class Line: public GenericElement
{
public:
	Line(unsigned long number, unsigned int type, unsigned int nTags) : GenericElement(number, type, nTags) {
    nNodes = 2;
    nodes = new unsigned long[nNodes];
  }
	~Line() {}
	int calcLocalMatrix(std::map<unsigned long,Node> *nodes) {(void) nodes; return 0;}
	void showLocal() {};
	double getFatorCondut() {return 1;}
	double getDerivCondut() {return 1;}

};

#endif /* LINE_H_ */
