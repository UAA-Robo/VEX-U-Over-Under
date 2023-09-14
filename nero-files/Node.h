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

  Node(int x, int y);
  void addNeighbor(Node *node);
  void forbid();
  void print();
};