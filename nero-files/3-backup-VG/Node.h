#pragma once
#include <set>

class Node
{
public:
  int x;
  int y;
  bool forbidden;
  std::set<Node *> neighbors;

  bool waypoint;
  std::set<Node *> tautVisibleWaypoints;

  Node(int x, int y);
  ~Node();
  void addNeighbor(Node *node);
  void forbid();
  void print();
};