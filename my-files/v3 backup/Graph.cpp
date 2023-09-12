#pragma once
#include "Graph.h"

Graph::Graph(int nodesX, int nodesY)
{
  this->nodesX = nodesX;
  this->nodesY = nodesY;
  this->nodes = new Node **[nodesX];
  for (int i = 0; i < nodesX; i++)
  {
    this->nodes[i] = new Node *[nodesY];
  }
};

// Graph::~Graph()
// {
//   for (int x = 0; x < this->nodesX; x++)
//   {
//     for (int y = 0; y < this->nodesY; y++)
//     {
//       // this->nodes[x][y] = nullptr;
//       delete this->nodes[x][y];
//     }
//     delete[] this->nodes[x];
//   }
//   delete[] this->nodes;
// }

Node *Graph::getNode(int x, int y)
{
  return this->nodes[x][y];
};