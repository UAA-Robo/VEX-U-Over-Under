#include <iostream>
#include <time.h>
#include <random>
#include <stdexcept>
#include <cmath>
#include "Node.h"
#include "Graph.h"

//=============================================================================
//=============================================================================
// PUBLIC METHODS
//=============================================================================
//=============================================================================

Graph::Graph(int xNodes, int yNodes)
{
  this->xNodes = xNodes;
  this->yNodes = yNodes;

  nodes = new Node **[yNodes];

  for (int x = 0; x < yNodes; x++)
  {
    nodes[x] = new Node *[xNodes];
  }

  for (int y = 0; y < yNodes; y++)
  {
    for (int x = 0; x < xNodes; x++)
    {
      nodes[y][x] = new Node(x, y, this);

      // top left node
      if (y == 0 && x == 0)
      {
      }
      // bottom left node
      else if (y == yNodes - 1 && x == 0)
      {
        nodes[y][x]->addNeighbor(nodes[y - 1][x + 1]); // NE
        nodes[y][x]->addNeighbor(nodes[y - 1][x]);     // N
      }
      // bottom right node
      else if (y == yNodes - 1 && x == xNodes - 1)
      {
        nodes[y][x]->addNeighbor(nodes[y - 1][x]);     // N
        nodes[y][x]->addNeighbor(nodes[y - 1][x - 1]); // NW
        nodes[y][x]->addNeighbor(nodes[y][x - 1]);     // W
      }
      // top right node
      else if (y == 0 && x == xNodes - 1)
      {
        nodes[y][x]->addNeighbor(nodes[y][x - 1]); // W
      }
      // top row of nodes
      else if (y == 0)
      {
        nodes[y][x]->addNeighbor(nodes[y][x - 1]); // W
      }
      // bottom row of nodes
      else if (y == yNodes - 1)
      {
        nodes[y][x]->addNeighbor(nodes[y - 1][x + 1]); // NE
        nodes[y][x]->addNeighbor(nodes[y - 1][x]);     // N
        nodes[y][x]->addNeighbor(nodes[y - 1][x - 1]); // NW
        nodes[y][x]->addNeighbor(nodes[y][x - 1]);     // W
      }
      // left column of nodes
      else if (x == 0)
      {
        nodes[y][x]->addNeighbor(nodes[y - 1][x + 1]); // NE
        nodes[y][x]->addNeighbor(nodes[y - 1][x]);     // N
      }
      // right column of nodes
      else if (x == xNodes - 1)
      {
        nodes[y][x]->addNeighbor(nodes[y - 1][x]);     // N
        nodes[y][x]->addNeighbor(nodes[y - 1][x - 1]); // NW
        nodes[y][x]->addNeighbor(nodes[y][x - 1]);     // W
      }
      // center nodes
      else
      {
        nodes[y][x]->addNeighbor(nodes[y - 1][x + 1]); // NE
        nodes[y][x]->addNeighbor(nodes[y - 1][x]);     // N
        nodes[y][x]->addNeighbor(nodes[y - 1][x - 1]); // NW
        nodes[y][x]->addNeighbor(nodes[y][x - 1]);     // W
      }
    };
  };
};

Graph::~Graph()
{

  for (int y = 0; y < yNodes; y++)
  {
    for (int x = 0; x < xNodes; x++)
    {
      delete nodes[y][x];
    }
  }

  for (int x = 0; x < yNodes; x++)
  {
    delete[] nodes[x];
  }

  delete[] nodes;
}

Node *Graph::getNode(int x, int y)
{
  if (x >= this->xNodes || y >= this->yNodes || x < 0 || y < 0)
  {
    throw std::runtime_error("ERROR 005");
  }
  return nodes[y][x];
};

std::vector<Node *> Graph::getForbiddenNodes()
{
  // std::vector<Node *> forbiddenNodes;
  // Node *node;

  // for (int y = 0; y < this->yNodes; y++)
  // {
  //   for (int x = 0; x < this->xNodes; x++)
  //   {
  //     node = nodes[y][x];
  //     if (node->forbidden)
  //     {
  //       forbiddenNodes.push_back(node);
  //     }
  //   }
  // }

  return forbiddenNodes;
}

void Graph::addForbiddenNode(Node *node)
{
  forbiddenNodes.push_back(node);
}

void Graph::forbidTriangle(Node *a, Node *b)
{
  int bx = b->x;
  int by = b->y;
  int x = a->x;
  int y = a->y;
  int xInitial = x;
  POSITION relativePosition = getPositionRelative(a, b);

  switch (relativePosition)
  {
  case B_TOP_A:
  case B_RIGHT_A:
  case B_BOTTOM_A:
  case B_LEFT_A:
  {
    throw std::runtime_error("ERROR 600");
    break;
  }
  case B_TOPLEFT_A:
  {
    while (x >= bx && y >= by)
    {
      for (int xx = xInitial; xx >= x; xx--)
      {
        // this->getNode(xx, y)->forbid();
        nodes[y][xx]->forbid();
      }

      x--;
      y--;
    }
    break;
  }
  case B_TOPRIGHT_A:
  {
    while (x <= bx && y >= by)
    {
      for (int xx = xInitial; xx <= x; xx++)
      {
        // this->getNode(xx, y)->forbid();
        nodes[y][xx]->forbid();
      }

      x++;
      y--;
    }
    break;
  }
  case B_BOTTOMRIGHT_A:
  {
    while (x <= bx && y <= by)
    {
      for (int xx = xInitial; xx <= x; xx++)
      {
        // this->getNode(xx, y)->forbid();
        nodes[y][xx]->forbid();
      }

      x++;
      y++;
    }
    break;
  }
  case B_BOTTOMLEFT_A:
  {
    while (x >= bx && y <= by)
    {
      for (int xx = xInitial; xx >= x; xx--)
      {
        // this->getNode(xx, y)->forbid();
        nodes[y][xx]->forbid();
      }

      x--;
      y++;
    }
    break;
  }
  default:
    throw std::runtime_error("ERROR 500");
    break;
  }
}

void Graph::forbidRectangle(Node *topLeftPoint, Node *topRightPoint, Node *bottomLeftPoint)
{
  for (int y = topLeftPoint->y; y <= bottomLeftPoint->y; y++)
  {
    for (int x = topLeftPoint->x; x <= topRightPoint->x; x++)
    {
      // this->getNode(x, y)->forbid();
      nodes[y][x]->forbid();
    }
  }
}

std::vector<Node *> Graph::getPath(Node *origin, Node *destination)
{
  std::cout << "GRAPH FUNCTION CALLED\n";
}
std::vector<Node *> Graph::getRandomPath()
{
  std::cout << "GRAPH FUNCTION CALLED\n";
}
std::vector<std::vector<Node *> *> Graph::getPathSnapshots(Node *origin, Node *destination)
{
  std::cout << "GRAPH FUNCTION CALLED\n";
}
std::vector<std::vector<Node *> *> Graph::getRandomPathSnapshots()
{
  std::cout << "GRAPH FUNCTION CALLED\n";
}

//=============================================================================
//=============================================================================
// PRIVATE METHODS
//=============================================================================
//=============================================================================

int Graph::getEdgeCost(Node *a, Node *b)
{
  // return 10;
  // if nodes are cardinal neighbors
  if ((b->x == a->x + 1 && b->y == a->y) || (b->x == a->x - 1 && b->y == a->y) || (b->x == a->x && b->y == a->y + 1) || (b->x == a->x && b->y == a->y - 1))
  {
    return 10;
  }
  // if nodes are diagonal neighbors
  else if ((b->x == a->x + 1 && b->y == a->y + 1) || (b->x == a->x + 1 && b->y == a->y - 1) || (b->x == a->x - 1 && b->y == a->y + 1) || (b->x == a->x - 1 && b->y == a->y - 1))
  {
    return 14;
  }
  else
  {
    // Euclidean distance
    int x1 = a->x;
    int y1 = a->y;
    int x2 = b->x;
    int y2 = b->y;
    // return std::sqrt(std::pow(x2 - x1, 2) * 10 + std::pow(y2 - y1, 2) * 10);
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));

    // throw std::runtime_error("ERROR 001");
  }
}

std::vector<Node *> Graph::reconstructPath(Node *currentNode, std::map<Node *, Node *> cameFrom)
{
  std::vector<Node *> path;

  while (currentNode != cameFrom[currentNode])
  {
    path.push_back(currentNode);
    currentNode = cameFrom[currentNode];
  }

  return path;
}

// Returns the position of b relative to a
POSITION Graph::getPositionRelative(Node *a, Node *b)
{
  int ax = a->x;
  int ay = a->y;
  int bx = b->x;
  int by = b->y;

  if (bx == ax && by < ay)
  {
    return B_TOP_A;
  }
  else if (bx > ax && by == ay)
  {
    return B_RIGHT_A;
  }
  else if (bx == ax && by > ay)
  {
    return B_BOTTOM_A;
  }
  else if (bx < ax && by == ay)
  {
    return B_LEFT_A;
  }
  else if (bx < ax && by < ay)
  {
    return B_TOPLEFT_A;
  }
  else if (bx > ax && by < ay)
  {
    return B_TOPRIGHT_A;
  }
  else if (bx > ax && by > ay)
  {
    return B_BOTTOMRIGHT_A;
  }
  else if (bx < ax && by > ay)
  {
    return B_BOTTOMLEFT_A;
  }
  else
  {
    a->println();
    b->println();
    throw std::runtime_error("ERROR 004");
  }
}
