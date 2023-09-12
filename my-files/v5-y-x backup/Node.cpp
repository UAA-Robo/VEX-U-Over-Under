#pragma once
#include "Node.h"

using namespace std;

Node::Node(int y, int x)
{
  this->y = y;
  this->x = x;
  this->visited = false;
  this->parent = nullptr;
};

// Node::~Node()
// {
//   this->parent = nullptr;
//   for (int i = 0; i < this->neighbors.size(); i++)
//   {
//     this->neighbors[i] = nullptr;
//   }
// };

void Node::addNeighbor(Node *neighborNode)
{
  neighbors.push_back(neighborNode);
};