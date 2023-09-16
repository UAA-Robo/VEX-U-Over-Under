#include "Graph.h"
#include <iostream>
#include <time.h>
#include <fstream>
#include <ostream>
#include <string>
#include <stdexcept>

//=============================================================================
//=============================================================================
// PUBLIC METHODS
//=============================================================================
//=============================================================================

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
      }
      // bottom right node
      else if (y == yNodes - 1 && x == xNodes - 1)
      {
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);     // N
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x - 1]); // NW
        this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]);     // W
      }
      // top right node
      else if (y == 0 && x == xNodes - 1)
      {
        this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]); // W
      }
      // top row of nodes
      else if (y == 0)
      {
        this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]); // W
      }
      // bottom row of nodes
      else if (y == yNodes - 1)
      {
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x + 1]); // NE
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);     // N
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x - 1]); // NW
        this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]);     // W
      }
      // left column of nodes
      else if (x == 0)
      {
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x + 1]); // NE
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);     // N
      }
      // right column of nodes
      else if (x == xNodes - 1)
      {
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);     // N
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x - 1]); // NW
        this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]);     // W
      }
      // center nodes
      else
      {
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x + 1]); // NE
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x]);     // N
        this->nodes[y][x]->addNeighbor(this->nodes[y - 1][x - 1]); // NW
        this->nodes[y][x]->addNeighbor(this->nodes[y][x - 1]);     // W
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
      delete this->nodes[y][x];
    }
  }

  for (int x = 0; x < yNodes; x++)
  {
    delete[] this->nodes[x];
  }

  delete[] this->nodes;
}

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

std::vector<Node *> Graph::getPath(Node *origin, Node *destination)
{
  std::set<Node *> frontier;
  std::map<Node *, Node *> cameFrom;
  std::map<Node *, int> gScores;
  std::map<Node *, int> fScores;

  frontier.insert(origin);
  cameFrom[origin] = origin;
  gScores[origin] = 0;
  fScores[origin] = this->heuristic(origin, destination);

  while (frontier.size() > 0)
  {
    int lowestFScore = 2147483647;
    Node *currentNode;

    for (Node *node : frontier)
    {
      if (fScores.find(node) == fScores.end() || fScores[node] < lowestFScore)
      {
        lowestFScore = fScores[node];
        currentNode = node;
      }
    }

    frontier.erase(currentNode);

    if (currentNode == destination)
    {
      return reconstructPath(currentNode, cameFrom);
    }

    for (Node *neighbor : currentNode->neighbors)
    {
      int neighborGScore = gScores[currentNode] + getEdgeCost(currentNode, neighbor);

      if (gScores.find(neighbor) == gScores.end() || neighborGScore < gScores[neighbor])
      {
        cameFrom[neighbor] = currentNode;
        gScores[neighbor] = neighborGScore;
        fScores[neighbor] = neighborGScore + this->heuristic(neighbor, destination);

        if (frontier.find(neighbor) == frontier.end())
        {
          frontier.insert(neighbor);
        }
      }
    }
  }
  throw std::runtime_error("ERROR 002");
}

std::vector<Node *> Graph::getRandomPath()
{
  srand(time(0));

  // std::random_device rd;
  std::mt19937 rng(rand());
  std::uniform_int_distribution<int> randX(0, this->xNodes - 1);
  std::uniform_int_distribution<int> randY(0, this->yNodes - 1);

  int originX;
  int originY;
  int destinationX;
  int destinationY;

  do
  {
    originX = randX(rng);
    originY = randY(rng);
    destinationX = randX(rng);
    destinationY = randY(rng);
  } while (this->nodes[originY][originX]->forbidden || this->nodes[destinationY][destinationX]->forbidden);

  return getPath(this->nodes[originY][originX], this->nodes[destinationY][destinationX]);
}

std::vector<std::vector<Node *> *> Graph::getPathSnapshots(Node *origin, Node *destination)
{
  std::set<Node *> frontier;
  std::set<Node *> closed;
  std::map<Node *, Node *> cameFrom;
  std::map<Node *, int> gScores;
  std::map<Node *, int> fScores;
  std::vector<std::vector<Node *> *> snapshots;

  frontier.insert(origin);
  cameFrom[origin] = origin;
  gScores[origin] = 0;
  fScores[origin] = this->heuristic(origin, destination);

  while (frontier.size() > 0)
  {
    snapshots.push_back(new std::vector<Node *>);
    int lowestFScore = 2147483647;
    int lowestHScore = 2147483647;
    int lowestGScore = 2147483647;
    int highestGScore = 0;
    Node *currentNode;

    for (Node *node : frontier)
    {
      // if (node == destination)
      // {
      //   currentNode = destination;
      //   break;
      // }

      // if (fScores[node] < lowestFScore)
      // {
      //   lowestFScore = fScores[node];
      //   currentNode = node;
      // }
      // else if (fScores[node] == lowestFScore && lowestFScore != 2147483647)
      // {
      //   if (gScores[node] > highestGScore)
      //   {
      //     highestGScore = gScores[node];
      //     currentNode = node;
      //   }
      // }

      // if (fScores[node] < lowestFScore)
      // {
      //   lowestFScore = fScores[node];
      //   currentNode = node;
      //   // highestGScore = 0;
      // }
      // else if (fScores[node] == lowestFScore && this->heuristic(node, destination) < lowestHScore)
      // {
      //   lowestHScore = this->heuristic(node, destination);
      //   currentNode = node;
      // }

      // else if (fScores[node] == lowestFScore && gScores[node] > highestGScore)
      // {
      //   highestGScore = gScores[node];
      //   currentNode = node;
      // }

      // PREFER DIAGONAL
      // <<

      // if (heuristic(node, destination) < lowestHScore)
      // {
      //   lowestHScore = heuristic(node, destination);
      //   currentNode = node;
      //   lowestGScore = gScores[node];
      //   // lowestFScore = fScores[node];
      // }
      // else if (heuristic(node, destination) == lowestHScore && gScores[node] < lowestGScore)
      // {
      //   lowestHScore = heuristic(node, destination);
      //   currentNode = node;
      //   lowestGScore = gScores[node];
      //   // lowestFScore = fScores[node];
      // }

      // if (fScores[node] < lowestFScore)
      // {
      //   lowestFScore = fScores[node];
      //   currentNode = node;
      // }
      // else if (fScores[node] == lowestFScore && heuristic(node, destination) < heuristic(currentNode, destination))
      // {
      //   currentNode = node;
      // }

      if (heuristic(node, destination) < lowestHScore)
      {
        lowestHScore = heuristic(node, destination);
        currentNode = node;
      }
      else if (heuristic(node, destination) == lowestHScore && gScores[node] < lowestGScore)
      {
        lowestGScore = gScores[node];
        currentNode = node;
      }
    }

    // std::cout << "\n\nSNAPSHOT #" << snapshots.size() << "\n";
    // for (int y = 0; y < yNodes; y++)
    // {
    //   for (int x = 0; x < xNodes; x++)
    //   {
    //     Node *node = nodes[y][x];
    //     std::cout << "(" << node->x << ", " << node->y << ") "
    //               << "F-" << fScores[node] << " H-" << this->heuristic(node, destination) << "   ";
    //   }

    //   std::cout << "\n";
    // }

    Node *temp = currentNode;
    snapshots.back()->push_back(temp);
    while (temp != cameFrom[temp])
    {
      snapshots.back()->push_back(cameFrom[temp]);
      temp = cameFrom[temp];
    }

    frontier.erase(currentNode);

    if (currentNode == destination)
    {
      // return reconstructPath(currentNode, cameFrom);
      return snapshots;
    }

    for (Node *neighbor : currentNode->neighbors)
    {
      if (closed.find(neighbor) == closed.end())
      {
        int neighborGScore = gScores[currentNode] + getEdgeCost(currentNode, neighbor);

        if (gScores.find(neighbor) == gScores.end() || neighborGScore < gScores[neighbor])
        {
          cameFrom[neighbor] = currentNode;
          gScores[neighbor] = neighborGScore;
          fScores[neighbor] = neighborGScore + this->heuristic(neighbor, destination);

          if (closed.find(neighbor) == closed.end() && frontier.find(neighbor) == frontier.end())
          {
            frontier.insert(neighbor);
          }
        }
      }
    }

    // Remove this?
    closed.insert(currentNode);
  }
  throw std::runtime_error("ERROR 100");
}

std::vector<std::vector<Node *> *> Graph::getRandomPathSnapshots()
{
  srand(time(0));

  // std::random_device rd;
  std::mt19937 rng(rand());
  std::uniform_int_distribution<int> randX(0, this->xNodes - 1);
  std::uniform_int_distribution<int> randY(0, this->yNodes - 1);

  int originX;
  int originY;
  int destinationX;
  int destinationY;

  do
  {
    originX = randX(rng);
    originY = randY(rng);
    destinationX = randX(rng);
    destinationY = randY(rng);
  } while (this->nodes[originY][originX]->forbidden || this->nodes[destinationY][destinationX]->forbidden);

  return getPathSnapshots(this->nodes[originY][originX], this->nodes[destinationY][destinationX]);
}

int Graph::getPositionRelative(Node *a, Node *b)
{
  int ax = a->x;
  int ay = a->y;
  int bx = b->x;
  int by = b->y;

  // b is top left of a
  if (bx < ax && by < ay)
  {
    return 0;
  }
  // b is top right of a
  else if (bx > ax && by < ay)
  {
    return 1;
  }
  // b is bottom left of a
  else if (bx < ax && by > ay)
  {
    return 2;
  }
  // b is bottom right of a
  else if (bx > ax && by > ay)
  {
    return 3;
  }
  else
  {
    throw std::runtime_error("ERROR 004");
  }
}

void Graph::forbidTriangle(Node *a, Node *b)
{

  int bx = b->x;
  int by = b->y;
  int x = a->x;
  int y = a->y;
  int xInitial = x;

  // b is top left of a
  if (getPositionRelative(a, b) == 0)
  {
    while (x >= bx && y >= by)
    {
      for (int xx = xInitial; xx >= x; xx--)
      {
        // this->getNode(xx, y)->forbid();
        this->nodes[y][xx]->forbid();
      }

      x--;
      y--;
    }
  }
  // b is top right of a
  else if (getPositionRelative(a, b) == 1)
  {
    while (x <= bx && y >= by)
    {
      for (int xx = xInitial; xx <= x; xx++)
      {
        // this->getNode(xx, y)->forbid();
        this->nodes[y][xx]->forbid();
      }

      x++;
      y--;
    }
  }
  // b is bottom left of a
  else if (getPositionRelative(a, b) == 2)
  {
    while (x >= bx && y <= by)
    {
      for (int xx = xInitial; xx >= x; xx--)
      {
        // this->getNode(xx, y)->forbid();
        this->nodes[y][xx]->forbid();
      }

      x--;
      y++;
    }
  }
  // b is bottom right of a
  else if (getPositionRelative(a, b) == 3)
  {
    while (x <= bx && y <= by)
    {
      for (int xx = xInitial; xx <= x; xx++)
      {
        // this->getNode(xx, y)->forbid();
        this->nodes[y][xx]->forbid();
      }

      x++;
      y++;
    }
  }
}

void Graph::forbidRectangle(Node *topLeftPoint, Node *topRightPoint, Node *bottomLeftPoint)
{
  for (int y = topLeftPoint->y; y <= bottomLeftPoint->y; y++)
  {
    for (int x = topLeftPoint->x; x <= topRightPoint->x; x++)
    {
      // this->getNode(x, y)->forbid();
      this->nodes[y][x]->forbid();
    }
  }
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
    throw std::runtime_error("ERROR 001");
  }
}

// Octile distance
int Graph::heuristic(Node *currentNode, Node *destination)
{
  int dx = abs(currentNode->x - destination->x);
  int dy = abs(currentNode->y - destination->y);

  if (dx > dy)
  {
    return 14 * dy + 10 * (dx - dy);
  }
  else
  {
    return 14 * dx + 10 * (dy - dx);
  }

  // return 10 * (dx + dy) + (14 - 2 * 10) * std::min(dx, dy);
}

// Manhattan distance
// int Graph::heuristic(Node *currentNode, Node *destination)
// {
//   int dx = abs(currentNode->x - destination->x);
//   int dy = abs(currentNode->y - destination->y);

//   return 10 * (dx + dy);
//   // return 10 * abs(currentNode->x - destination->x) + 10 * abs(currentNode->y - destination->y);
// }

// Direct distance
// int Graph::heuristic(Node *currentNode, Node *destination)
// {
//   int x1 = currentNode->x;
//   int y1 = currentNode->y;
//   int x2 = destination->x;
//   int y2 = destination->y;
//   // return std::sqrt(std::pow(x2 - x1, 2) * 10 + std::pow(y2 - y1, 2) * 10);
//   return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
// }

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
