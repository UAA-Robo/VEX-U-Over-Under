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
      // this->nodes[x][y]->addNeighbor(this->nodes[x][y]);

      // this->nodes[x][y]->addNeighbor(this->nodes[x - 1][y]);     // N
      // this->nodes[x][y]->addNeighbor(this->nodes[x - 1][y+ 1]);  // NE
      // this->nodes[x][y]->addNeighbor(this->nodes[x][y + 1]);     // E
      // this->nodes[x][y]->addNeighbor(this->nodes[x + 1][y + 1]); // SE
      // this->nodes[x][y]->addNeighbor(this->nodes[x + 1][y]);     // S
      // this->nodes[x][y]->addNeighbor(this->nodes[x + 1][y - 1]); // SW
      // this->nodes[x][y]->addNeighbor(this->nodes[x][y- 1]);      // W
      // this->nodes[x][y]->addNeighbor(this->nodes[x - 1][y - 1]); // NW

      // this->nodes[x - 1][y]->addNeighbor(this->nodes[x][y]);     // N
      // this->nodes[x - 1][y+ 1]->addNeighbor(this->nodes[x][y]);  // NE
      // this->nodes[x][y + 1]->addNeighbor(this->nodes[x][y]);     // E
      // this->nodes[x + 1][y + 1]->addNeighbor(this->nodes[x][y]); // SE
      // this->nodes[x + 1][y]->addNeighbor(this->nodes[x][y]);     // S
      // this->nodes[x + 1][y - 1]->addNeighbor(this->nodes[x][y]); // SW
      // this->nodes[x][y- 1]->addNeighbor(this->nodes[x][y]);      // W
      // this->nodes[x - 1][y - 1]->addNeighbor(this->nodes[x][y]); // NW

      if (x == 0 && y == 0)
      {
        // top left point
        // no N, NW, NE, W, SW
      }
      else if (x == nodesX && y == 0)
      {
        // bottom left point
        // no S, SW, SE, W, NW

        this->nodes[x][y]->addNeighbor(this->nodes[x - 1][y]);     // N
        this->nodes[x][y]->addNeighbor(this->nodes[x - 1][y + 1]); // NE

        this->nodes[x - 1][y]->addNeighbor(this->nodes[x][y]);     // N
        this->nodes[x - 1][y + 1]->addNeighbor(this->nodes[x][y]); // NE
      }
      else if (x == nodesX && y == nodesY)
      {
        // bottom right point
        // no S, SW, SE, E, NE

        this->nodes[x][y]->addNeighbor(this->nodes[x][y - 1]);     // W
        this->nodes[x][y]->addNeighbor(this->nodes[x - 1][y - 1]); // NW
        this->nodes[x][y]->addNeighbor(this->nodes[x - 1][y]);     // N

        this->nodes[x][y - 1]->addNeighbor(this->nodes[x][y]);     // W
        this->nodes[x - 1][y - 1]->addNeighbor(this->nodes[x][y]); // NW
        this->nodes[x - 1][y]->addNeighbor(this->nodes[x][y]);     // N
      }
      else if (x == 0 && y == nodesY)
      {
        // top right point
        // no N, NW, NE, E, SE

        this->nodes[x][y]->addNeighbor(this->nodes[x][y - 1]); // W

        this->nodes[x][y - 1]->addNeighbor(this->nodes[x][y]); // W
      }
      else if (x == 0)
      {
        // top row
        // no N, NW, NE

        this->nodes[x][y]->addNeighbor(this->nodes[x][y - 1]); // W

        this->nodes[x][y - 1]->addNeighbor(this->nodes[x][y]); // W
      }
      else if (x == nodesX)
      {
        // bottom row
        // no S, SW, SE

        this->nodes[x][y]->addNeighbor(this->nodes[x][y - 1]);     // W
        this->nodes[x][y]->addNeighbor(this->nodes[x - 1][y - 1]); // NW
        this->nodes[x][y]->addNeighbor(this->nodes[x - 1][y]);     // N
        this->nodes[x][y]->addNeighbor(this->nodes[x - 1][y + 1]); // NE

        this->nodes[x][y - 1]->addNeighbor(this->nodes[x][y]);     // W
        this->nodes[x - 1][y - 1]->addNeighbor(this->nodes[x][y]); // NW
        this->nodes[x - 1][y]->addNeighbor(this->nodes[x][y]);     // N
        this->nodes[x - 1][y + 1]->addNeighbor(this->nodes[x][y]); // NE
      }
      else if (y == 0)
      {
        // left column
        // no W, NW, SW

        this->nodes[x][y]->addNeighbor(this->nodes[x - 1][y]);     // N
        this->nodes[x][y]->addNeighbor(this->nodes[x - 1][y + 1]); // NE

        this->nodes[x - 1][y]->addNeighbor(this->nodes[x][y]);     // N
        this->nodes[x - 1][y + 1]->addNeighbor(this->nodes[x][y]); // NE
      }
      else if (y == nodesY)
      {
        // right column
        // no E, NE, SE

        this->nodes[x][y]->addNeighbor(this->nodes[x][y - 1]);     // W
        this->nodes[x][y]->addNeighbor(this->nodes[x - 1][y - 1]); // NW
        this->nodes[x][y]->addNeighbor(this->nodes[x - 1][y]);     // N

        this->nodes[x][y - 1]->addNeighbor(this->nodes[x][y]);     // W
        this->nodes[x - 1][y - 1]->addNeighbor(this->nodes[x][y]); // NW
        this->nodes[x - 1][y]->addNeighbor(this->nodes[x][y]);     // N
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