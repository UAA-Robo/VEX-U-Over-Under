#pragma once
#include "Graph.h"

Graph::Graph(){};

// void Graph::addNode(Node node, vector<Node> neighbors)
// {
//   nodes.push_back(node);

//   for (int i = 0; i < sizeof(neighbors); i++)
//   {
//     node.neighbors.push_back(neighbors[i]);
//   }
// }

Node *Graph::getNode(float x, float y)
{

  {
    for (int i = 0; i < this->nodes.size(); i++)
    {
      if (this->nodes[i]->x == x && this->nodes[i]->y == y)
      {
        return this->nodes[i];
      }
    }
  };
};