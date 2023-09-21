#pragma once
#include <vector>
#include <map>
#include "Node.h"

class Graph
{
private:
  Node ***nodes;
  std::set<Node *> waypoints;

  int getEdgeCost(Node *a, Node *b);
  int heuristic(Node *currentNode, Node *destination);
  std::vector<Node *> reconstructPath(Node *currentNode, std::map<Node *, Node *> cameFrom);
  POSITION getPositionRelative(Node *a, Node *b);

  // VG methods
  WAYPOINT isWaypoint(Node *node);
  bool isTaut(Node *a, Node *b);
  bool areTautWaypoints(Node *a, Node *b);
  void findWaypoints();
  void addNeighboringWaypoints();

public:
  int xNodes;
  int yNodes;
  bool hasLOS(Node *a, Node *b);
  std::set<Node *> getWaypoints();

  Graph(int xNodes, int yNodes);
  ~Graph();
  Node *getNode(int x, int y);
  std::vector<Node *> getForbiddenNodes();
  std::vector<Node *> getPath(Node *origin, Node *destination);
  std::vector<Node *> getRandomPath();
  std::vector<std::vector<Node *> *> getPathSnapshots(Node *origin, Node *destination);
  std::vector<std::vector<Node *> *> getRandomPathSnapshots();
  void forbidTriangle(Node *a, Node *b);
  void forbidRectangle(Node *topLeftPoint, Node *topRightPoint, Node *bottomLeftPoint);

  // VG methods
  void createVG();
  std::vector<Node *> getVGPath(Node *origin, Node *destination);
  std::vector<Node *> getVGRandomPath();
  std::vector<std::vector<Node *> *> getVGPathSnapshots(Node *origin, Node *destination);
  std::vector<std::vector<Node *> *> getVGRandomPathSnapshots();

  void removeWaypoint(Node *node);
  void insertWaypoint(Node *node);
};
