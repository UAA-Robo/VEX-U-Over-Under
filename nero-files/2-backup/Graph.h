#pragma once
#include <vector>
#include "Node.h"
#include "Triangle.h"
#include "Rectangle.h"

class Graph
{
private:
  Node ***nodes;

public:
  int xNodes;
  int yNodes;

  Graph(int xNodes, int yNodes);
  Graph(int xNodes, int yNodes, std::vector<Triangle> forbiddenZonesTriangles, std::vector<Rectangle> forbiddenZonesRectangles);
  Node *getNode(int x, int y);
  void forbidNode(int x, int y);
  void forbidNode(Node *node);
  std::vector<Node*> getForbiddenNodes();
};
