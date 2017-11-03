#include "Node.h"
#include <stdio.h>

int Node::getData(const char* buf)
{
  int iRet = sscanf(buf, "%lu %lf %lf %lf", &number, &x, &y, &z);
  if (4 == iRet)
    return 1;
  return 0;
}

unsigned long Node::getNumber()
{
  return number;
}

