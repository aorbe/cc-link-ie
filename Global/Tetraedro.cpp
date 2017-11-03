/*
 * Tetraedro.cpp
 *
 *  Created on: 24/10/2017
 *      Author: asantos
 */

#include "Tetraedro.h"
#include <stdio.h>
#include <list>

void Tetraedro::calcVolume(std::map<unsigned long,Node> *list_nodes)
{
  double M[3][3];
  printf("Calculando area do tetraedro de nos: %lu %lu %lu %lu\n", nodes[0], nodes[1], nodes[2], nodes[3]);
  for(unsigned int x=0; x<3; x++) {
    printf("Coordenada %u : %8.5f %8.5f %8.5f\n", x, list_nodes->at(nodes[x]).x, list_nodes->at(nodes[x]).y, list_nodes->at(nodes[x]).z);
    M[x][0] = list_nodes->at(nodes[3]).x - list_nodes->at(nodes[x]).x;
    M[x][1] = list_nodes->at(nodes[3]).y - list_nodes->at(nodes[x]).y;
    M[x][2] = list_nodes->at(nodes[3]).z - list_nodes->at(nodes[x]).z;
  }
  printf("Coordenada 3 : %8.5f %8.5f %8.5f\n",list_nodes->at(nodes[3]).x, list_nodes->at(nodes[3]).y, list_nodes->at(nodes[3]).z);
  volume = determinante(M[0], M[1], M[2]) / 6;
  printf("Volume do Tetraedo %lu = %e\n", number, volume);
  
}

Tetraedro::Tetraedro(unsigned long number, unsigned int type, unsigned int nTags) : GenericElement(number, type, nTags)
{
  nNodes = 4;
  nodes = new unsigned long[nNodes];
  local = new double[nNodes * nNodes];
}

Tetraedro::~Tetraedro()
{
  delete[] nodes;
  delete[] local;
}

int Tetraedro::calcLocalMatrix(long double new_value, std::map<unsigned long,Node> *list_nodes)
{
  condutividade = new_value;
  printf("Calculando Tetraedro...\n");
	calcVolume(list_nodes);

  Node* u1 = &list_nodes->at(nodes[0]);
  Node* u2 = &list_nodes->at(nodes[1]);
  Node* u3 = &list_nodes->at(nodes[2]);
  Node* u4 = &list_nodes->at(nodes[3]);

	// H=1.5cm
  double a1[3][3] = {{u2->x, u3->x, u4->x},{u2->y, u3->y, u4->y},{u2->z, u3->z, u4->z}};
  double a2[3][3] = {{u1->x, u3->x, u4->x},{u1->y, u3->y, u4->y},{u1->z, u3->z, u4->z}};
  double a3[3][3] = {{u1->x, u2->x, u4->x},{u1->y, u2->y, u4->y},{u1->z, u2->z, u4->z}};
  double a4[3][3] = {{u1->x, u2->x, u3->x},{u1->y, u2->y, u3->y},{u1->z, u2->z, u3->z}};
  double ones[3] = {1, 1, 1};

  
  double b1 = determinante(ones, a1[1], a1[2]);
  double c1 = determinante(ones, a1[0], a1[2]);
  double d1 = determinante(ones, a1[0], a1[1]);
  double b2 = determinante(ones, a2[1], a2[2]);
  double c2 = determinante(ones, a2[0], a2[2]);
  double d2 = determinante(ones, a2[0], a2[1]);
  double b3 = determinante(ones, a3[1], a3[2]);
  double c3 = determinante(ones, a3[0], a3[2]);
  double d3 = determinante(ones, a3[0], a3[1]);
  double b4 = determinante(ones, a4[1], a4[2]);
  double c4 = determinante(ones, a4[0], a4[2]);
  double d4 = determinante(ones, a4[0], a4[1]);

  double fator = 1/(36*volume*condutividade);

  local[0] = fator*(b1*b1+c1*c1+d1*d1);
  local[1] = fator*(b1*b2+c1*c2+d1*d2);
  local[2] = fator*(b1*b3+c1*c3+d1*d3);
  local[3] = fator*(b1*b4+c1*c4+d1*d4);
  local[4] = local[1];
  local[5] = fator*(b2*b2+c2*c2+d2*d2);
  local[6] = fator*(b2*b3+c2*c3+d2*d3);
  local[7] = fator*(b2*b4+c2*c4+d2*d4);
  local[8] = local[2];
  local[9] = local[6];
  local[10] = fator*(b3*b3+c3*c3+d3*d3);
  local[11] = fator*(b3*b4+c3*c4+d3*d4);
  local[12] = local[3];
  local[13] = local[7];
  local[14] = local[11];
  local[15] = fator*(b4*b4+c4*c4+d4*d4);
  
  return 1;

}

unsigned long Tetraedro::getNumber()
{
  return number;

}

