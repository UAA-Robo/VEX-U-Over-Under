#pragma once
#include "Node.h"

using namespace std;

// Node::Node(float x = 0, float y = 0)
Node::Node(float x, float y)
{
  this->x = x;
  this->y = y;
};

void Node::addNeighbor(Node *neighborNode)
{
  neighbors.push_back(neighborNode);
};