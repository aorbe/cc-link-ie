/*
 * GenericElement.h
 *
 *  Created on: 17/08/2016
 *      Author: asantos
 */

#ifndef GENERIC_ELEMENT_H_
#define GENERIC_ELEMENT_H_

#include <stdio.h>
#include <string.h>
#include <map>
#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>

#include "Node.h"

class GenericElement
{
protected:
  double *local;
  unsigned long number;
  unsigned int type;
  struct Tags {
    unsigned int number;
    int *values;
  } tags;
  unsigned int nNodes;
  unsigned long *nodes;

	double condutividade;
  double determinante(const double va[3], const double vb[3], const double vc[3]);

public:
  virtual int calcLocalMatrix(long double condutividade, std::map<unsigned long,Node> *list_nodes) = 0;
  
  GenericElement(unsigned long number, unsigned int type, unsigned int nTags);
  GenericElement(GenericElement &element);
  ~GenericElement();
  void updateGlobal(gsl_matrix *matrix, std::map<unsigned long,unsigned long> *hash_nodes);
  
  int readNodes(char* buf);
  unsigned long getNumber();
  unsigned int getNumberTags();
  unsigned int getType();
  unsigned int getNumberNodes();
  unsigned long getNode(unsigned int idx);
  void setTag(unsigned int pos, int value);
  int getTag(unsigned int pos);
  void showLocal();


};

#endif /* GENERIC_ELEMENT_H_ */
