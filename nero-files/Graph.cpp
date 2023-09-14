#include "Graph.h"
#include <iostream>

// Graph::Graph(int xNodes, int yNodes)
// {
//   this->xNodes = xNodes;
//   this->yNodes = yNodes;
//   this->nodes = new Node **[yNodes];
//   for (int x = 0; x < yNodes; x++)
//   {
//     this->nodes[x] = new Node *[xNodes];
//   }

//   for (int y = 0; y < yNodes; y++)
//   {
//     for (int x = 0; x < xNodes; x++)
//     {
//       Node *tempNode = new Node(x, y);
//       this->nodes[y][x] = tempNode;

//       if (y == 0 && x == 0)
//       {
//       }
//       else if (y == yNodes - 1 && x == 0)
//       {

//         this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x + 1]); // NE
//         this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);     // N

//         this->nodes[y - 1][x + 1]->addNeighbor(this->nodes[y][x]); // NE
//         this->nodes[y - 1][x]->addNeighbor(this->nodes[y][x]);     // N
//       }
//       else if (y == yNodes - 1 && x == xNodes - 1)
//       {

//         this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);     // N
//         this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x - 1]); // NW
//         this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]);     // W

//         this->nodes[y - 1][x]->addNeighbor(this->nodes[y][x]);     // N
//         this->nodes[y - 1][x - 1]->addNeighbor(this->nodes[y][x]); // NW
//         this->nodes[y][x - 1]->addNeighbor(this->nodes[y][x]);     // W
//       }
//       else if (y == 0 && x == xNodes - 1)
//       {

//         this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]); // W

//         this->nodes[y][x - 1]->addNeighbor(this->nodes[y][x]); // W
//       }
//       else if (y == 0)
//       {

//         this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]); // W

//         this->nodes[y][x - 1]->addNeighbor(this->nodes[y][x]); // W
//       }
//       else if (y == yNodes - 1)
//       {

//         this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x + 1]); // NE
//         this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);     // N
//         this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x - 1]); // NW
//         this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]);     // W

//         this->nodes[y - 1][x + 1]->addNeighbor(this->nodes[y][x]); // NE
//         this->nodes[y - 1][x]->addNeighbor(this->nodes[y][x]);     // N
//         this->nodes[y - 1][x - 1]->addNeighbor(this->nodes[y][x]); // NW
//         this->nodes[y][x - 1]->addNeighbor(this->nodes[y][x]);     // W
//       }
//       else if (x == 0)
//       {

//         this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x + 1]); // NE
//         this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);     // N

//         this->nodes[y - 1][x + 1]->addNeighbor(this->nodes[y][x]); // NE
//         this->nodes[y - 1][x]->addNeighbor(this->nodes[y][x]);     // N
//       }
//       else if (x == xNodes - 1)
//       {

//         this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);     // N
//         this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x - 1]); // NW
//         this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]);     // W

//         this->nodes[y - 1][x]->addNeighbor(this->nodes[y][x]);     // N
//         this->nodes[y - 1][x - 1]->addNeighbor(this->nodes[y][x]); // NW
//         this->nodes[y][x - 1]->addNeighbor(this->nodes[y][x]);     // W
//       }
//       else
//       {
//         this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x + 1]); // NE
//         this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);     // N
//         this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x - 1]); // NW
//         this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]);     // W

//         this->nodes[y - 1][x + 1]->addNeighbor(this->nodes[y][x]); // NE
//         this->nodes[y - 1][x]->addNeighbor(this->nodes[y][x]);     // N
//         this->nodes[y - 1][x - 1]->addNeighbor(this->nodes[y][x]); // NW
//         this->nodes[y][x - 1]->addNeighbor(this->nodes[y][x]);     // W
//       }
//     };
//   };
// };

Graph::Graph(int xNodes, int yNodes)
{
  this->xNodes = xNodes;
  this->yNodes = yNodes;

  this->nodes = new Node **[yNodes];

  for (int x = 0; x < yNodes; x++)
  {
    this->nodes[x] = new Node *[xNodes];
  }

  for (int y = 0; y < yNodes; y++)
  {
    for (int x = 0; x < xNodes; x++)
    {
      this->nodes[y][x] = new Node(x, y);

      // top left node
      if (y == 0 && x == 0)
      {
      }
      // bottom left node
      else if (y == yNodes - 1 && x == 0)
      {

        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x + 1]); // NE
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);     // N

        // this->nodes[y - 1][x + 1]->addNeighbor(this->nodes[y][x]); // NE
        // this->nodes[y - 1][x]->addNeighbor(this->nodes[y][x]);     // N
      }
      // bottom right node
      else if (y == yNodes - 1 && x == xNodes - 1)
      {

        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);     // N
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x - 1]); // NW
        this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]);     // W

        // this->nodes[y - 1][x]->addNeighbor(this->nodes[y][x]);     // N
        // this->nodes[y - 1][x - 1]->addNeighbor(this->nodes[y][x]); // NW
        // this->nodes[y][x - 1]->addNeighbor(this->nodes[y][x]);     // W
      }
      // top right node
      else if (y == 0 && x == xNodes - 1)
      {

        this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]); // W

        // this->nodes[y][x - 1]->addNeighbor(this->nodes[y][x]); // W
      }
      // top row of nodes
      else if (y == 0)
      {

        this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]); // W

        // this->nodes[y][x - 1]->addNeighbor(this->nodes[y][x]); // W
      }
      // bottom row of nodes
      else if (y == yNodes - 1)
      {

        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x + 1]); // NE
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);     // N
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x - 1]); // NW
        this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]);     // W

        // this->nodes[y - 1][x + 1]->addNeighbor(this->nodes[y][x]); // NE
        // this->nodes[y - 1][x]->addNeighbor(this->nodes[y][x]);     // N
        // this->nodes[y - 1][x - 1]->addNeighbor(this->nodes[y][x]); // NW
        // this->nodes[y][x - 1]->addNeighbor(this->nodes[y][x]);     // W
      }
      // left column of nodes
      else if (x == 0)
      {

        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x + 1]); // NE
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);     // N

        // this->nodes[y - 1][x + 1]->addNeighbor(this->nodes[y][x]); // NE
        // this->nodes[y - 1][x]->addNeighbor(this->nodes[y][x]);     // N
      }
      // right column of nodes
      else if (x == xNodes - 1)
      {

        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);     // N
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x - 1]); // NW
        this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]);     // W

        // this->nodes[y - 1][x]->addNeighbor(this->nodes[y][x]);     // N
        // this->nodes[y - 1][x - 1]->addNeighbor(this->nodes[y][x]); // NW
        // this->nodes[y][x - 1]->addNeighbor(this->nodes[y][x]);     // W
      }
      // center nodes
      else
      {
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x + 1]); // NE
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);     // N
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x - 1]); // NW
        this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]);     // W

        // this->nodes[y - 1][x + 1]->addNeighbor(this->nodes[y][x]); // NE
        // this->nodes[y - 1][x]->addNeighbor(this->nodes[y][x]);     // N
        // this->nodes[y - 1][x - 1]->addNeighbor(this->nodes[y][x]); // NW
        // this->nodes[y][x - 1]->addNeighbor(this->nodes[y][x]);     // W
      }
    };
  };
};

Node *Graph::getNode(int x, int y)
{
  if (x >= this->xNodes || y >= this->yNodes || x < 0 || y < 0)
  {
    throw std::runtime_error("ERROR 005");
  }
  return this->nodes[y][x];
};

std::vector<Node *> Graph::getForbiddenNodes()
{
  std::vector<Node *> forbiddenNodes;
  Node *node;

  for (int y = 0; y < this->yNodes; y++)
  {
    for (int x = 0; x < this->xNodes; x++)
    {
      node = this->nodes[y][x];
      if (node->forbidden)
      {
        forbiddenNodes.push_back(node);
      }
    }
  }

  return forbiddenNodes;
}