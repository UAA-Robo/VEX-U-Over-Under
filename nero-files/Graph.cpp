#include "Graph.h"
#include <iostream>
#include <time.h>
#include <fstream>
#include <ostream>
#include <string>
#include <stdexcept>

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

bool Graph::shouldKeepNode(Node *a)
{
  while (true)
  {
  }
  for (int y = a->y - 1; y >= 0; y--)
  {
    if (this->nodes[y][a->x]->forbidden)
    {
      break;
    }
    if (this->nodes[y][a->x]->waypoint)
    {
    }
  }
}

void Graph::ENLSVG(int xNodes, int yNodes)
{
  this->xNodes = xNodes;
  this->yNodes = yNodes;

  for (int y = 0; y < yNodes; y++)
  {
    for (int x = 0; x < xNodes; x++)
    {
      if (this->isCornerNode(this->nodes[y][x]))
      {
        this->waypoints.push_back(this->nodes[y][x]);
      }
    }
  }

  // when adding a taut visible waypoint as a neight to node a, also add a as a taut visible waypoint to it?
  for (Node *node : this->waypoints)
  {
    node->tautVisibleWaypoints = this->getTautVisibleWaypoints(node);
  }

  for (Node *node : this->waypoints)
  {
  }
}

std::set<Node *> Graph::getTautVisibleWaypoints(Node *node)
{
}

bool Graph::LOS(Node *a, Node *b)
{
}

// break whenever you hit an obstacle
std::vector<Node *> Graph::getVisibleNodes(Node *node)
{
}

// can store this data on the node itself
bool Graph::isCornerNode(Node *node)
{
  int x = node->x;
  int y = node->y;

  bool hasTopNeighbor = y - 1 >= 0;
  bool hasBottomNeighbor = y + 1 <= this->yNodes - 1;
  bool hasLeftNeighbor = x - 1 >= 0;
  bool hasRightNeighbor = x + 1 <= this->xNodes - 1;

  bool hasTopLeftNeighbor = hasTopNeighbor && hasLeftNeighbor;
  bool hasTopRightNeighbor = hasTopNeighbor && hasRightNeighbor;
  bool hasBottomLeftNeighbor = hasBottomNeighbor && hasLeftNeighbor;
  bool hasBottomRightNeighbor = hasBottomNeighbor && hasRightNeighbor;

  bool topNeighborForbidden = hasTopNeighbor && this->nodes[y - 1][x]->forbidden;
  bool rightNeighborForbidden = hasRightNeighbor && this->nodes[y][x + 1]->forbidden;
  bool bottomNeighborForbidden = hasBottomNeighbor && this->nodes[y + 1][x]->forbidden;
  bool leftNeighborForbidden = hasLeftNeighbor && this->nodes[y][x - 1]->forbidden;

  bool topLeftNeighborForbidden = hasTopLeftNeighbor && this->nodes[y - 1][x - 1]->forbidden;
  bool topRightNeighborForbidden = hasTopRightNeighbor && this->nodes[y - 1][x + 1]->forbidden;
  bool bottomRightNeighborForbidden = hasBottomRightNeighbor && this->nodes[y + 1][x + 1]->forbidden;
  bool bottomLeftNeighborForbidden = hasBottomLeftNeighbor && this->nodes[y + 1][x - 1]->forbidden;

  if ((topLeftNeighborForbidden && topRightNeighborForbidden) ||
      (topRightNeighborForbidden && bottomRightNeighborForbidden) ||
      (bottomRightNeighborForbidden && bottomLeftNeighborForbidden) ||
      (bottomLeftNeighborForbidden && topLeftNeighborForbidden))
  {
    throw std::runtime_error("ERROR 200");
  }

  if (topNeighborForbidden ||
      bottomNeighborForbidden ||
      leftNeighborForbidden ||
      rightNeighborForbidden)
  {
    return false;
  }

  if (topLeftNeighborForbidden ||
      topRightNeighborForbidden ||
      bottomRightNeighborForbidden ||
      bottomLeftNeighborForbidden)
  {
    return true;
  };

  return false;
}

int Graph::getEdgeCost(Node *a, Node *b)
{
  // return 10;
  //  if nodes are diagonal neighbors
  if ((b->x == a->x + 1 && b->y == a->y) || (b->x == a->x - 1 && b->y == a->y) || (b->x == a->x && b->y == a->y + 1) || (b->x == a->x && b->y == a->y - 1))
  {
    return 14;
  }
  // if nodes are cardinal neighbors
  else if ((b->x == a->x + 1 && b->y == a->y + 1) || (b->x == a->x + 1 && b->y == a->y - 1) || (b->x == a->x - 1 && b->y == a->y + 1) || (b->x == a->x - 1 && b->y == a->y - 1))
  {
    return 10;
  }
  else
  {
    throw std::runtime_error("ERROR 001");
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

// Octile distance
int Graph::heuristic(Node *currentNode, Node *destination)
{
  int dx = abs(currentNode->x - destination->x);
  int dy = abs(currentNode->y - destination->y);

  return 10 * (dx + dy) + (14 - 2 * 10) * std::min(dx, dy);
}

// Manhattan distance
// int Graph::heuristic(Node *currentNode, Node *destination)
// {
//   return 10 * abs(currentNode->x - destination->x) + 10 * abs(currentNode->y - destination->y);
// }

// Direct distance
// int Graph::heuristic(Node *currentNode, Node *destination)
// {
//   int x1 = currentNode->x;
//   int y1 = currentNode->y;
//   int x2 = destination->x;
//   int y2 = destination->y;

//   return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
// }

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

// A* algorithm
std::vector<Node *> Graph::getPath(Node *origin, Node *destination)
{
  // this->reset();

  std::remove("snapshots.txt");
  std::ofstream fileWriteSnapshot("snapshots.txt");
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

    for (auto node : cameFrom)
    {
      fileWriteSnapshot << std::to_string(node.first->x) << " " << std::to_string(node.first->y) << " " << std::to_string(node.second->x) << " " << std::to_string(node.second->y) << " ";
      //   fileWriteSnapshot << std::to_string(node.first->x) +
      //                            " " + std::to_string(node.first->y) +
      //                            " " + std::to_string(node.second->x) +
      //                            " " + std::to_string(node.second->y) + " ";
    }
    fileWriteSnapshot << "X ";

    frontier.erase(currentNode);

    if (currentNode == destination)
    {
      return reconstructPath(currentNode, cameFrom);
      fileWriteSnapshot.close();
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
  int c = 0;
  do
  {
    originX = randX(rng);
    originY = randY(rng);
    destinationX = randX(rng);
    destinationY = randY(rng);
    c++;
  } while (c < 5 || this->nodes[originY][originX]->forbidden || this->nodes[destinationY][destinationX]->forbidden);

  // this->reset();
  // std::cout << originX << ", " << originY << "\t" << destinationX << ", " << destinationY << "\n";
  return getPath(this->nodes[originY][originX], this->nodes[destinationY][destinationX]);
  // return getPath(this->nodes[50][50], this->nodes[30][30]);
}

void Graph::reset()
{
  for (int y = 0; y < this->yNodes; y++)
  {
    for (int x = 0; x < this->xNodes; x++)
    {
      this->nodes[y][x]->parent = nullptr;
    }
  }
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
  int c = 0;
  do
  {
    originX = randX(rng);
    originY = randY(rng);
    destinationX = randX(rng);
    destinationY = randY(rng);
    c++;
  } while (c < 5 || this->nodes[originY][originX]->forbidden || this->nodes[destinationY][destinationX]->forbidden);

  // this->reset();
  // std::cout << originX << ", " << originY << "\t" << destinationX << ", " << destinationY << "\n";
  return getPathSnapshots(this->nodes[originY][originX], this->nodes[destinationY][destinationX]);
  // return getPath(this->nodes[50][50], this->nodes[30][30]);
}

std::vector<std::vector<Node *> *> Graph::getPathSnapshots(Node *origin, Node *destination)
{
  std::set<Node *> frontier;
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
    Node *currentNode;

    for (Node *node : frontier)
    {
      // if ((fScores.find(node) == fScores.end() && fScores[node] < lowestFScore) || fScores.find(node) == fScores.end())
      /*
      if (fScores.find(node) == fScores.end())
      {
        fScores[node] = 2147483647;
      }
      */
      if (fScores.find(node) == fScores.end())
      {
        std::cout << fScores[node] << "\n";
        std::cout << "AAAAAAAAAAn";
      }
      if (fScores[node] < lowestFScore)
      {
        lowestFScore = fScores[node];
        currentNode = node;
      }
    }

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
      int neighborGScore = gScores[currentNode] + getEdgeCost(currentNode, neighbor);

      // TODO - should this be like or like split into two if clauses?
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
      // if (neighborGScore < gScores[neighbor])
      // if (gScores.find(neighbor) == gScores.end() || neighborGScore < gScores[neighbor])
      // TODO - why or equals?
      // if (neighborGScore < gScores[neighbor])
      // {
      //   cameFrom[neighbor] = currentNode;
      //   gScores[neighbor] = neighborGScore;
      //   fScores[neighbor] = neighborGScore + this->heuristic(neighbor, destination);

      //   if (frontier.find(neighbor) == frontier.end())
      //   {
      //     frontier.insert(neighbor);
      //   }
      // }
    }
  }
  throw std::runtime_error("ERROR 100");
}
