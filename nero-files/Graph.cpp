#include <iostream>
#include <time.h>
#include <random>
#include <stdexcept>
#include <cmath>
#include "Node.h"
#include "Graph.h"
#include "Constants.h"
#include "Triangle.h"
#include "Rectangle.h"

Graph::Graph(int xNodes, int yNodes, double cellSize)
{
  this->xNodes = xNodes;
  this->yNodes = yNodes;
  this->cellSize = cellSize;

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
        add_edge(nodes[y][x], nodes[y - 1][x + 1]);
        add_edge(nodes[y][x], nodes[y - 1][x]);

        // nodes[y][x]->add_neighbor(nodes[y - 1][x + 1]); // NE
        // nodes[y][x]->add_neighbor(nodes[y - 1][x]);     // N
      }
      // bottom right node
      else if (y == yNodes - 1 && x == xNodes - 1)
      {
        add_edge(nodes[y][x], nodes[y - 1][x]);
        add_edge(nodes[y][x], nodes[y - 1][x - 1]);
        add_edge(nodes[y][x], nodes[y][x - 1]);

        // nodes[y][x]->add_neighbor(nodes[y - 1][x]);     // N
        // nodes[y][x]->add_neighbor(nodes[y - 1][x - 1]); // NW
        // nodes[y][x]->add_neighbor(nodes[y][x - 1]);     // W
      }
      // top right node
      else if (y == 0 && x == xNodes - 1)
      {
        add_edge(nodes[y][x], nodes[y][x - 1]);

        // nodes[y][x]->add_neighbor(nodes[y][x - 1]); // W
      }
      // top row of nodes
      else if (y == 0)
      {
        add_edge(nodes[y][x], nodes[y][x - 1]);

        // nodes[y][x]->add_neighbor(nodes[y][x - 1]); // W
      }
      // bottom row of nodes
      else if (y == yNodes - 1)
      {
        add_edge(nodes[y][x], nodes[y - 1][x + 1]);
        add_edge(nodes[y][x], nodes[y - 1][x]);
        add_edge(nodes[y][x], nodes[y - 1][x - 1]);
        add_edge(nodes[y][x], nodes[y][x - 1]);

        // nodes[y][x]->add_neighbor(nodes[y - 1][x + 1]); // NE
        // nodes[y][x]->add_neighbor(nodes[y - 1][x]);     // N
        // nodes[y][x]->add_neighbor(nodes[y - 1][x - 1]); // NW
        // nodes[y][x]->add_neighbor(nodes[y][x - 1]);     // W
      }
      // left column of nodes
      else if (x == 0)
      {
        add_edge(nodes[y][x], nodes[y - 1][x + 1]);
        add_edge(nodes[y][x], nodes[y - 1][x]);

        // nodes[y][x]->add_neighbor(nodes[y - 1][x + 1]); // NE
        // nodes[y][x]->add_neighbor(nodes[y - 1][x]);     // N
      }
      // right column of nodes
      else if (x == xNodes - 1)
      {
        add_edge(nodes[y][x], nodes[y - 1][x]);
        add_edge(nodes[y][x], nodes[y - 1][x - 1]);
        add_edge(nodes[y][x], nodes[y][x - 1]);

        // nodes[y][x]->add_neighbor(nodes[y - 1][x]);     // N
        // nodes[y][x]->add_neighbor(nodes[y - 1][x - 1]); // NW
        // nodes[y][x]->add_neighbor(nodes[y][x - 1]);     // W
      }
      // center nodes
      else
      {
        add_edge(nodes[y][x], nodes[y - 1][x + 1]);
        add_edge(nodes[y][x], nodes[y - 1][x]);
        add_edge(nodes[y][x], nodes[y - 1][x - 1]);
        add_edge(nodes[y][x], nodes[y][x - 1]);

        // nodes[y][x]->add_neighbor(nodes[y - 1][x + 1]); // NE
        // nodes[y][x]->add_neighbor(nodes[y - 1][x]);     // N
        // nodes[y][x]->add_neighbor(nodes[y - 1][x - 1]); // NW
        // nodes[y][x]->add_neighbor(nodes[y][x - 1]);     // W
      }
    };
  };

  addForbiddenNodes(this);
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
  Position relativePosition = getPositionRelative(a, b);

  switch (relativePosition)
  {
  case Position::TOP:
  case Position::RIGHT:
  case Position::BOTTOM:
  case Position::LEFT:
  {
    throw std::runtime_error("ERROR 600");
    break;
  }
  case Position::TOP_LEFT:
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
  case Position::TOP_RIGHT:
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
  case Position::BOTTOM_RIGHT:
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
  case Position::BOTTOM_LEFT:
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
}
std::vector<Node *> Graph::getRandomPath()
{
}
std::vector<std::vector<Node *> *> Graph::getPathSnapshots(Node *origin, Node *destination)
{
}
std::vector<std::vector<Node *> *> Graph::getRandomPathSnapshots()
{
}

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

  path.push_back(currentNode);

  return path;
}

// Returns the position of b relative to a
Position Graph::getPositionRelative(Node *a, Node *b)
{
  int ax = a->x;
  int ay = a->y;
  int bx = b->x;
  int by = b->y;

  if (bx == ax && by < ay)
  {
    return Position::TOP;
  }
  else if (bx > ax && by == ay)
  {
    return Position::RIGHT;
  }
  else if (bx == ax && by > ay)
  {
    return Position::BOTTOM;
  }
  else if (bx < ax && by == ay)
  {
    return Position::LEFT;
  }
  else if (bx < ax && by < ay)
  {
    return Position::TOP_LEFT;
  }
  else if (bx > ax && by < ay)
  {
    return Position::TOP_RIGHT;
  }
  else if (bx > ax && by > ay)
  {
    return Position::BOTTOM_RIGHT;
  }
  else if (bx < ax && by > ay)
  {
    return Position::BOTTOM_LEFT;
  }
  else
  {
    throw std::runtime_error("ERROR 004");
  }
}

void Graph::add_edge(Node *a, Node *b)
{
  a->add_neighbor(b);
  b->add_neighbor(a);
}

void Graph::remove_edge(Node *a, Node *b)
{
  a->remove_neighbor(b);
  b->remove_neighbor(a);
}

void Graph::addForbiddenNodes(Graph *graph)
{
  double const ROLLER_SIZE = ZONE_SIZE;

  double const GOAL_START_X = 2 * ZONE_SIZE;
  double const GOAL_START_Y = 0;
  double const GOAL_SIZE_X = 2 * ZONE_SIZE;
  double const GOAL_SIZE_Y = ZONE_SIZE;

  double const BAR_LEFT_START_X = ZONE_SIZE;
  double const BAR_LEFT_START_Y = 2 * ZONE_SIZE;
  double const BAR_LEFT_SIZE_X = 2.375;
  double const BAR_LEFT_SIZE_Y = 2 * ZONE_SIZE;
  double const BAR_RIGHT_START_X = 5 * ZONE_SIZE;
  double const BAR_RIGHT_START_Y = 2 * ZONE_SIZE;
  double const BAR_RIGHT_SIZE_X = 2.375;
  double const BAR_RIGHT_SIZE_Y = 2 * ZONE_SIZE;

  double const BAR_MAIN_START_X = ZONE_SIZE;
  double const BAR_MAIN_START_Y = 3 * ZONE_SIZE;
  double const BAR_MAIN_SIZE_X = 4 * ZONE_SIZE;
  double const BAR_MAIN_SIZE_Y = 2.375;

  std::vector<Triangle> forbiddenZonesTriangles;
  std::vector<Rectangle> forbiddenZonesRectangles;

  forbiddenZonesTriangles.push_back(Triangle(graph->getNode(0, 0),
                                             graph->getNode(0, ROLLER_SIZE),
                                             graph->getNode(ROLLER_SIZE, 0)));
  forbiddenZonesTriangles.push_back(Triangle(graph->getNode(FIELD_SIZE - 1, 0),
                                             graph->getNode(FIELD_SIZE - 1, ROLLER_SIZE),
                                             graph->getNode(FIELD_SIZE - 1 - ROLLER_SIZE, 0)));
  forbiddenZonesTriangles.push_back(Triangle(graph->getNode(0, FIELD_SIZE - 1),
                                             graph->getNode(0, FIELD_SIZE - 1 - ROLLER_SIZE),
                                             graph->getNode(ROLLER_SIZE, FIELD_SIZE - 1)));
  forbiddenZonesTriangles.push_back(Triangle(graph->getNode(FIELD_SIZE - 1, FIELD_SIZE - 1),
                                             graph->getNode(FIELD_SIZE - 1, FIELD_SIZE - 1 - ROLLER_SIZE),
                                             graph->getNode(FIELD_SIZE - 1 - ROLLER_SIZE, FIELD_SIZE - 1)));

  forbiddenZonesRectangles.push_back(Rectangle(graph->getNode(GOAL_START_X, GOAL_START_Y),
                                               graph->getNode(GOAL_START_X + GOAL_SIZE_X, GOAL_START_Y),
                                               graph->getNode(GOAL_START_X, GOAL_START_Y + GOAL_SIZE_Y),
                                               graph->getNode(GOAL_START_X + GOAL_SIZE_X, GOAL_START_Y + GOAL_SIZE_Y)));
  forbiddenZonesRectangles.push_back(Rectangle(graph->getNode(GOAL_START_X, FIELD_SIZE - 1 - GOAL_SIZE_Y),
                                               graph->getNode(GOAL_START_X + GOAL_SIZE_X, FIELD_SIZE - 1 - GOAL_SIZE_Y),
                                               graph->getNode(GOAL_START_X, FIELD_SIZE - 1),
                                               graph->getNode(GOAL_START_X + GOAL_SIZE_X, FIELD_SIZE - 1)));
  forbiddenZonesRectangles.push_back(Rectangle(graph->getNode(BAR_LEFT_START_X, BAR_LEFT_START_Y),
                                               graph->getNode(BAR_LEFT_START_X + BAR_LEFT_SIZE_X, BAR_LEFT_START_Y),
                                               graph->getNode(BAR_LEFT_START_X, BAR_LEFT_START_Y + BAR_LEFT_SIZE_Y),
                                               graph->getNode(BAR_LEFT_START_X + BAR_LEFT_SIZE_X, BAR_LEFT_START_Y + BAR_LEFT_SIZE_Y)));
  forbiddenZonesRectangles.push_back(Rectangle(graph->getNode(BAR_RIGHT_START_X, BAR_RIGHT_START_Y),
                                               graph->getNode(BAR_RIGHT_START_X + BAR_RIGHT_SIZE_X, BAR_RIGHT_START_Y),
                                               graph->getNode(BAR_RIGHT_START_X, BAR_RIGHT_START_Y + BAR_RIGHT_SIZE_Y),
                                               graph->getNode(BAR_RIGHT_START_X + BAR_RIGHT_SIZE_X, BAR_RIGHT_START_Y + BAR_RIGHT_SIZE_Y)));
  forbiddenZonesRectangles.push_back(Rectangle(graph->getNode(BAR_MAIN_START_X, BAR_MAIN_START_Y),
                                               graph->getNode(BAR_MAIN_START_X + BAR_MAIN_SIZE_X, BAR_MAIN_START_Y),
                                               graph->getNode(BAR_MAIN_START_X, BAR_MAIN_START_Y + BAR_MAIN_SIZE_Y),
                                               graph->getNode(BAR_MAIN_START_X + BAR_MAIN_SIZE_X, BAR_MAIN_START_Y + BAR_MAIN_SIZE_Y)));

  for (Triangle triangle : forbiddenZonesTriangles)
  {
    graph->forbidTriangle(triangle.sidePointA, triangle.sidePointB);
  }

  for (Rectangle rectangle : forbiddenZonesRectangles)
  {
    graph->forbidRectangle(rectangle.topLeftPoint, rectangle.topRightPoint, rectangle.bottomLeftPoint);
  }
}
