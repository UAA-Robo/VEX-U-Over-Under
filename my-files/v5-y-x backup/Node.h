#pragma once
#include <vector>

using namespace std;

class Node
{
public:
  int y;
  int x;
  bool visited;
  Node *parent;
  vector<Node *> neighbors;

  Node(int y, int x);
  // ~Node();
  void addNeighbor(Node *node);
};