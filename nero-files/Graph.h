#pragma once
#include <vector>
#include <map>
#include <cstdlib>
#include <random>
#include "Enums.h"
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
  bool bIsTaut(Node *a, Node *b);
  bool areTautWaypoints(Node *a, Node *b);
  bool hasLOS(Node *a, Node *b);
  void findWaypoints();
  void addNeighboringWaypoints();

  void initVG();

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
  void forbidTriangle(Node *a, Node *b);
  void forbidRectangle(Node *topLeftPoint, Node *topRightPoint, Node *bottomLeftPoint);

  // VG methods
  std::set<Node *> getWaypoints();
};

// VG
// * Create grid
// * Mark forbidden nodes
// For each node, if it is a corner node, or between two corner forbidden nodes, add it and mark it as a waypoint
// For each waypoint in the waypoints collection, search its neighboring waypoints while calculating their edge

// SVG
// For each waypoint in the waypoints collection, make sure it only has taut neighbors

// ENLSVG