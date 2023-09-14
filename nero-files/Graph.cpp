#include "Graph.h"
#include <iostream>

int Graph::getEdgeCost(Node *a, Node *b)
{
  // if nodes are diagonal neighbors
  if ((b->x == a->x + 1 && b->y == a->y) || (b->x == a->x - 1 && b->y == a->y) || (b->x == a->x && b->y == a->y + 1) || (b->x == a->x && b->y == a->y - 1))
  {
    return 10;
  }
  // if nodes are orthogonal neighbors
  else if ((b->x == a->x + 1 && b->y == a->y + 1) || (b->x == a->x + 1 && b->y == a->y - 1) || (b->x == a->x - 1 && b->y == a->y + 1) || (b->x == a->x - 1 && b->y == a->y - 1))
  {
    return 14;
  }
  else
  {
    throw std::runtime_error("ERROR 001");
  }
}

std::vector<Node *> Graph::reconstructPath(Node *currentNode)
{
  std::vector<Node *> path;
  path.push_back(currentNode);

  while (currentNode->parent != NULL)
  {
    currentNode = currentNode->parent;
    path.push_back(currentNode);
  }

  return path;
}

int Graph::heuristic(Node *currentNode, Node *destination)
{
  int dx = abs(currentNode->x - destination->x);
  int dy = abs(currentNode->y - destination->y);

  return 10 * (dx + dy) + (14 - 2 * 10) * std::min(dx, dy);
}

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
  this->reset();

  std::set<Node *> frontier;
  std::map<Node *, int> gScores;
  std::map<Node *, int> fScores;

  frontier.insert(origin);
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
      return reconstructPath(currentNode);
    }

    for (Node *neighbor : currentNode->neighbors)
    {
      int neighborGScore = gScores[currentNode] + getEdgeCost(currentNode, neighbor);

      if (gScores.find(neighbor) == gScores.end() || neighborGScore < gScores[neighbor])
      {
        neighbor->parent = currentNode;
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
  std::random_device rd;
  std::mt19937 rng(rd());
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

Node **Graph::getSnapshot()
{
  Node **newGraph;

  for (int y = 0; y < this->yNodes; y++)
  {
    for (int x = 0; x < this->xNodes; x++)
    {
    }
  }
}