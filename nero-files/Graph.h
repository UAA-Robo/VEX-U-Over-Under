#pragma once
#include <vector>
#include <map>
#include <cstdlib>
#include <random>
#include "Node.h"

class Graph
{
public:
  enum POSITION
  {
    B_TOP_A,
    B_RIGHT_A,
    B_BOTTOM_A,
    B_LEFT_A,
    B_TOPLEFT_A,
    B_TOPRIGHT_A,
    B_BOTTOMRIGHT_A,
    B_BOTTOMLEFT_A
  };

  enum WAYPOINT
  {
    NO,
    TOPLEFT,
    TOPRIGHT,
    BOTTOMRIGHT,
    BOTTOMLEFT
  };

private:
  Node ***nodes;
  std::set<Node *> waypoints;

  int getEdgeCost(Node *a, Node *b);
  int heuristic(Node *currentNode, Node *destination);
  std::vector<Node *> reconstructPath(Node *currentNode, std::map<Node *, Node *> cameFrom);
  POSITION getPositionRelative(Node *a, Node *b);

  WAYPOINT isWaypoint(Node *node);
  std::set<Node *> findWaypoints(Node *node);
  void Graph::addNeighboringWaypoints(Node *node);
  
  bool areTautNeighbors(Node *a, Node *b);

  bool canSeeEachOther(Node *a, Node *b);
  void findNeighborWaypoints();

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
};

// VG
// * Create grid
// * Mark forbidden nodes
// For each node, if it is a corner node, or between two corner forbidden nodes, add it and mark it as a waypoint
// For each waypoint in the waypoints collection, search its neighboring waypoints while calculating their edge

// SVG
// For each waypoint in the waypoints collection, make sure it only has taut neighbors

// ENLSVG