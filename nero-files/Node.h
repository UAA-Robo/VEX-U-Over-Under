#pragma once
#include <set>
#include "Enums.h"

class Node
{
public:
  int x;
  int y;
  bool forbidden;
  std::set<Node *> neighbors;
  WAYPOINT waypoint;
  std::set<Node *> waypointNeighbors;

  Node(int x, int y);
  void addNeighbor(Node *node);
  void forbid();
  void print();
  void println();
};