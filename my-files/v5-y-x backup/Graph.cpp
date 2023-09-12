#pragma once
#include <iostream>
#include "Graph.h"

Graph::Graph(int nodesY, int nodesX)
{
  this->nodesY = nodesY;
  this->nodesX = nodesX;
  this->nodes = new Node **[nodesY];
  for (int i = 0; i < nodesY; i++)
  {
    this->nodes[i] = new Node *[nodesX];
  }

  for (int y = 0; y < nodesY; y++)
  {
    for (int x = 0; x < nodesX; x++)
    {
      Node *tempNode = new Node(y, x);
      this->nodes[y][x] = tempNode;
      // this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);     // W
      // this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]);     // N
      // this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x - 1]); // NW
      // this->nodes[y][x]->addNeighbor(this->nodes[y + 1][x - 1]); // NE

      // this->nodes[y - 1][x]->addNeighbor(this->nodes[y][x]);     // W
      // this->nodes[y][x - 1]->addNeighbor(this->nodes[y][x]);     // N
      // this->nodes[y - 1][x - 1]->addNeighbor(this->nodes[y][x]); // NW
      // this->nodes[y + 1][x - 1]->addNeighbor(this->nodes[y][x]); // NE

      if (y == 0 && x == 0)
      {
        // top left point
        // no W, N, NW, NE, SW
      }
      else if (y == nodesY && x == 0)
      {
        // top right point
        // no N, E, NE, NW, ES
      }
      else if (y == nodesY && x == nodesX)
      {
        // bottom right point
        // no S, E, SE, SW, NE
      }
      else if (y == 0 && x == nodesX)
      {
        // bottom left point
        // no S, W, SW, SE, NW
      }
      else if (y == 0)
      {
        // left column
        // no WN, W, WS
      }
      else if (y == nodesY)
      {
        // right column
        // no EN, E, ES
      }
      else if (x == 0)
      {
        // top row
        // no NW, N, NE
      }
      else if (x == nodesX)
      {
        // bottom row
        // no SW, S, SE
      }

      // if (y > 0)
      // {
      //   this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);
      //   this->nodes[y - 1][x]->addNeighbor(this->nodes[y][x]);
      // };
      // if (x > 0)
      // {
      //   this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]);
      //   this->nodes[y][x - 1]->addNeighbor(this->nodes[y][x]);
      // };
    };
  };

  // for (int y = 0; y < nodesY; y++)
  // {
  //   for (int x = 0; x < nodesX; x++)
  //   {
  //     Node *tempNode = new Node(y, x);
  //     this->nodes[y][x] = tempNode;
  //     if (y > 0)
  //     {
  //       this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);
  //       this->nodes[y - 1][x]->addNeighbor(this->nodes[y][x]);
  //     };
  //     if (x > 0)
  //     {
  //       this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]);
  //       this->nodes[y][x - 1]->addNeighbor(this->nodes[y][x]);
  //     };
  //   };
  // };
};

// Graph::~Graph()
// {
//   for (int y = 0; y < this->nodesY; y++)
//   {
//     for (int x = 0; x < this->nodesX; x++)
//     {
//       // this->nodes[y][x] = nullptr;
//       delete this->nodes[y][x];
//     }
//     delete[] this->nodes[y];
//   }
//   delete[] this->nodes;
// }

Node *Graph::getNode(int y, int x)
{
  return this->nodes[y][x];
};