#pragma once
#include <set>

class Node
{
public:
  int x;
  int y;
  bool forbidden;
  Node *parent;
  std::set<Node *> neighbors;
  bool waypoint;
  std::set<Node*> tautVisibleWaypoints;

  Node(int x, int y);
  void addNeighbor(Node *node);
  void forbid();
  void print();
};