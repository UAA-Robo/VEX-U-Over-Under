#pragma once
#include <set>
#include "Enums.h"

class Node
{
public:
  int x;
  int y;
  bool forbidden;
  WAYPOINT waypoint;
  std::set<Node *> neighbors;
  std::set<Node *> waypointNeighbors;

  Node(int x, int y);
  void addNeighbor(Node *neighborNode);
  void addWaypointNeighbor(Node *node);
  void forbid();
  void print();
  void println();
};