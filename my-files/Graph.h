#pragma once
#include "Node.h"

class Graph
{
public:
  int nodesX;
  int nodesY;
  Node ***nodes;

  Graph(int nodesX, int nodesY);
  // ~Graph();
  Node *getNode(int x, int y);
};
