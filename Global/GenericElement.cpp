/*
 * GenericElement.cpp
 *
 *  Created on: 17/08/2016
 *      Author: asantos
 */
#include "GenericElement.h"

/*
 * As classe derivadas devem atribuir valor a nNodes e alocar memória para nodes
 */
GenericElement::GenericElement(unsigned long _number, unsigned int _type, unsigned int nTags)
{
   tags.values = new int[nTags];
   tags.number = nTags;
   number = _number;
   type = _type;
   nodes = NULL;
}

GenericElement::GenericElement(GenericElement &element)
{
  tags.number = element.getNumberTags();
  tags.values = new int[tags.number];
  for(unsigned int x=0; x<tags.number; x++)
    tags.values[x] = element.getTag(x);
  number = element.getNumber();
  type = element.getType();
}

GenericElement::~GenericElement()
{
  if (tags.number != 0) {
    delete[] tags.values;
  }
  if (nodes != NULL)
    delete[] nodes;
}

/*
 * Os nós são lidos e armazenados em 'nodes[i]'
 */
int GenericElement::readNodes(char* buf) {
   unsigned int iPos = 0, iAux = 0;
   for(unsigned int x=0; x<nNodes; x++) {
     sscanf(&buf[iPos], "%ld%n", &nodes[x], &iAux);
     iPos += (iAux + 1);
   }
   return iPos;
}

unsigned long GenericElement::getNumber()
{
   return number;
}

unsigned int GenericElement::getNumberTags()
{
  return tags.number;
}

unsigned int GenericElement::getType()
{
  return type;
}

double GenericElement::determinante(const double va[3], const double vb[3], const double vc[3])
{
  double volume;
  volume  = va[0]*vb[1]*vc[2] + va[1]*vb[2]*vc[0] + va[2]*vb[0]*vc[1];
  volume -= va[2]*vb[1]*vc[0] + va[1]*vb[0]*vc[2] + va[0]*vb[2]*vc[1];
  return volume;
}

void GenericElement::setTag(unsigned int pos, int value)
{
  if (pos<tags.number)
    tags.values[pos] = value;
}

int GenericElement::getTag(unsigned int pos)
{
  if (pos<tags.number)
    return tags.values[pos];
  return -1;
}

unsigned int GenericElement::getNumberNodes()
{
  return nNodes;
}

unsigned long GenericElement::getNode(unsigned int idx)
{
  if (idx < nNodes)
    return nodes[idx];
  return 0;
}

void GenericElement::showLocal()
{
	printf("Matriz Local do Elemento %lu\n", number);
	printf("Nodes:");
  for(unsigned int x=0; x<nNodes; x++) {
    printf(" %lu", nodes[x]);
  }
  printf("\n");
	for(unsigned int x=0; x<nNodes*nNodes; x++)
	{
		printf("%20.10f ", local[x]);
		if(!((x+1)%nNodes))
		  printf("\n");
	}
}

void GenericElement::updateGlobal(gsl_matrix *matrix, std::map<unsigned long,unsigned long> *hash_nodes,
		double condutividade)
{
	this->condutividade = condutividade;
	printf("Validando ...");
	for(unsigned int x=0; x<nNodes; x++) {
	if(hash_nodes->find(nodes[x]) == hash_nodes->end()) {
			printf("Falha! No %lu inexistente\n", nodes[x]);
			return;
		}
	}

	for(unsigned int x=0; x<nNodes; x++) {
		for(unsigned int y=0; y<nNodes; y++) {
			double d = gsl_matrix_get(matrix, hash_nodes->at(nodes[x]), hash_nodes->at(nodes[y]));
			d += (getFatorCondut()* local[x*nNodes + y]);
			gsl_matrix_set(matrix, hash_nodes->at(nodes[x]), hash_nodes->at(nodes[y]), d);
		}
	}
}

void GenericElement::updateJacobian(gsl_matrix *matrix, std::map<unsigned long,unsigned long> *hash_nodes,
		double condutividade)
{
	this->condutividade = condutividade;
	printf("Validando Nos...");
	for(unsigned int x=0; x<nNodes; x++) {
	if(hash_nodes->find(nodes[x]) == hash_nodes->end()) {
			printf("Falha! No %lu inexistente\n", nodes[x]);
			return;
		}
	}

	for(unsigned int x=0; x<nNodes; x++) {
		for(unsigned int y=0; y<nNodes; y++) {
			double d = gsl_matrix_get(matrix, hash_nodes->at(nodes[x]), hash_nodes->at(nodes[y]));
			d += (getDerivCondut()* local[x*nNodes + y]);
			gsl_matrix_set(matrix, hash_nodes->at(nodes[x]), hash_nodes->at(nodes[y]), d);
		}
	}
}
