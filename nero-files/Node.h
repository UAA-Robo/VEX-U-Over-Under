#pragma once
#include <set>
#include "Graph.h"

class Node
{
public:
  int x;
  int y;
  bool forbidden;
  std::set<Node *> neighbors;
  Graph::WAYPOINT waypoint;
  std::set<Node *> waypointNeighbors;

  Node(int x, int y);
  void addNeighbor(Node *node);
  void forbid();
  void print();
  void println();
};