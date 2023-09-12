#pragma once
#include <vector>

using namespace std;

class Node
{
public:
  int x;
  int y;
  bool visited;
  Node *parent;
  vector<Node *> neighbors;

  Node(int x, int y);
  // ~Node();
  void addNeighbor(Node *node);
};