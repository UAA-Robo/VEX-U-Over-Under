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

  int heuristic(Node *currentNode, Node *destination);
  std::vector<Node *> reconstructPath(Node *currentNode, std::map<Node *, Node *> cameFrom);
  // std::vector<std::vector<Node *> *> reconstructPath(Node *currentNode, std::map<Node *, Node *> cameFrom);
  int getEdgeCost(Node *a, Node *b);
  void reset();

public:
  int xNodes;
  int yNodes;

  Graph(int xNodes, int yNodes);
  Node *getNode(int x, int y);
  std::vector<Node *> getForbiddenNodes();
  std::vector<Node *> getPath(Node *origin, Node *destination);
  std::vector<Node *> getRandomPath();
  std::vector<std::vector<Node *> *> getRandomPathSnapshots();
  std::vector<std::vector<Node *> *> getPathSnapshots(Node *origin, Node *destination);
};
