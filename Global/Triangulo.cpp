#include <Triangulo.h>

void Triangulo::calcArea(std::map<unsigned long,Node> *list_nodes)
{
  area = (list_nodes->at(nodes[0]).x * list_nodes->at(nodes[2]).y) - (list_nodes->at(nodes[2]).x * list_nodes->at(nodes[0]).y);
	area -= (list_nodes->at(nodes[0]).x * list_nodes->at(nodes[1]).y) - (list_nodes->at(nodes[1]).x * list_nodes->at(nodes[0]).y);
	area -= (list_nodes->at(nodes[1]).x * list_nodes->at(nodes[2]).y) - (list_nodes->at(nodes[2]).x * list_nodes->at(nodes[1]).y);
	if (area <= 0) {
		printf("\n\nErro Elemento %lu (A (%10.3lf )<= 0)\n", number, area);
    printf("Nos %lu %lu %lu\n\n", nodes[0], nodes[1], nodes[2]);
  }
}

Triangulo::Triangulo(unsigned long number, unsigned int type, unsigned int nTags): GenericElement(number, type, nTags)
{
  nNodes = 3;
  nodes = new unsigned long[nNodes];
  local = new double[nNodes * nNodes];
}

Triangulo::~Triangulo()
{
  delete[] nodes;
  delete[] local;
}

int Triangulo::calcLocalMatrix(long double _condutividade, std::map<unsigned long,Node> *list_nodes)
{
	long double k;
	condutividade = _condutividade;
	calcArea(list_nodes);
	// H=1.5cm
	k = 0.015 / (condutividade * 4.0 * area);
  Node *lNode[3];
  for(unsigned int x=0; x<nNodes; x++)
    lNode[x] = &list_nodes->at(nodes[x]);

	local[0] = k * ((lNode[0]->y - lNode[2]->y)*(lNode[1]->y - lNode[2]->y) +  (lNode[2]->x - lNode[1]->x)*(lNode[2]->x - lNode[1]->x));
	local[1] = k * ((lNode[1]->y - lNode[2]->y)*(lNode[2]->y - lNode[0]->y) +  (lNode[2]->x - lNode[1]->x)*(lNode[0]->x - lNode[2]->x));
	local[2] = k * ((lNode[1]->y - lNode[2]->y)*(lNode[0]->y - lNode[1]->y) +  (lNode[2]->x - lNode[1]->x)*(lNode[1]->x - lNode[0]->x));

	local[3] = local[1];
	local[4] = k * ((lNode[2]->y - lNode[0]->y)*(lNode[2]->y - lNode[0]->y) +  (lNode[0]->x - lNode[2]->x)*(lNode[0]->x - lNode[2]->x));
	local[5] = k * ((lNode[2]->y - lNode[0]->y)*(lNode[0]->y - lNode[1]->y) +  (lNode[0]->x - lNode[2]->x)*(lNode[1]->x - lNode[0]->x));

	local[6] = local[2];
	local[7] = local[5];
	local[8] = k * ((lNode[0]->y - lNode[1]->y)*(lNode[0]->y - lNode[1]->y) +  (lNode[1]->x - lNode[0]->x)*(lNode[1]->x - lNode[0]->x));
  
  return 1;
}

