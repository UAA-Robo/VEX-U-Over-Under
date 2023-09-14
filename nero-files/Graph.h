#pragma once
#include <vector>
#include "Node.h"

class Graph
{
private:
  Node ***nodes;

public:
  int xNodes;
  int yNodes;

  Graph(int xNodes, int yNodes);
  Node *getNode(int x, int y);
  std::vector<Node *> getForbiddenNodes();
};
