#pragma once
#include <iostream>
#include "Graph.h"

Graph::Graph(int nodesX, int nodesY)
{
  this->nodesX = nodesX;
  this->nodesY = nodesY;
  this->nodes = new Node **[nodesY];
  for (int x = 0; x < nodesY; x++)
  {
    this->nodes[x] = new Node *[nodesX];
  }

  for (int y = 0; y < nodesY; y++)
  {
    for (int x = 0; x < nodesX; x++)
    {
      Node *tempNode = new Node(x, y);
      this->nodes[y][x] = tempNode;
      // this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x + 1]); // NE
      // this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);     // N
      // this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x - 1]); // NW
      // this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]);     // W

      // this->nodes[y - 1][x + 1]->addNeighbor(this->nodes[y][x]); // NE
      // this->nodes[y - 1][x]->addNeighbor(this->nodes[y][x]);     // N
      // this->nodes[y - 1][x - 1]->addNeighbor(this->nodes[y][x]); // NW
      // this->nodes[y][x - 1]->addNeighbor(this->nodes[y][x]);     // W

      if (y == 0 && x == 0)
      {
        // top left point
        // no W, N, NW, NE, SW
      }
      else if (y == nodesY - 1 && x == 0)
      {
        // bottom left point
        // no S, W, SW, SE, NW

        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x + 1]); // NE
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);     // N

        this->nodes[y - 1][x + 1]->addNeighbor(this->nodes[y][x]); // NE
        this->nodes[y - 1][x]->addNeighbor(this->nodes[y][x]);     // N
      }
      else if (y == nodesY - 1 && x == nodesX - 1)
      {
        // bottom right point
        // no S, E, SE, SW, NE

        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);     // N
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x - 1]); // NW
        this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]);     // W

        this->nodes[y - 1][x]->addNeighbor(this->nodes[y][x]);     // N
        this->nodes[y - 1][x - 1]->addNeighbor(this->nodes[y][x]); // NW
        this->nodes[y][x - 1]->addNeighbor(this->nodes[y][x]);     // W
      }
      else if (y == 0 && x == nodesX - 1)
      {
        // top right point
        // no N, E, NE, NW, ES

        this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]); // W

        this->nodes[y][x - 1]->addNeighbor(this->nodes[y][x]); // W
      }
      else if (y == 0)
      {
        // top row
        // no NW, N, NE

        this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]); // W

        this->nodes[y][x - 1]->addNeighbor(this->nodes[y][x]); // W
      }
      else if (y == nodesY - 1)
      {
        // bottom row
        // no SW, S, SE

        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x + 1]); // NE
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);     // N
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x - 1]); // NW
        this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]);     // W

        this->nodes[y - 1][x + 1]->addNeighbor(this->nodes[y][x]); // NE
        this->nodes[y - 1][x]->addNeighbor(this->nodes[y][x]);     // N
        this->nodes[y - 1][x - 1]->addNeighbor(this->nodes[y][x]); // NW
        this->nodes[y][x - 1]->addNeighbor(this->nodes[y][x]);     // W
      }
      else if (x == 0)
      {
        // left column
        // no WN, W, WS

        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x + 1]); // NE
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);     // N

        this->nodes[y - 1][x + 1]->addNeighbor(this->nodes[y][x]); // NE
        this->nodes[y - 1][x]->addNeighbor(this->nodes[y][x]);     // N
      }
      else if (x == nodesX - 1)
      {
        // right column
        // no EN, E, ES

        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);     // N
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x - 1]); // NW
        this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]);     // W

        this->nodes[y - 1][x]->addNeighbor(this->nodes[y][x]);     // N
        this->nodes[y - 1][x - 1]->addNeighbor(this->nodes[y][x]); // NW
        this->nodes[y][x - 1]->addNeighbor(this->nodes[y][x]);     // W
      }
      else
      {
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x + 1]); // NE
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);     // N
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x - 1]); // NW
        this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]);     // W

        this->nodes[y - 1][x + 1]->addNeighbor(this->nodes[y][x]); // NE
        this->nodes[y - 1][x]->addNeighbor(this->nodes[y][x]);     // N
        this->nodes[y - 1][x - 1]->addNeighbor(this->nodes[y][x]); // NW
        this->nodes[y][x - 1]->addNeighbor(this->nodes[y][x]);     // W
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

Node *Graph::getNode(int x, int y)
{
  return this->nodes[y][x];
};