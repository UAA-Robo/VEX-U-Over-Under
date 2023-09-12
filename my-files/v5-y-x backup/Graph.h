#pragma once
#include "Node.h"

class Graph
{
public:
  int nodesY;
  int nodesX;
  Node ***nodes;

  Graph(int nodesY, int nodesX);
  // ~Graph();
  Node *getNode(int y, int x);
};
