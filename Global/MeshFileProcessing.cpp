#include "MeshFile.h"
#include <string.h>
#include <functional>
#include <iostream>


#include "Line.h"
#include "Tetraedro.h"
#include "Triangulo.h"
#include "Point.h"
#include "Line.h"

#if (0)
int main()
{
  MeshFile teste;
  teste.ReadFile("cuba.msh");
  return 0;
}
#endif

MeshFile::MeshFile(int mode)
{
  this->mode = mode;
  nNodes = -1;
  nPhysNames = -1;
  nElements = -1;
  nPeriodic = -1;
  nIntNodeData= -1;
  nRealNodeData = -1;
  nStrNodeData = -1;
  nIntElementData = -1;
  nRealElementData = -1;
  nStrElementData = -1;
  nIntElementNodeData = -1;
  nRealElementNodeData = -1;
  nStrElementNodeData = -1;
  iNodes = 0;
  iPhysNames = 0;
  iElements = 0;
  iPeriodic = 0;
  iIntNodeData = 0;
  iRealNodeData = 0;
  iStrNodeData = 0;
  iIntElementData = 0;
  iRealElementData = 0;
  iStrElementData = 0;
  iIntElementNodeData = 0;
  iRealElementNodeData = 0;
  iStrElementNodeData = 0;
  
}

/*
 * Le arquivo mesh associando cada chave a uma função
 */
int MeshFile::ReadFile(const char* FileName)
{
  std::function<void(void)> f[] = {
    std::bind(&MeshFile::ProcessMeshFormat, this), 
    std::bind(&MeshFile::ProcessPhysicalNames, this),
    std::bind(&MeshFile::ProcessNodes, this),
    std::bind(&MeshFile::ProcessElements, this),
    std::bind(&MeshFile::ProcessPeriodic, this),
    std::bind(&MeshFile::ProcessNodeData, this),
    std::bind(&MeshFile::ProcessElementData, this),
    std::bind(&MeshFile::ProcessElementNodeData, this),
    std::bind(&MeshFile::ProcessInterpolationScheme, this)
  };
  std::map<std::string, std::function<void(void)>&> funcMap = {
    {std::string("$MeshFormat\n"), f[0]},
    {std::string("$PhysicalNames\n"), f[1]},
    {std::string("$Nodes\n"), f[2]},
    {std::string("$Elements\n"), f[3]},
    {std::string("$Periodic\n"), f[4]},
    {std::string("$NodeData\n"), f[5]},
    {std::string("$ElementData\n"), f[6]},
    {std::string("$ElementNodeData\n"), f[7]},
    {std::string("$InterpolationScheme\n"), f[8]}
  };

  std::map<std::string, std::function<void(void)>&>::iterator it;

  if ((mshFile = fopen(FileName, "r")) == NULL) {
    printf("File Open Error\n");
    return -1;
  }

  key = (char *)malloc(MAX_LINE_LENGTH + 1);
  buf = (char *)malloc(MAX_LINE_LENGTH + 1);
  while(!feof(mshFile) && (NULL != fgets(buf, MAX_LINE_LENGTH, mshFile))) {
    it = funcMap.find(std::string(buf));
    if (it == funcMap.end()) {
      printf("Error: %s", buf);
      continue;
    }
    printf("Start %s", &buf[1]);
    sprintf(key, "$End%s", &buf[1]);
    while(!feof(mshFile)) {
      fgets(buf, MAX_LINE_LENGTH, mshFile);
      if (strcmp(buf, key) == 0) {
        break;
      }
      it->second();
      //printf("%s", buf);
    }
    printf("End %s\n", &buf[4]);

  }; 
  free(buf);
  fclose(mshFile);  
  return 0;
}

// Versão, Tipo e Tamanho do arquivo são armazenados porém não utilizados
void MeshFile::ProcessMeshFormat()
{
  int iRead = sscanf(buf, "%s %s %d", MeshVersion, FileType, &DataSize);
  if (iRead != 3)
    printf("MeshFormat Error\n");
}

// Os nomes físicos não são utilizados
void MeshFile::ProcessPhysicalNames()
{
  for(;iPhysNames < getLength(&nPhysNames); iPhysNames++) {
  }
}

// O nós são armazenados no map "nodes" utilizando
// seu número como chave
void MeshFile::ProcessNodes()
{
  if(iNodes < getLength(&nNodes)) {
    Node *n = new Node();
    if (n->getData(buf))
        nodes.insert(std::pair<unsigned long, Node>(n->getNumber(),*n));
    else {
      delete n;
    }
    iNodes++;
  }
  if (iNodes == getLength(&nNodes))
    printf("Foram lidos %ld Nodes\n", iNodes);
}

// Cada elemento possui uma classe herdada de GenericElement associada
// Essas classes são armazenadas na lista "elements"
void MeshFile::ProcessElements()
{
  static std::map<unsigned int, unsigned int> stats;
  std::map<unsigned int, unsigned int>::iterator it;
  std::map<unsigned long, unsigned long>::iterator elem;
  unsigned long number;
  unsigned int type;
  unsigned int nTags;
  int iPos = 0, iAux= 0;
  GenericElement *e;

  // Verificação redundante se já foram lidos todos os elementos
  if(iElements < getLength(&nElements)) {
    sscanf(buf, "%ld %u %d%n", &number, &type, &nTags, &iPos);
      
    // A quantidade de elementos de cada tipo é registrada em "stats" 
    it = stats.find(type);
    if (stats.end() != it) {
      it->second++;
    } else {
      stats[type] = 1;
    }

    // De acordo com o tipo, uma classe é instanciada
    switch (type) {
    case 2:
      e = new Triangulo(number, type, nTags);
      break;
    case 3:
      e = new Tetraedro(number, type, nTags);
      break;
    case 4:
      e = new Tetraedro(number, type, nTags);
      break;
    case 15:
      e = new Point(number, type, nTags);
      break;
    default:
    case 1:
      e = new Line(number, type, nTags);
      break;
    }

    // Leitura dos tags
    int tag;
    for(unsigned int x=0; x<e->getNumberTags(); x++) {
      sscanf(&buf[iPos], "%d%n", &tag, &iAux);
      iPos += iAux;
      e->setTag(x, tag);
    }

    // A respectiva classe realiza a leitura dos nos
    e->readNodes(&buf[iPos]);

    // No modo 2D somente elementos tipo 2 serão calculados
    // No modo 3D somente elementos tipo 3 serão calculados
    if (((type==2) && (mode==2)) || ((type==3) && (mode==3))) {
      // Para cada nó pertencente a elemento calculado tem um número sequencial
      // associado indicando a coluna/linha relativa na matriz global
      for(unsigned int x=0; x<e->getNumberNodes(); x++) {
        unsigned long n = e->getNode(x);        
        elem = gNodes.find(n);
        if (gNodes.end() == elem) {
          gNodes[n] = gNodes.size() - 1;
        } 
      }
      
    }      
    elements.push_back(e);
    iElements++;
  }
  // Apresentação dos totais relativos aos elementos do arquivo
  if (iElements == getLength(&nElements)) {
    printf("Foram lidos %ld Elementos\n", iElements);
    for (it=stats.begin(); it!=stats.end(); ++it) {
      printf("* %u elementos do tipo %u\n", it->second, it->first);
    }
    printf("Total de %lu hashs\n", gNodes.size());
  }
}

void MeshFile::ProcessPeriodic()
{
  for(;iPeriodic < getLength(&nPeriodic); iPeriodic++) {
  }
}

void MeshFile::ProcessNodeData()
{
  for(;iStrNodeData < getLength(&nStrNodeData); iStrNodeData++) {
  }
  for(;iRealNodeData < getLength(&nRealNodeData); iRealNodeData++) {
  }
  for(;iIntNodeData < getLength(&nIntNodeData); iIntNodeData++) {
  }
}

void MeshFile::ProcessElementData()
{
  for(;iStrElementData < getLength(&nStrElementData); iStrElementData++) {
  }
  for(;iRealElementData < getLength(&nRealElementData); iRealElementData++) {
  }
  for(;iIntElementData < getLength(&nIntElementData); iIntElementData++) {
  }
}

void MeshFile::ProcessElementNodeData()
{
  for(;iStrElementNodeData < getLength(&nStrElementNodeData); iStrElementNodeData++) {
  }
  for(;iRealElementNodeData < getLength(&nRealElementNodeData); iRealElementNodeData++) {
  }
  for(;iIntElementNodeData < getLength(&nIntElementNodeData); iIntElementNodeData++) {
  }
}

void MeshFile::ProcessInterpolationScheme()
{
  printf("%s", buf);
}

unsigned int MeshFile::getNodesSize()
{
  return gNodes.size();
}




