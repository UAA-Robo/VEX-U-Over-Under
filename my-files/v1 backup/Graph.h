#pragma once
#include <vector>
#include "Node.h"

class Graph
{
public:
  vector<Node *> nodes;

  Graph();

  Node *getNode(float x, float y);
};
