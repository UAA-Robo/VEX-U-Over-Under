#pragma once
#include "Node.h"

using namespace std;

Node::Node(int x, int y)
{
  this->x = x;
  this->y = y;
  this->visited = false;
  this->parent = nullptr;
  this->forbidden = false;
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