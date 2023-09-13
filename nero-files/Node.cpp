#include "Node.h"

Node::Node(int x, int y)
{
  this->x = x;
  this->y = y;
  this->visited = false;
  this->parent = nullptr;
  this->forbidden = false;
};

void Node::addNeighbor(Node *neighborNode)
{
  neighbors.insert(neighborNode);
};