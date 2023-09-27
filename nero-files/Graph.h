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
public:
  GraphType type;
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
  Node *getHit();

  void add_edge(Node *a, Node *b);
  void remove_edge(Node *a, Node *b);

  void addForbiddenNodes(Graph *graph);

protected:
  Node ***nodes;
  std::vector<Node *> forbiddenNodes;

  Position getPositionRelative(Node *a, Node *b);
  int getEdgeCost(Node *a, Node *b);
  std::vector<Node *> reconstructPath(Node *currentNode, std::map<Node *, Node *> cameFrom);

private:
};
