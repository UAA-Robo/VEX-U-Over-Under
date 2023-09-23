// Remove redundant this->
// Standarize between node, currentNode, a, b
// do constructors and deconstructors need to be virtual?
// pass heavy stuff around with pointers or by reference

#pragma once
#include <vector>
#include <map>
#include "Node.h"

class Graph
{
protected:
  Node ***nodes;
  std::vector<Node *> forbiddenNodes;

  POSITION getPositionRelative(Node *a, Node *b);
  int getEdgeCost(Node *a, Node *b);
  std::vector<Node *> reconstructPath(Node *currentNode, std::map<Node *, Node *> cameFrom);

public:
  int xNodes;
  int yNodes;
  double cellSize;
  std::set<Node *> waypoints;
  virtual bool hasLOS(Node *a, Node *b){};
  virtual void createVG(){};

  Graph(int xNodes, int yNodes, double cellSize);
  ~Graph();
  Node *getNode(int x, int y);
  void addForbiddenNode(Node *node);
  std::vector<Node *> getForbiddenNodes();
  virtual std::vector<Node *> getPath(Node *origin, Node *destination);
  virtual std::vector<Node *> getRandomPath();
  virtual std::vector<std::vector<Node *> *> getPathSnapshots(Node *origin, Node *destination);
  virtual std::vector<std::vector<Node *> *> getRandomPathSnapshots();
  virtual void forbidTriangle(Node *a, Node *b);
  virtual void forbidRectangle(Node *topLeftPoint, Node *topRightPoint, Node *bottomLeftPoint);
  Node* getHit();
};

// shared = not virtual, in parent class, protected

// different = virtual, in parent class, for same call methods, etc.
//(to non graph classes users, for public API use)

// different = no virtual, own class