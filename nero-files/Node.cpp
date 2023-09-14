#include "Node.h"
#include <iostream>

Node::Node(int x, int y)
{
  this->x = x;
  this->y = y;
  this->forbidden = false;
  this->parent = nullptr;
};

void Node::addNeighbor(Node *neighborNode)
{

  this->neighbors.insert(neighborNode);
  neighborNode->neighbors.insert(this);
};

void Node::forbid()
{
  this->forbidden = true;

  for (Node *neighbor : this->neighbors)
  {
    neighbor->neighbors.erase(this);
  }

  this->neighbors.clear();
}

void Node::print()
{
  std::cout << "(" << this->x << ", " << this->y << ")\t";
}