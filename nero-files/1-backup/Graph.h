#pragma once
#include "Node.h"

class Graph
{
public:
  int xNodes;
  int yNodes;
  Node ***nodes;

  Graph(int xNodes, int yNodes);
  // ~Graph();
  Node *getNode(int x, int y);
};
