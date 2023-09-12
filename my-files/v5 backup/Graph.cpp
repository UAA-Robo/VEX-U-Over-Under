#pragma once
#include <iostream>
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

  for (int x = 0; x < nodesX; x++)
  {
    for (int y = 0; y < nodesY; y++)
    {
      // bool zz = false;
      // for (int xx = 0; xx < nodesX; xx++)
      // {
      //   for (int yy = 0; yy < nodesY; y++)
      //   {
      //     if (this->nodes[xx][yy] == nullptr || this->nodes[xx][yy] == NULL)
      //     {
      //       zz = true;
      //       break;
      //     }
      //     else
      //     {
      //       std::cout << "(" << this->nodes[xx][yy]->x << ", " << this->nodes[xx][yy]->y << ")\n";
      //     }
      //   }
      //   if (zz)
      //   {
      //     break;
      //   }
      // }

      Node *tempNode = new Node(x, y);
      this->nodes[x][y] = tempNode;
      // this->nodes[x][y]->addNeighbor(this->nodes[x - 1][y]);     // W
      // this->nodes[x][y]->addNeighbor(this->nodes[x][y - 1]);     // N
      // this->nodes[x][y]->addNeighbor(this->nodes[x - 1][y - 1]); // NW
      // this->nodes[x][y]->addNeighbor(this->nodes[x + 1][y - 1]); // NE

      // this->nodes[x - 1][y]->addNeighbor(this->nodes[x][y]);     // W
      // this->nodes[x][y - 1]->addNeighbor(this->nodes[x][y]);     // N
      // this->nodes[x - 1][y - 1]->addNeighbor(this->nodes[x][y]); // NW
      // this->nodes[x + 1][y - 1]->addNeighbor(this->nodes[x][y]); // NE

      if (x == 0 && y == 0)
      {
        // top left point
        // no W, N, NW, NE, SW
      }
      else if (x == nodesX && y == 0)
      {
        // top right point
        // no N, E, NE, NW, ES

        this->nodes[x][y]->addNeighbor(this->nodes[x - 1][y]); // W

        this->nodes[x - 1][y]->addNeighbor(this->nodes[x][y]); // W
      }
      else if (x == nodesX && y == nodesY)
      {
        // bottom right point
        // no S, E, SE, SW, NE

        this->nodes[x][y]->addNeighbor(this->nodes[x - 1][y]);     // W
        this->nodes[x][y]->addNeighbor(this->nodes[x][y - 1]);     // N
        this->nodes[x][y]->addNeighbor(this->nodes[x - 1][y - 1]); // NW

        this->nodes[x - 1][y]->addNeighbor(this->nodes[x][y]);     // W
        this->nodes[x][y - 1]->addNeighbor(this->nodes[x][y]);     // N
        this->nodes[x - 1][y - 1]->addNeighbor(this->nodes[x][y]); // NW
      }
      else if (x == 0 && y == nodesY)
      {
        // bottom left point
        // no S, W, SW, SE, NW

        this->nodes[x][y]->addNeighbor(this->nodes[x][y - 1]);     // N
        this->nodes[x][y]->addNeighbor(this->nodes[x + 1][y - 1]); // NE

        this->nodes[x][y - 1]->addNeighbor(this->nodes[x][y]);     // N
        this->nodes[x + 1][y - 1]->addNeighbor(this->nodes[x][y]); // NE
      }
      else if (x == 0)
      {
        // left column
        // no WN, W, WS
        this->nodes[x][y]->addNeighbor(this->nodes[x][y - 1]);     // N
        this->nodes[x][y]->addNeighbor(this->nodes[x + 1][y - 1]); // NE

        this->nodes[x][y - 1]->addNeighbor(this->nodes[x][y]); // N
        // int z = (*this->nodes[x + 1][y - 1]).x;
        // int z = (*this->nodes[x][y]).x;
        this->nodes[x + 1][y - 1]->addNeighbor(this->nodes[x][y]); // NE
      }
      else if (x == nodesX)
      {
        // right column
        // no EN, E, ES
        this->nodes[x][y]->addNeighbor(this->nodes[x - 1][y]);     // W
        this->nodes[x][y]->addNeighbor(this->nodes[x][y - 1]);     // N
        this->nodes[x][y]->addNeighbor(this->nodes[x - 1][y - 1]); // NW

        this->nodes[x - 1][y]->addNeighbor(this->nodes[x][y]);     // W
        this->nodes[x][y - 1]->addNeighbor(this->nodes[x][y]);     // N
        this->nodes[x - 1][y - 1]->addNeighbor(this->nodes[x][y]); // NW
      }
      else if (y == 0)
      {
        // top row
        // no NW, N, NE
        this->nodes[x][y]->addNeighbor(this->nodes[x - 1][y]); // W

        this->nodes[x - 1][y]->addNeighbor(this->nodes[x][y]); // W
      }
      else if (y == nodesY)
      {
        // bottom row
        // no SW, S, SE
        this->nodes[x][y]->addNeighbor(this->nodes[x - 1][y]);     // W
        this->nodes[x][y]->addNeighbor(this->nodes[x][y - 1]);     // N
        this->nodes[x][y]->addNeighbor(this->nodes[x - 1][y - 1]); // NW
        this->nodes[x][y]->addNeighbor(this->nodes[x + 1][y - 1]); // NE

        this->nodes[x - 1][y]->addNeighbor(this->nodes[x][y]);     // W
        this->nodes[x][y - 1]->addNeighbor(this->nodes[x][y]);     // N
        this->nodes[x - 1][y - 1]->addNeighbor(this->nodes[x][y]); // NW
        this->nodes[x + 1][y - 1]->addNeighbor(this->nodes[x][y]); // NE
      }

      // if (x > 0)
      // {
      //   this->nodes[x][y]->addNeighbor(this->nodes[x - 1][y]);
      //   this->nodes[x - 1][y]->addNeighbor(this->nodes[x][y]);
      // };
      // if (y > 0)
      // {
      //   this->nodes[x][y]->addNeighbor(this->nodes[x][y - 1]);
      //   this->nodes[x][y - 1]->addNeighbor(this->nodes[x][y]);
      // };
    };
  };

  // for (int x = 0; x < nodesX; x++)
  // {
  //   for (int y = 0; y < nodesY; y++)
  //   {
  //     Node *tempNode = new Node(x, y);
  //     this->nodes[x][y] = tempNode;
  //     if (x > 0)
  //     {
  //       this->nodes[x][y]->addNeighbor(this->nodes[x - 1][y]);
  //       this->nodes[x - 1][y]->addNeighbor(this->nodes[x][y]);
  //     };
  //     if (y > 0)
  //     {
  //       this->nodes[x][y]->addNeighbor(this->nodes[x][y - 1]);
  //       this->nodes[x][y - 1]->addNeighbor(this->nodes[x][y]);
  //     };
  //   };
  // };
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