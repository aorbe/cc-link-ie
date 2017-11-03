/*
 * Node.h
 *
 *  Created on: 17/08/2016
 *      Author: asantos
 */

#ifndef NODE_H_
#define NODE_H_

class Node
{
   public:
   unsigned long number;
   double x;
   double y;
   double z;

   int getData(const char* buf);
   unsigned long getNumber();
};

#endif /* NODE_H_ */
