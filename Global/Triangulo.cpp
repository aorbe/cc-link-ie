#include <Triangulo.h>

void Triangulo::calcArea(std::map<unsigned long,Node> *list_nodes)
{
	area = (list_nodes->at(nodes[0]).x * list_nodes->at(nodes[1]).y) - (list_nodes->at(nodes[1]).x * list_nodes->at(nodes[0]).y);
	area += (list_nodes->at(nodes[2]).x * list_nodes->at(nodes[0]).y) - (list_nodes->at(nodes[0]).x * list_nodes->at(nodes[2]).y);
	area += (list_nodes->at(nodes[1]).x * list_nodes->at(nodes[2]).y) - (list_nodes->at(nodes[2]).x * list_nodes->at(nodes[1]).y);
	if (area <= 0) {
		printf("Erro Elemento %lu (A (%10.3lf)<= 0)", number, area);
		printf(" Nos %lu %lu %lu\n", nodes[0], nodes[1], nodes[2]);
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

int Triangulo::calcLocalMatrix(std::map<unsigned long,Node> *list_nodes)
{
	calcArea(list_nodes);
	Node *lNode[3];
	for(unsigned int x=0; x<nNodes; x++)
		lNode[x] = &list_nodes->at(nodes[x]);

	local[0] = ((lNode[0]->y - lNode[2]->y)*(lNode[1]->y - lNode[2]->y) +  (lNode[2]->x - lNode[1]->x)*(lNode[2]->x - lNode[1]->x));
	local[1] = ((lNode[1]->y - lNode[2]->y)*(lNode[2]->y - lNode[0]->y) +  (lNode[2]->x - lNode[1]->x)*(lNode[0]->x - lNode[2]->x));
	local[2] = ((lNode[1]->y - lNode[2]->y)*(lNode[0]->y - lNode[1]->y) +  (lNode[2]->x - lNode[1]->x)*(lNode[1]->x - lNode[0]->x));

	local[3] = local[1];
	local[4] = ((lNode[2]->y - lNode[0]->y)*(lNode[2]->y - lNode[0]->y) +  (lNode[0]->x - lNode[2]->x)*(lNode[0]->x - lNode[2]->x));
	local[5] = ((lNode[2]->y - lNode[0]->y)*(lNode[0]->y - lNode[1]->y) +  (lNode[0]->x - lNode[2]->x)*(lNode[1]->x - lNode[0]->x));

	local[6] = local[2];
	local[7] = local[5];
	local[8] = ((lNode[0]->y - lNode[1]->y)*(lNode[0]->y - lNode[1]->y) +  (lNode[1]->x - lNode[0]->x)*(lNode[1]->x - lNode[0]->x));
  
	return 1;
}

double Triangulo::getFatorCondut() {
	// H=1.5cm
	return 0.015 / (condutividade * 4.0 * area);
}

double Triangulo::getDerivCondut() {
	return -getFatorCondut()/condutividade;
}
