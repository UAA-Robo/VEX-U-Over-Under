#include <map>
#include <vector>
#include <climits>
#include <cmath>
#include <iostream>
#include <utility>
#include <tuple>
#include <random>
#include <chrono>
#include <fstream>
#include <ostream>
#include <string>

#include "Node.h"
#include "Graph.h"
#include "Triangle.h"
#include "Rectangle.h"

std::ostream &operator<<(std::ostream &os, Node *const &m)
{
  return os << "(" << m->x << ", " << m->y << ")";
}

bool hasLOS(Graph graph, Node *origin, Node *destination)
{
  double slope_y = ((destination->y - origin->y) / (destination->x - origin->x)) / 10;
  double slope_x = ((destination->x - origin->x) / (destination->y - origin->y)) / 10;
  double x = origin->x;
  double y = origin->y;

  do
  {
    if (graph.getNode(x, y)->forbidden)
    {
      return false;
    }

    x += slope_x;
    y += slope_y;
  } while ((x <= destination->x) && (y <= destination->y));

  return true;
}

int const BUFFER_NODES = 5;

double const NODE_SIZE = 1;

// int const X_NODES = 50;
// int const Y_NODES = 50;

double const FIELD_SIZE = 138.73;
double const ZONE_SIZE = FIELD_SIZE / 6;

int const X_NODES = FIELD_SIZE / NODE_SIZE;
int const Y_NODES = FIELD_SIZE / NODE_SIZE;

int getDistanceDirect(Node *a, Node *b)
{
  int dx = abs(b->x - a->x);
  int dy = abs(b->y - a->y);
  return 10 * sqrt(dx * dx + dy * dy);
}

int h(Node *currentNode, Node *destination)
{
  int dx = abs(currentNode->x - destination->x);
  int dy = abs(currentNode->y - destination->y);
  return 10 * (dx + dy) + (14 - 2 * 10) * std::min(dx, dy);
}

std::vector<Node *> reconstructPath(Node *currentNode)
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

int getEdgeCost(Node *a, Node *b)
{
  if ((b->x == a->x + 1 && b->y == a->y) || (b->x == a->x - 1 && b->y == a->y) || (b->x == a->x && b->y == a->y + 1) || (b->x == a->x && b->y == a->y - 1))
  {
    return 10;
  }
  else if ((b->x == a->x + 1 && b->y == a->y + 1) || (b->x == a->x + 1 && b->y == a->y - 1) || (b->x == a->x - 1 && b->y == a->y + 1) || (b->x == a->x - 1 && b->y == a->y - 1))
  {
    return 14;
  }
  else
  {
    throw "Error 010";
  }
}

std::vector<Node *> AStar(Graph graph, Node *origin, Node *destination, int (*h)(Node *, Node *))
{
  std::set<Node *> frontier;
  std::map<Node *, int> gScores;
  std::map<Node *, int> fScores;

  frontier.insert(origin);
  gScores[origin] = 0;
  fScores[origin] = h(origin, destination);

  while (frontier.size() > 0)
  {
    int lowestFScore = INT_MAX;
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
        fScores[neighbor] = neighborGScore + h(neighbor, destination);
        if (frontier.find(neighbor) == frontier.end())
        {
          frontier.insert(neighbor);
        }
      }
    }
  }
  throw "Error 020";
}

std::vector<Node *> ThetaStar(Graph graph, Node *origin, Node *destination, int (*h)(Node *, Node *))
{
  std::set<Node *> frontier;
  std::set<Node *> explored;
  std::map<Node *, int> gScores;
  std::map<Node *, int> fScores;

  frontier.insert(origin);
  gScores[origin] = 0;
  fScores[origin] = h(origin, destination);

  while (frontier.size() > 0)
  {
    int lowestFScore = INT_MAX;
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
        fScores[neighbor] = neighborGScore + h(neighbor, destination);
        if (frontier.find(neighbor) == frontier.end())
        {
          frontier.insert(neighbor);
        }
      }
    }
  }
  throw "Error 020";
}

void forbidZone(Graph graph, std::vector<std::tuple<int, int>> zonePoints)
{
  if (zonePoints.size() == 3)
  {
  }
  else if (zonePoints.size() == 4)
  {
  }
  else
  {
    throw "Error 030";
  }
}

bool operator==(const Triangle &triangleOne, const Triangle &triangleTwo)
{
  if (std::get<0>(triangleOne.rightAnglePoint) == std::get<0>(triangleTwo.rightAnglePoint) &&
      std::get<1>(triangleOne.rightAnglePoint) == std::get<1>(triangleTwo.rightAnglePoint) &&
      std::get<0>(triangleOne.sidePointA) == std::get<0>(triangleTwo.sidePointA) &&
      std::get<1>(triangleOne.sidePointA) == std::get<1>(triangleTwo.sidePointA) &&
      std::get<0>(triangleOne.sidePointB) == std::get<0>(triangleTwo.sidePointB) &&
      std::get<1>(triangleOne.sidePointB) == std::get<1>(triangleTwo.sidePointB))
  {
    return true;
  }
  return false;
}

bool operator==(const Rectangle &rectangleOne, const Rectangle &rectangleTwo)
{
  if (std::get<0>(rectangleOne.topLeftPoint) == std::get<0>(rectangleTwo.topLeftPoint) &&
      std::get<1>(rectangleOne.topLeftPoint) == std::get<1>(rectangleTwo.topLeftPoint) &&
      std::get<0>(rectangleOne.topRightPoint) == std::get<0>(rectangleTwo.topRightPoint) &&
      std::get<1>(rectangleOne.topRightPoint) == std::get<1>(rectangleTwo.topRightPoint) &&
      std::get<0>(rectangleOne.bottomLeftPoint) == std::get<0>(rectangleTwo.bottomLeftPoint) &&
      std::get<1>(rectangleOne.bottomLeftPoint) == std::get<1>(rectangleTwo.bottomLeftPoint) &&
      std::get<0>(rectangleOne.bottomRightPoint) == std::get<0>(rectangleTwo.bottomRightPoint) &&
      std::get<1>(rectangleOne.bottomRightPoint) == std::get<1>(rectangleTwo.bottomRightPoint))
  {
    return true;
  }
  return false;
}
bool operator<(const Triangle &triangleOne, const Triangle &triangleTwo)
{
  if (std::get<0>(triangleOne.rightAnglePoint) == std::get<0>(triangleTwo.rightAnglePoint) &&
      std::get<1>(triangleOne.rightAnglePoint) == std::get<1>(triangleTwo.rightAnglePoint) &&
      std::get<0>(triangleOne.sidePointA) == std::get<0>(triangleTwo.sidePointA) &&
      std::get<1>(triangleOne.sidePointA) == std::get<1>(triangleTwo.sidePointA) &&
      std::get<0>(triangleOne.sidePointB) == std::get<0>(triangleTwo.sidePointB) &&
      std::get<1>(triangleOne.sidePointB) == std::get<1>(triangleTwo.sidePointB))
  {
    return true;
  }
  return false;
}

bool operator<(const Rectangle &rectangleOne, const Rectangle &rectangleTwo)
{
  if (std::get<0>(rectangleOne.topLeftPoint) == std::get<0>(rectangleTwo.topLeftPoint) &&
      std::get<1>(rectangleOne.topLeftPoint) == std::get<1>(rectangleTwo.topLeftPoint) &&
      std::get<0>(rectangleOne.topRightPoint) == std::get<0>(rectangleTwo.topRightPoint) &&
      std::get<1>(rectangleOne.topRightPoint) == std::get<1>(rectangleTwo.topRightPoint) &&
      std::get<0>(rectangleOne.bottomLeftPoint) == std::get<0>(rectangleTwo.bottomLeftPoint) &&
      std::get<1>(rectangleOne.bottomLeftPoint) == std::get<1>(rectangleTwo.bottomLeftPoint) &&
      std::get<0>(rectangleOne.bottomRightPoint) == std::get<0>(rectangleTwo.bottomRightPoint) &&
      std::get<1>(rectangleOne.bottomRightPoint) == std::get<1>(rectangleTwo.bottomRightPoint))
  {
    return true;
  }
  return false;
}

std::tuple<int, int> GPStoLocal(std::tuple<int, int> point)
{
  return std::make_tuple(std::get<1>(point) + 30, std::get<0>(point) + 30);
}

std::tuple<int, int> LocaltoGPS(std::tuple<int, int> point)
{
  return std::make_tuple(std::get<1>(point) - 30, std::get<0>(point) - 30);
}

std::vector<Node *> getForbiddenNodesBetweenPoints(std::tuple<int, int> a, std::tuple<int, int> b)
{
}

void zoneOutTriangles(Graph graph)
{
  Node *origin;
  Node *destination;

  double slope_y;
  double slope_x;
  double x;
  double y;

  origin = graph.getNode(0, ZONE_SIZE);
  destination = graph.getNode(ZONE_SIZE, 0);
  origin->forbidden = true;
  destination->forbidden = true;
  slope_y = abs((destination->y - origin->y) / (destination->x - origin->x)) / 10.0;
  slope_x = abs((destination->x - origin->x) / (destination->y - origin->y)) / 10.0;
  x = origin->x;
  y = origin->y;

  std::cout << slope_x << " " << slope_y;

  do
  {
    graph.getNode(0 + x, ZONE_SIZE - y)->forbidden = true;
    x += slope_x;
    y -= slope_y;
  } while ((x <= destination->x) && (y >= destination->y));

  // int ZONE_SIZE = int(ZONE_SIZE);
  int ZONE_SIZEZ = int(ZONE_SIZE);

  for (int y = 0; y < Y_NODES; y++)
  {
    for (int x = 0; x < X_NODES; x++)
    {
    }
  }

  std::cout << ZONE_SIZEZ;

  graph.getNode(ZONE_SIZEZ, ZONE_SIZEZ)->forbidden = true;
  Node *node;

  // top left
  for (int y = 0; y <= ZONE_SIZEZ; y++)
  {
    for (int x = 0; x <= ZONE_SIZEZ; x++)
    {
      node = graph.getNode(x, y);

      if (node->forbidden)
      {
        break;
      }
      else
      {
        node->forbidden = true;
      }
    }
  }

  // top right
  for (int y = 0; y <= ZONE_SIZEZ; y++)
  {
    for (int x = FIELD_SIZE - 1; x >= FIELD_SIZE - ZONE_SIZEZ; x--)
    {
      node = graph.getNode(x, y);

      if (node->forbidden)
      {
        break;
      }
      else
      {
        node->forbidden = true;
      }
    }
  }

  // bottom left
  for (int y = FIELD_SIZE - 1; y >= FIELD_SIZE - ZONE_SIZEZ; y--)
  {
    for (int x = 0; x <= ZONE_SIZEZ; x++)
    {
      node = graph.getNode(x, y);

      if (node->forbidden)
      {
        break;
      }
      else
      {
        node->forbidden = true;
      }
    }
  }

  // bottom right
  for (int y = FIELD_SIZE - 1; y >= FIELD_SIZE - ZONE_SIZEZ; y--)
  {
    for (int x = FIELD_SIZE - 1; x >= FIELD_SIZE - ZONE_SIZEZ; x--)
    {
      node = graph.getNode(x, y);

      if (node->forbidden)
      {
        break;
      }
      else
      {
        node->forbidden = true;
      }
    }
  }
}

std::vector<Node *> getNodesInsideShape(Triangle triangle)
{
}

std::vector<Node *> getNodesInsideShape(Rectangle rectangle)
{
}

int main()
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
  double const BAR_MAIN_START_Y = 2 * ZONE_SIZE;
  double const BAR_MAIN_SIZE_X = 4 * ZONE_SIZE;
  double const BAR_MAIN_SIZE_Y = 2.375;

  std::vector<Triangle> forbiddenZonesTriangles;
  std::vector<Rectangle> forbiddenZonesRectangles;

  forbiddenZonesTriangles.push_back(Triangle(std::make_tuple(0, 0),
                                             std::make_tuple(0, ROLLER_SIZE),
                                             std::make_tuple(ROLLER_SIZE, 0)));
  forbiddenZonesTriangles.push_back(Triangle(std::make_tuple(FIELD_SIZE, 0),
                                             std::make_tuple(FIELD_SIZE, ROLLER_SIZE),
                                             std::make_tuple(FIELD_SIZE - ROLLER_SIZE, 0)));
  forbiddenZonesTriangles.push_back(Triangle(std::make_tuple(0, FIELD_SIZE),
                                             std::make_tuple(0, FIELD_SIZE - ROLLER_SIZE),
                                             std::make_tuple(ROLLER_SIZE, FIELD_SIZE)));
  forbiddenZonesTriangles.push_back(Triangle(std::make_tuple(FIELD_SIZE, FIELD_SIZE),
                                             std::make_tuple(FIELD_SIZE, FIELD_SIZE - ROLLER_SIZE),
                                             std::make_tuple(FIELD_SIZE - ROLLER_SIZE, FIELD_SIZE)));

  forbiddenZonesRectangles.push_back(Rectangle(std::make_tuple(GOAL_START_X, GOAL_START_Y),
                                               std::make_tuple(GOAL_START_X + GOAL_SIZE_X, GOAL_START_Y),
                                               std::make_tuple(GOAL_START_X, GOAL_START_Y + GOAL_SIZE_Y),
                                               std::make_tuple(GOAL_START_X + GOAL_SIZE_X, GOAL_START_Y + GOAL_SIZE_Y)));
  forbiddenZonesRectangles.push_back(Rectangle(std::make_tuple(GOAL_START_X, FIELD_SIZE),
                                               std::make_tuple(GOAL_START_X + GOAL_SIZE_X, FIELD_SIZE),
                                               std::make_tuple(GOAL_START_X, FIELD_SIZE - GOAL_SIZE_Y),
                                               std::make_tuple(GOAL_START_X + GOAL_SIZE_X, FIELD_SIZE - GOAL_SIZE_Y)));
  forbiddenZonesRectangles.push_back(Rectangle(std::make_tuple(BAR_LEFT_START_X, BAR_LEFT_START_Y),
                                               std::make_tuple(BAR_LEFT_START_X + BAR_LEFT_SIZE_X, BAR_LEFT_START_Y),
                                               std::make_tuple(BAR_LEFT_START_X, BAR_LEFT_START_Y + BAR_LEFT_SIZE_Y),
                                               std::make_tuple(BAR_LEFT_START_X + BAR_LEFT_SIZE_X, BAR_LEFT_START_Y + BAR_LEFT_SIZE_Y)));
  forbiddenZonesRectangles.push_back(Rectangle(std::make_tuple(BAR_RIGHT_START_X, BAR_RIGHT_START_Y),
                                               std::make_tuple(BAR_RIGHT_START_X + BAR_RIGHT_SIZE_X, BAR_RIGHT_START_Y),
                                               std::make_tuple(BAR_RIGHT_START_X, BAR_RIGHT_START_Y + BAR_RIGHT_SIZE_Y),
                                               std::make_tuple(BAR_RIGHT_START_X + BAR_RIGHT_SIZE_X, BAR_RIGHT_START_Y + BAR_RIGHT_SIZE_Y)));
  forbiddenZonesRectangles.push_back(Rectangle(std::make_tuple(BAR_MAIN_START_X, BAR_MAIN_START_Y),
                                               std::make_tuple(BAR_MAIN_START_X + BAR_MAIN_SIZE_X, BAR_MAIN_START_Y),
                                               std::make_tuple(BAR_MAIN_START_X, BAR_MAIN_START_Y + BAR_MAIN_SIZE_Y),
                                               std::make_tuple(BAR_MAIN_START_X + BAR_MAIN_SIZE_X, BAR_MAIN_START_Y + BAR_MAIN_SIZE_Y)));

  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<int> randX(0, X_NODES - 1);
  std::uniform_int_distribution<int> randY(0, Y_NODES - 1);

  int originX = randX(rng);
  int originY = randY(rng);
  int destinationX = randX(rng);
  int destinationY = randY(rng);

  std::cout << "Path from (" << originX << ", " << originY << ") to (" << destinationX << ", " << destinationY << ")\n\n";

  Graph graph = Graph(X_NODES, Y_NODES, forbiddenZonesTriangles, forbiddenZonesRectangles);
  std::vector<Node *> path = AStar(graph, graph.getNode(originX, originY), graph.getNode(destinationX, destinationY), h);

  for (int i = path.size() - 1; i >= 0; i--)
  {
    std::cout << "(" << path[i]->x << ", " << path[i]->y << ")\n";
  }

  zoneOutTriangles(graph);
  // std::cout << "\n\n\nPrinting forbidden nodes.\n\n";
  // for (Node *node : graph.getForbiddenNodes())
  // {
  //   std::cout << node << std::endl;
  // }

  std::ofstream file("data.txt");

  for (int x = 0; x < X_NODES; x++)
  {
    for (int y = 0; y < Y_NODES; y++)
    {
      if (graph.getNode(x, y)->forbidden)
      {
        file << graph.getNode(x, y)->x << " " << graph.getNode(x, y)->y << std::endl;
      }
    }
  }

  file.close();
}
