#pragma once
#include <vector>
#include <map>
#include <cstdlib>
#include <random>
#include "Node.h"

class Graph
{
private:
  Node ***nodes;

  int getEdgeCost(Node *a, Node *b);
  int heuristic(Node *currentNode, Node *destination);
  std::vector<Node *> reconstructPath(Node *currentNode, std::map<Node *, Node *> cameFrom);

public:
  int xNodes;
  int yNodes;

  Graph(int xNodes, int yNodes);
  ~Graph();
  Node *getNode(int x, int y);
  std::vector<Node *> getForbiddenNodes();
  std::vector<Node *> getPath(Node *origin, Node *destination);
  std::vector<Node *> getRandomPath();
  std::vector<std::vector<Node *> *> getPathSnapshots(Node *origin, Node *destination);
  std::vector<std::vector<Node *> *> getRandomPathSnapshots();
  int getPositionRelative(Node *a, Node *b);
  void forbidTriangle(Node *a, Node *b);
  void forbidRectangle(Node *topLeftPoint, Node *topRightPoint, Node *bottomLeftPoint);
};
