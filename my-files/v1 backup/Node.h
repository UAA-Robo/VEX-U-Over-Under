#pragma once
#include <vector>

using namespace std;

class Node
{
public:
  float x;
  float y;
  bool visited = false;
  Node *parent;
  vector<Node *> neighbors;

  Node(float x = 0, float y = 0);

  void addNeighbor(Node *node);
};