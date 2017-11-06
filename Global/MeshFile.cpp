#include "MeshFile.h"
#include <string.h>
#include <functional>
#include <map>
#include <iostream>


#include "Line.h"
#include "Tetraedro.h"
#include "Triangulo.h"

long MeshFile::getLength(long *n)
{
  if (*n > 0)
    return *n;
  int iRead = sscanf(buf, "%ld", n);
  if ((iRead != 1) || (*n < 1)) {
    printf("Invalid number of elements\n");
    return 0;
  }
  printf("Size %ld \n", *n);
  return 0;
}

void MeshFile::calcGlobal(gsl_matrix *global, long double condutividade)
{
  (void) global;
  std::list<GenericElement*>::iterator it;
  for (it=elements.begin(); it!=elements.end(); ++it) {
    printf("Calculando Elemento %lu (Tipo %d)...", (*it)->getNumber(), (*it)->getType());
    if ((*it)->calcLocalMatrix(condutividade, &nodes)) {
      printf(" Atualizando ... ");
      (*it)->updateGlobal(global, &gNodes);
      printf("Pronto\n");
    } else {
      printf("Descartado\n");
    }
  }
}

void MeshFile::showLocal(unsigned long number)
{
  std::list<GenericElement*>::iterator it;
  for (it=elements.begin(); it!=elements.end(); ++it) {
    if (number == (*it)->getNumber()) {
      printf("Elemento %lu Encontrado\n", (*it)->getNumber());
      (*it)->showLocal();
      return;
    }    
  }
  printf("Elemento Não Encontrado\n");
}

unsigned long MeshFile::getNodeReference(unsigned long nodeNumber)
{
  std::map<unsigned long, unsigned long>::iterator elem;
  elem = gNodes.find(nodeNumber);
  if (gNodes.end() != elem)
    return elem->second;
  return getNodesSize() + 1; 
}



