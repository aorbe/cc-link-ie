/*
 * MeshFile.h
 *
 *  Created on: 17/08/2016
 *      Author: asantos
 */
#ifndef MESHFILE_H_
#define MESHFILE_H_

#include <iostream>
#include <list>
#include <map>

#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_vector.h>

#include "GenericElement.h"
#include "Node.h"

class MeshFile
{
#define MAX_LINE_LENGTH 255
  int mode;
  char* buf, *key;
  char MeshVersion[16], FileType[16];
  unsigned int DataSize;
  long nNodes, nPhysNames, nElements, nPeriodic, nIntNodeData, nRealNodeData, nStrNodeData, nIntElementData, nRealElementData, nStrElementData, nIntElementNodeData, nRealElementNodeData, nStrElementNodeData;
  long iNodes, iPhysNames, iElements, iPeriodic, iIntNodeData, iRealNodeData, iStrNodeData, iIntElementData, iRealElementData, iStrElementData, iIntElementNodeData, iRealElementNodeData, iStrElementNodeData;
  long getLength(long *n);
  FILE* mshFile;
protected:
  std::list<GenericElement*> elements;
  std::map<unsigned long, Node> nodes;
  std::map<unsigned long, unsigned long> gNodes;
  void ProcessMeshFormat();
  void ProcessPhysicalNames();
  void ProcessNodes();
  void ProcessElements();
  void ProcessPeriodic();
  void ProcessNodeData();
  void ProcessElementData();
  void ProcessElementNodeData();
  void ProcessInterpolationScheme();

public:
  MeshFile(int mode);
  int ReadFile(const char* fileName);
  void calcGlobal(gsl_matrix *global, gsl_vector* condutividade);
  void calcJacobian(gsl_matrix *jacobian, gsl_vector* condutividade);
  void showLocal(unsigned long number);
  unsigned int getNodesSize();
  unsigned long getNodeReference(unsigned long nodeNumber);
  void showImage();
  unsigned int getElementSize();
};
#endif /* MESHFILE_H_ */
