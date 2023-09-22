#pragma once
#include <set>
#include "Enums.h"

class Graph;

class Node
{
public:
  int x;
  int y;
  bool forbidden;
  Graph *graph;
  WAYPOINT waypoint;
  std::set<Node *> neighbors;
  std::set<Node *> waypointNeighbors;

  Node(int x, int y, Graph *graph);
  void addNeighbor(Node *neighborNode);
  void addWaypointNeighbor(Node *node);
  void forbid();
  void print();
  void println();
};