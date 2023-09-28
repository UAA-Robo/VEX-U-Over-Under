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

Position Graph::get_node_position(int x, int y)
{
  if (y == 0 && x == 0)
  {
    return Position::TOP_LEFT;
  }
  else if (y == 0 && x == X_NODES_COUNT - 1)
  {
    return Position::TOP_RIGHT;
  }
  else if (y == Y_NODES_COUNT - 1 && x == X_NODES_COUNT - 1)
  {
    return Position::BOTTOM_RIGHT;
  }
  else if (y == Y_NODES_COUNT - 1 && x == 0)
  {
    return Position::BOTTOM_LEFT;
  }
  else if (y == 0)
  {
    return Position::TOP;
  }
  else if (x == X_NODES_COUNT - 1)
  {
    return Position::RIGHT;
  }
  else if (y == Y_NODES_COUNT - 1)
  {
    return Position::BOTTOM;
  }
  else if (x == 0)
  {
    return Position::LEFT;
  }
  else
  {
    return Position::CENTER;
  }
}

Graph::Graph(int X_NODES_COUNT, int Y_NODES_COUNT, double NODE_SIZE, GraphType type) : X_NODES_COUNT(X_NODES_COUNT),
                                                                                       Y_NODES_COUNT(Y_NODES_COUNT),
                                                                                       NODE_SIZE(NODE_SIZE),
                                                                                       type(type),
                                                                                       waypoints(new std::set<Node *>),
                                                                                       forbidden_nodes(new std::vector<Node *>),
                                                                                       nodes(new Node **[Y_NODES_COUNT])
{
  for (int x = 0; x < Y_NODES_COUNT; x++)
  {
    nodes[x] = new Node *[X_NODES_COUNT];
  }

  for (int y = 0; y < Y_NODES_COUNT; y++)
  {
    for (int x = 0; x < X_NODES_COUNT; x++)
    {
      nodes[y][x] = new Node(x, y, this);

      switch (get_node_position(x, y))
      {
      case Position::TOP_LEFT:
      {
        break;
      }
      case Position::TOP_RIGHT:
      {
        add_edge(nodes[y][x], nodes[y][x - 1]);
        break;
      }
      case Position::BOTTOM_RIGHT:
      {
        add_edge(nodes[y][x], nodes[y - 1][x]);
        add_edge(nodes[y][x], nodes[y - 1][x - 1]);
        add_edge(nodes[y][x], nodes[y][x - 1]);
        break;
      }
      case Position::BOTTOM_LEFT:
      {
        add_edge(nodes[y][x], nodes[y - 1][x + 1]);
        add_edge(nodes[y][x], nodes[y - 1][x]);
        break;
      }
      case Position::TOP:
      {
        add_edge(nodes[y][x], nodes[y][x - 1]);
        break;
      }
      case Position::RIGHT:
      {
        add_edge(nodes[y][x], nodes[y - 1][x]);
        add_edge(nodes[y][x], nodes[y - 1][x - 1]);
        add_edge(nodes[y][x], nodes[y][x - 1]);
        break;
      }
      case Position::BOTTOM:
      {
        add_edge(nodes[y][x], nodes[y - 1][x + 1]);
        add_edge(nodes[y][x], nodes[y - 1][x]);
        add_edge(nodes[y][x], nodes[y - 1][x - 1]);
        add_edge(nodes[y][x], nodes[y][x - 1]);
        break;
      }
      case Position::LEFT:
      {
        add_edge(nodes[y][x], nodes[y - 1][x + 1]);
        add_edge(nodes[y][x], nodes[y - 1][x]);
        break;
      }
      case Position::CENTER:
      {
        add_edge(nodes[y][x], nodes[y - 1][x + 1]);
        add_edge(nodes[y][x], nodes[y - 1][x]);
        add_edge(nodes[y][x], nodes[y - 1][x - 1]);
        add_edge(nodes[y][x], nodes[y][x - 1]);
        break;
      }
      default:
      {
        std::cout << "\n";
        throw std::runtime_error("ERROR 10237123");
        break;
      }
      }
    };
  };

  add_forbidden_zones(this);
};

Graph::~Graph()
{

  for (int y = 0; y < Y_NODES_COUNT; y++)
  {
    for (int x = 0; x < X_NODES_COUNT; x++)
    {
      delete nodes[y][x];
    }
  }

  for (int x = 0; x < Y_NODES_COUNT; x++)
  {
    delete[] nodes[x];
  }

  delete[] nodes;
}

Node *Graph::get_node(int x, int y)
{
  if (x >= this->X_NODES_COUNT || y >= this->Y_NODES_COUNT || x < 0 || y < 0)
  {
    throw std::runtime_error("ERROR 005");
  }
  return nodes[y][x];
};

std::vector<Node *> *Graph::get_forbidden_nodes()
{
  return forbidden_nodes;
}

void Graph::add_forbidden_node(Node *node)
{
  forbidden_nodes->push_back(node);
}

void Graph::forbid_triangle(Node *a, Node *b)
{
  int bx = b->x;
  int by = b->y;
  int x = a->x;
  int y = a->y;
  int xInitial = x;
  Position relativePosition = get_relative_position(a, b);

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

void Graph::forbid_rectangle(Node *topLeftPoint, Node *topRightPoint, Node *bottomLeftPoint)
{
  for (int y = topLeftPoint->y; y <= bottomLeftPoint->y; y++)
  {
    for (int x = topLeftPoint->x; x <= topRightPoint->x; x++)
    {
      nodes[y][x]->forbid();
    }
  }
}

std::vector<Node *> *Graph::get_path(Node *origin, Node *destination)
{
}

std::vector<Node *> *Graph::get_random_path()
{
}

std::vector<std::vector<Node *> *> *Graph::get_path_snapshots(Node *origin, Node *destination)
{
}

std::vector<std::vector<Node *> *> *Graph::get_random_path_snapshots()
{
}

int Graph::get_edge_cost(Node *a, Node *b)
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

std::vector<Node *> *Graph::reconstruct_path(Node *currentNode, std::map<Node *, Node *> cameFrom)
{
  path_nodes = new std::vector<Node *>;

  while (currentNode != cameFrom[currentNode])
  {
    path_nodes->push_back(currentNode);
    currentNode = cameFrom[currentNode];
  }

  path_nodes->push_back(currentNode);

  return path_nodes;
}

// Returns the position of b relative to a
Position Graph::get_relative_position(Node *a, Node *b)
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

void Graph::add_forbidden_zones(Graph *graph)
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

  forbiddenZonesTriangles.push_back(Triangle(graph->get_node(0, 0),
                                             graph->get_node(0, ROLLER_SIZE),
                                             graph->get_node(ROLLER_SIZE, 0)));
  forbiddenZonesTriangles.push_back(Triangle(graph->get_node(FIELD_SIZE - 1, 0),
                                             graph->get_node(FIELD_SIZE - 1, ROLLER_SIZE),
                                             graph->get_node(FIELD_SIZE - 1 - ROLLER_SIZE, 0)));
  forbiddenZonesTriangles.push_back(Triangle(graph->get_node(0, FIELD_SIZE - 1),
                                             graph->get_node(0, FIELD_SIZE - 1 - ROLLER_SIZE),
                                             graph->get_node(ROLLER_SIZE, FIELD_SIZE - 1)));
  forbiddenZonesTriangles.push_back(Triangle(graph->get_node(FIELD_SIZE - 1, FIELD_SIZE - 1),
                                             graph->get_node(FIELD_SIZE - 1, FIELD_SIZE - 1 - ROLLER_SIZE),
                                             graph->get_node(FIELD_SIZE - 1 - ROLLER_SIZE, FIELD_SIZE - 1)));

  forbiddenZonesRectangles.push_back(Rectangle(graph->get_node(GOAL_START_X, GOAL_START_Y),
                                               graph->get_node(GOAL_START_X + GOAL_SIZE_X, GOAL_START_Y),
                                               graph->get_node(GOAL_START_X, GOAL_START_Y + GOAL_SIZE_Y),
                                               graph->get_node(GOAL_START_X + GOAL_SIZE_X, GOAL_START_Y + GOAL_SIZE_Y)));
  forbiddenZonesRectangles.push_back(Rectangle(graph->get_node(GOAL_START_X, FIELD_SIZE - 1 - GOAL_SIZE_Y),
                                               graph->get_node(GOAL_START_X + GOAL_SIZE_X, FIELD_SIZE - 1 - GOAL_SIZE_Y),
                                               graph->get_node(GOAL_START_X, FIELD_SIZE - 1),
                                               graph->get_node(GOAL_START_X + GOAL_SIZE_X, FIELD_SIZE - 1)));
  forbiddenZonesRectangles.push_back(Rectangle(graph->get_node(BAR_LEFT_START_X, BAR_LEFT_START_Y),
                                               graph->get_node(BAR_LEFT_START_X + BAR_LEFT_SIZE_X, BAR_LEFT_START_Y),
                                               graph->get_node(BAR_LEFT_START_X, BAR_LEFT_START_Y + BAR_LEFT_SIZE_Y),
                                               graph->get_node(BAR_LEFT_START_X + BAR_LEFT_SIZE_X, BAR_LEFT_START_Y + BAR_LEFT_SIZE_Y)));
  forbiddenZonesRectangles.push_back(Rectangle(graph->get_node(BAR_RIGHT_START_X, BAR_RIGHT_START_Y),
                                               graph->get_node(BAR_RIGHT_START_X + BAR_RIGHT_SIZE_X, BAR_RIGHT_START_Y),
                                               graph->get_node(BAR_RIGHT_START_X, BAR_RIGHT_START_Y + BAR_RIGHT_SIZE_Y),
                                               graph->get_node(BAR_RIGHT_START_X + BAR_RIGHT_SIZE_X, BAR_RIGHT_START_Y + BAR_RIGHT_SIZE_Y)));
  forbiddenZonesRectangles.push_back(Rectangle(graph->get_node(BAR_MAIN_START_X, BAR_MAIN_START_Y),
                                               graph->get_node(BAR_MAIN_START_X + BAR_MAIN_SIZE_X, BAR_MAIN_START_Y),
                                               graph->get_node(BAR_MAIN_START_X, BAR_MAIN_START_Y + BAR_MAIN_SIZE_Y),
                                               graph->get_node(BAR_MAIN_START_X + BAR_MAIN_SIZE_X, BAR_MAIN_START_Y + BAR_MAIN_SIZE_Y)));

  for (Triangle triangle : forbiddenZonesTriangles)
  {
    graph->forbid_triangle(triangle.sidePointA, triangle.sidePointB);
  }

  for (Rectangle rectangle : forbiddenZonesRectangles)
  {
    graph->forbid_rectangle(rectangle.topLeftPoint, rectangle.topRightPoint, rectangle.bottomLeftPoint);
  }
}
