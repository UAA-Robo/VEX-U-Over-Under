#include "Node.h"
#include <iostream>

//=============================================================================
//=============================================================================
// PUBLIC METHODS
//=============================================================================
//=============================================================================

Node::Node(int x, int y)
{
  this->x = x;
  this->y = y;
  forbidden = false;
};

void Node::addNeighbor(Node *neighborNode)
{

  neighbors.insert(neighborNode);
  neighborNode->neighbors.insert(this);
};

void Node::forbid()
{
  forbidden = true;

  for (Node *neighbor : neighbors)
  {
    neighbor->neighbors.erase(this);
  }

  neighbors.clear();
}

void Node::print()
{
  std::cout << "(" << x << ", " << y << ")\t";
}

void Node::println()
{
  std::cout << "(" << x << ", " << y << ")\n";
}
