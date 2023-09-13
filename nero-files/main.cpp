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
#include <cstdio>
#include <SDL2/SDL.h>

#include "Node.h"
#include "Graph.h"
#include "Triangle.h"
#include "Rectangle.h"

// double const ZONE_SIZE = 1;
double const CELL_SIZE = 5;

void drawCell(SDL_Renderer *renderer, Node *node)
{
  SDL_Rect cell;
  cell.x = node->x * CELL_SIZE;
  cell.y = node->y * CELL_SIZE;
  cell.h = CELL_SIZE;
  cell.w = CELL_SIZE;

  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderFillRect(renderer, &cell);
}

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

// bool operator==(const Triangle &triangleOne, const Triangle &triangleTwo)
// {
//   if (std::get<0>(triangleOne.rightAnglePoint) == std::get<0>(triangleTwo.rightAnglePoint) &&
//       std::get<1>(triangleOne.rightAnglePoint) == std::get<1>(triangleTwo.rightAnglePoint) &&
//       std::get<0>(triangleOne.sidePointA) == std::get<0>(triangleTwo.sidePointA) &&
//       std::get<1>(triangleOne.sidePointA) == std::get<1>(triangleTwo.sidePointA) &&
//       std::get<0>(triangleOne.sidePointB) == std::get<0>(triangleTwo.sidePointB) &&
//       std::get<1>(triangleOne.sidePointB) == std::get<1>(triangleTwo.sidePointB))
//   {
//     return true;
//   }
//   return false;
// }

// bool operator==(const Rectangle &rectangleOne, const Rectangle &rectangleTwo)
// {
//   if (std::get<0>(rectangleOne.topLeftPoint) == std::get<0>(rectangleTwo.topLeftPoint) &&
//       std::get<1>(rectangleOne.topLeftPoint) == std::get<1>(rectangleTwo.topLeftPoint) &&
//       std::get<0>(rectangleOne.topRightPoint) == std::get<0>(rectangleTwo.topRightPoint) &&
//       std::get<1>(rectangleOne.topRightPoint) == std::get<1>(rectangleTwo.topRightPoint) &&
//       std::get<0>(rectangleOne.bottomLeftPoint) == std::get<0>(rectangleTwo.bottomLeftPoint) &&
//       std::get<1>(rectangleOne.bottomLeftPoint) == std::get<1>(rectangleTwo.bottomLeftPoint) &&
//       std::get<0>(rectangleOne.bottomRightPoint) == std::get<0>(rectangleTwo.bottomRightPoint) &&
//       std::get<1>(rectangleOne.bottomRightPoint) == std::get<1>(rectangleTwo.bottomRightPoint))
//   {
//     return true;
//   }
//   return false;
// }
// bool operator<(const Triangle &triangleOne, const Triangle &triangleTwo)
// {
//   if (std::get<0>(triangleOne.rightAnglePoint) == std::get<0>(triangleTwo.rightAnglePoint) &&
//       std::get<1>(triangleOne.rightAnglePoint) == std::get<1>(triangleTwo.rightAnglePoint) &&
//       std::get<0>(triangleOne.sidePointA) == std::get<0>(triangleTwo.sidePointA) &&
//       std::get<1>(triangleOne.sidePointA) == std::get<1>(triangleTwo.sidePointA) &&
//       std::get<0>(triangleOne.sidePointB) == std::get<0>(triangleTwo.sidePointB) &&
//       std::get<1>(triangleOne.sidePointB) == std::get<1>(triangleTwo.sidePointB))
//   {
//     return true;
//   }
//   return false;
// }

// bool operator<(const Rectangle &rectangleOne, const Rectangle &rectangleTwo)
// {
//   if (std::get<0>(rectangleOne.topLeftPoint) == std::get<0>(rectangleTwo.topLeftPoint) &&
//       std::get<1>(rectangleOne.topLeftPoint) == std::get<1>(rectangleTwo.topLeftPoint) &&
//       std::get<0>(rectangleOne.topRightPoint) == std::get<0>(rectangleTwo.topRightPoint) &&
//       std::get<1>(rectangleOne.topRightPoint) == std::get<1>(rectangleTwo.topRightPoint) &&
//       std::get<0>(rectangleOne.bottomLeftPoint) == std::get<0>(rectangleTwo.bottomLeftPoint) &&
//       std::get<1>(rectangleOne.bottomLeftPoint) == std::get<1>(rectangleTwo.bottomLeftPoint) &&
//       std::get<0>(rectangleOne.bottomRightPoint) == std::get<0>(rectangleTwo.bottomRightPoint) &&
//       std::get<1>(rectangleOne.bottomRightPoint) == std::get<1>(rectangleTwo.bottomRightPoint))
//   {
//     return true;
//   }
//   return false;
// }

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

int getPositionRelative(Node *a, Node *b)
{
  int ax = a->x;
  int ay = a->y;
  int bx = b->x;
  int by = b->y;

  // top left
  if (bx < ax && by < ay)
  {
    return 0;
  }
  // top right
  else if (bx > ax && by < ay)
  {
    return 1;
  }
  // bottom left
  else if (bx < ax && by > ay)
  {
    return 2;
  }
  // bottom right
  else if (bx > ax && by > ay)
  {
    return 3;
  }
}

// void forbidTriangle(Graph graph, Node *a, Node *b)
// {

//   int ax = a->x;
//   int ay = a->y;
//   int bx = b->x;
//   int by = b->y;
//   int x = ax;
//   int y = ay;
//   int xo = x;
//   int yo = y;

//   // b is top left of a
//   if (getPositionRelative(a, b) == 0)
//   {
//     while (x >= bx && y >= by)
//     {
//       for (int yy = y; yy >= y; yy--)
//       {
//         for (int xx = xo; xx >= x; xx--)
//         {
//           graph.getNode(xx, yy)->forbidden = true;
//         }
//       }

//       x--;
//       y--;
//     }
//   }
//   // b is top right of a
//   else if (getPositionRelative(a, b) == 1)
//   {
//     while (x <= bx && y >= by)
//     {
//       graph.getNode(x, y)->forbidden = true;
//       x++;
//       y--;
//     }
//   }
//   // b is bottom left of a
//   else if (getPositionRelative(a, b) == 2)
//   {
//     while (x >= bx && y <= by)
//     {
//       graph.getNode(x, y)->forbidden = true;
//       x--;
//       y++;
//     }
//   }
//   // b is bottom right of a
//   else if (getPositionRelative(a, b) == 3)
//   {
//     while (x <= bx && y <= by)
//     {
//       graph.getNode(x, y)->forbidden = true;
//       x++;
//       y++;
//     }
//   }
// }

void forbidTriangle(Graph graph, Node *a, Node *b)
{

  int ax = a->x;
  int ay = a->y;
  int bx = b->x;
  int by = b->y;
  int x = ax;
  int y = ay;
  int xo = x;
  int yo = y;

  // b is top left of a
  if (getPositionRelative(a, b) == 0)
  {
    while (x >= bx && y >= by)
    {
      for (int xx = xo; xx >= x; xx--)
      {
        graph.forbidNode(graph.getNode(xx, y));
        // graph.getNode(xx, y)->forbidden = true;
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
      for (int xx = xo; xx <= x; xx++)
      {
        graph.forbidNode(graph.getNode(xx, y));
        // graph.getNode(xx, y)->forbidden = true;
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
      for (int xx = xo; xx >= x; xx--)
      {
        graph.forbidNode(graph.getNode(xx, y));
        // graph.getNode(xx, y)->forbidden = true;
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
      for (int xx = xo; xx <= x; xx++)
      {
        graph.forbidNode(graph.getNode(xx, y));
        // graph.getNode(xx, y)->forbidden = true;
      }
      x++;
      y++;
    }
  }
}

void forbidRectangle(Graph graph, Node *topLeftPoint, Node *topRightPoint, Node *bottomLeftPoint, Node *bottomRightPoint)
{
  for (int y = topLeftPoint->y; y <= bottomLeftPoint->y; y++)
  {
    for (int x = topLeftPoint->x; x <= topRightPoint->x; x++)
    {
      graph.forbidNode(graph.getNode(x, y));
      // graph.getNode(x, y)->forbidden = true;
    }
  }
}

std::vector<Node *> getNodesInsideShape(Triangle triangle)
{
}

std::vector<Node *> getNodesInsideShape(Rectangle rectangle)
{
}

int main(int argv, char **args)
{
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<int> randX(0, X_NODES - 1);
  std::uniform_int_distribution<int> randY(0, Y_NODES - 1);

  int originX = randX(rng);
  int originY = randY(rng);
  int destinationX = randX(rng);
  int destinationY = randY(rng);

  std::cout << "Path from (" << originX << ", " << originY << ") to (" << destinationX << ", " << destinationY << ")\n\n";

  Graph graph = Graph(X_NODES, Y_NODES);
  std::vector<Node *> path = AStar(graph, graph.getNode(originX, originY), graph.getNode(destinationX, destinationY), h);

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

  // forbiddenZonesTriangles.push_back(Triangle(graph.getNode(0, 0),
  //                                            graph.getNode(0, ROLLER_SIZE),
  //                                            graph.getNode(ROLLER_SIZE, 0)));
  // forbiddenZonesTriangles.push_back(Triangle(graph.getNode(FIELD_SIZE, 0),
  //                                            graph.getNode(FIELD_SIZE, ROLLER_SIZE),
  //                                            graph.getNode(FIELD_SIZE - ROLLER_SIZE, 0)));
  // forbiddenZonesTriangles.push_back(Triangle(graph.getNode(0, FIELD_SIZE),
  //                                            graph.getNode(0, FIELD_SIZE - ROLLER_SIZE),
  //                                            graph.getNode(ROLLER_SIZE, FIELD_SIZE)));
  // forbiddenZonesTriangles.push_back(Triangle(graph.getNode(FIELD_SIZE, FIELD_SIZE),
  //                                            graph.getNode(FIELD_SIZE, FIELD_SIZE - ROLLER_SIZE),
  //                                            graph.getNode(FIELD_SIZE - ROLLER_SIZE, FIELD_SIZE)));

  // forbiddenZonesRectangles.push_back(Rectangle(graph.getNode(GOAL_START_X, GOAL_START_Y),
  //                                              graph.getNode(GOAL_START_X + GOAL_SIZE_X, GOAL_START_Y),
  //                                              graph.getNode(GOAL_START_X, GOAL_START_Y + GOAL_SIZE_Y),
  //                                              graph.getNode(GOAL_START_X + GOAL_SIZE_X, GOAL_START_Y + GOAL_SIZE_Y)));
  // forbiddenZonesRectangles.push_back(Rectangle(graph.getNode(GOAL_START_X, FIELD_SIZE),
  //                                              graph.getNode(GOAL_START_X + GOAL_SIZE_X, FIELD_SIZE),
  //                                              graph.getNode(GOAL_START_X, FIELD_SIZE - GOAL_SIZE_Y),
  //                                              graph.getNode(GOAL_START_X + GOAL_SIZE_X, FIELD_SIZE - GOAL_SIZE_Y)));
  // forbiddenZonesRectangles.push_back(Rectangle(graph.getNode(BAR_LEFT_START_X, BAR_LEFT_START_Y),
  //                                              graph.getNode(BAR_LEFT_START_X + BAR_LEFT_SIZE_X, BAR_LEFT_START_Y),
  //                                              graph.getNode(BAR_LEFT_START_X, BAR_LEFT_START_Y + BAR_LEFT_SIZE_Y),
  //                                              graph.getNode(BAR_LEFT_START_X + BAR_LEFT_SIZE_X, BAR_LEFT_START_Y + BAR_LEFT_SIZE_Y)));
  // forbiddenZonesRectangles.push_back(Rectangle(graph.getNode(BAR_RIGHT_START_X, BAR_RIGHT_START_Y),
  //                                              graph.getNode(BAR_RIGHT_START_X + BAR_RIGHT_SIZE_X, BAR_RIGHT_START_Y),
  //                                              graph.getNode(BAR_RIGHT_START_X, BAR_RIGHT_START_Y + BAR_RIGHT_SIZE_Y),
  //                                              graph.getNode(BAR_RIGHT_START_X + BAR_RIGHT_SIZE_X, BAR_RIGHT_START_Y + BAR_RIGHT_SIZE_Y)));
  // forbiddenZonesRectangles.push_back(Rectangle(graph.getNode(BAR_MAIN_START_X, BAR_MAIN_START_Y),
  //                                              graph.getNode(BAR_MAIN_START_X + BAR_MAIN_SIZE_X, BAR_MAIN_START_Y),
  //                                              graph.getNode(BAR_MAIN_START_X, BAR_MAIN_START_Y + BAR_MAIN_SIZE_Y),
  //                                              graph.getNode(BAR_MAIN_START_X + BAR_MAIN_SIZE_X, BAR_MAIN_START_Y + BAR_MAIN_SIZE_Y)));

  forbiddenZonesTriangles.push_back(Triangle(graph.getNode(0, 0),
                                             graph.getNode(0, ROLLER_SIZE),
                                             graph.getNode(ROLLER_SIZE, 0)));
  forbiddenZonesTriangles.push_back(Triangle(graph.getNode(FIELD_SIZE - 1, 0),
                                             graph.getNode(FIELD_SIZE - 1, ROLLER_SIZE),
                                             graph.getNode(FIELD_SIZE - 1 - ROLLER_SIZE, 0)));
  forbiddenZonesTriangles.push_back(Triangle(graph.getNode(0, FIELD_SIZE - 1),
                                             graph.getNode(0, FIELD_SIZE - 1 - ROLLER_SIZE),
                                             graph.getNode(ROLLER_SIZE, FIELD_SIZE - 1)));
  forbiddenZonesTriangles.push_back(Triangle(graph.getNode(FIELD_SIZE - 1, FIELD_SIZE - 1),
                                             graph.getNode(FIELD_SIZE - 1, FIELD_SIZE - 1 - ROLLER_SIZE),
                                             graph.getNode(FIELD_SIZE - 1 - ROLLER_SIZE, FIELD_SIZE - 1)));

  forbiddenZonesRectangles.push_back(Rectangle(graph.getNode(GOAL_START_X, GOAL_START_Y),
                                               graph.getNode(GOAL_START_X + GOAL_SIZE_X, GOAL_START_Y),
                                               graph.getNode(GOAL_START_X, GOAL_START_Y + GOAL_SIZE_Y),
                                               graph.getNode(GOAL_START_X + GOAL_SIZE_X, GOAL_START_Y + GOAL_SIZE_Y)));
  // forbiddenZonesRectangles.push_back(Rectangle(graph.getNode(GOAL_START_X, FIELD_SIZE - 1),
  //                                              graph.getNode(GOAL_START_X + GOAL_SIZE_X, FIELD_SIZE - 1),
  //                                              graph.getNode(GOAL_START_X, FIELD_SIZE - 1 - GOAL_SIZE_Y),
  //                                              graph.getNode(GOAL_START_X + GOAL_SIZE_X, FIELD_SIZE - 1 - GOAL_SIZE_Y)));
  // forbiddenZonesRectangles.push_back(Rectangle(graph.getNode(GOAL_START_X, FIELD_SIZE - 1), // bottom left
  //                                              graph.getNode(GOAL_START_X + GOAL_SIZE_X, FIELD_SIZE - 1), // bottom right
  //                                              graph.getNode(GOAL_START_X, FIELD_SIZE - 1 - GOAL_SIZE_Y), // top left
  //                                              graph.getNode(GOAL_START_X + GOAL_SIZE_X, FIELD_SIZE - 1 - GOAL_SIZE_Y))); // top right
  forbiddenZonesRectangles.push_back(Rectangle(graph.getNode(GOAL_START_X, FIELD_SIZE - 1 - GOAL_SIZE_Y),
                                               graph.getNode(GOAL_START_X + GOAL_SIZE_X, FIELD_SIZE - 1 - GOAL_SIZE_Y),
                                               graph.getNode(GOAL_START_X, FIELD_SIZE - 1),
                                               graph.getNode(GOAL_START_X + GOAL_SIZE_X, FIELD_SIZE - 1)));
  forbiddenZonesRectangles.push_back(Rectangle(graph.getNode(BAR_LEFT_START_X, BAR_LEFT_START_Y),
                                               graph.getNode(BAR_LEFT_START_X + BAR_LEFT_SIZE_X, BAR_LEFT_START_Y),
                                               graph.getNode(BAR_LEFT_START_X, BAR_LEFT_START_Y + BAR_LEFT_SIZE_Y),
                                               graph.getNode(BAR_LEFT_START_X + BAR_LEFT_SIZE_X, BAR_LEFT_START_Y + BAR_LEFT_SIZE_Y)));
  forbiddenZonesRectangles.push_back(Rectangle(graph.getNode(BAR_RIGHT_START_X, BAR_RIGHT_START_Y),
                                               graph.getNode(BAR_RIGHT_START_X + BAR_RIGHT_SIZE_X, BAR_RIGHT_START_Y),
                                               graph.getNode(BAR_RIGHT_START_X, BAR_RIGHT_START_Y + BAR_RIGHT_SIZE_Y),
                                               graph.getNode(BAR_RIGHT_START_X + BAR_RIGHT_SIZE_X, BAR_RIGHT_START_Y + BAR_RIGHT_SIZE_Y)));
  forbiddenZonesRectangles.push_back(Rectangle(graph.getNode(BAR_MAIN_START_X, BAR_MAIN_START_Y),
                                               graph.getNode(BAR_MAIN_START_X + BAR_MAIN_SIZE_X, BAR_MAIN_START_Y),
                                               graph.getNode(BAR_MAIN_START_X, BAR_MAIN_START_Y + BAR_MAIN_SIZE_Y),
                                               graph.getNode(BAR_MAIN_START_X + BAR_MAIN_SIZE_X, BAR_MAIN_START_Y + BAR_MAIN_SIZE_Y)));

  std::remove("path.txt");
  std::ofstream fileWritePath("path.txt");
  fileWritePath << path.size();
  for (int i = path.size() - 1; i >= 0; i--)
  {
    // std::cout << "(" << path[i]->x << ", " << path[i]->y << ")\n";
    fileWritePath << "" << path[i]->x << " " << path[i]->y << "\n";
  }
  fileWritePath.close();

  // forbidTriangle(graph, graph.getNode(0, ROLLER_SIZE), graph.getNode(ROLLER_SIZE, 0));
  // forbidTriangle(graph, graph.getNode(FIELD_SIZE - 1 - ROLLER_SIZE, 0), graph.getNode(FIELD_SIZE - 1, ROLLER_SIZE));
  // forbidTriangle(graph, graph.getNode(0, FIELD_SIZE - 1 - ROLLER_SIZE), graph.getNode(ROLLER_SIZE, FIELD_SIZE - 1));
  // forbidTriangle(graph, graph.getNode(FIELD_SIZE - 1 - ROLLER_SIZE, FIELD_SIZE - 1), graph.getNode(FIELD_SIZE - 1, FIELD_SIZE - 1 - ROLLER_SIZE));

  for (Triangle triangle : forbiddenZonesTriangles)
  {
    forbidTriangle(graph, triangle.sidePointA, triangle.sidePointB);
  }

  for (Rectangle rectangle : forbiddenZonesRectangles)
  {
    forbidRectangle(graph, rectangle.topLeftPoint, rectangle.topRightPoint, rectangle.bottomLeftPoint, rectangle.bottomRightPoint);
  }

  std::remove("forbidden-nodes.txt");
  std::ofstream fileWriteForbiddenNodes("forbidden-nodes.txt");

  for (int x = 0; x < X_NODES; x++)
  {
    for (int y = 0; y < Y_NODES; y++)
    {
      if (graph.getNode(x, y)->forbidden)
      {
        fileWriteForbiddenNodes << graph.getNode(x, y)->x << " " << graph.getNode(x, y)->y << std::endl;
      }
    }
  }

  fileWriteForbiddenNodes.close();

  std::vector<Node *> pathNodes;
  std::vector<SDL_Rect> pathRects;

  std::ifstream fileReadPath("path.txt");

  std::string xx;
  std::string yy;

  while (getline(fileReadPath, xx, ' '))
  {
    getline(fileReadPath, yy, '\n');
    pathRects.push_back(SDL_Rect{std::stoi(xx) * 5, std::stoi(yy) * 5, 5, 5});
  }
  fileReadPath.close();

  std::vector<SDL_Rect> selectedNodes;

  // int grid_cell_size = 5;
  int grid_cell_size = CELL_SIZE;
  int grid_width = 138;
  int grid_height = 138;

  // + 1 so that the last grid lines fit in the screen.
  int window_width = (grid_width * grid_cell_size) + 1;
  int window_height = (grid_height * grid_cell_size) + 1;

  // Place the grid cursor in the middle of the screen.
  SDL_Rect grid_cursor = {
      // .x = (grid_width - 1) / 2 * grid_cell_size,
      // .y = (grid_height - 1) / 2 * grid_cell_size,
      .w = grid_cell_size,
      .h = grid_cell_size,
  };

  // The cursor ghost is a cursor that always shows in the cell below the
  // mouse cursor.
  // SDL_Rect grid_cursor_ghost = {grid_cursor.x, grid_cursor.y, grid_cell_size,
  //                               grid_cell_size};
  SDL_Rect grid_cursor_ghost = {0, 0, grid_cell_size,
                                grid_cell_size};

  // Dark theme.
  // SDL_Color grid_background = {22, 22, 22, 255}; // Barely Black
  // SDL_Color grid_line_color = {44, 44, 44, 255}; // Dark grey
  // SDL_Color grid_cursor_ghost_color = {44, 44, 44, 255};
  // SDL_Color grid_cursor_color = {255, 255, 255, 255}; // White

  // Light Theme.
  SDL_Color grid_background = {233, 233, 233, 255}; // Barely white
  SDL_Color grid_line_color = {200, 200, 200, 255}; // Very light grey
  SDL_Color grid_cursor_ghost_color = {200, 200, 200, 255};
  SDL_Color grid_cursor_color = {160, 160, 160, 255}; // Grey

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Initialize SDL: %s",
                 SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_Window *window;
  SDL_Renderer *renderer;
  if (SDL_CreateWindowAndRenderer(window_width, window_height, 0, &window,
                                  &renderer) < 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Create window and renderer: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_SetWindowTitle(window, "SDL Grid");

  SDL_bool quit = SDL_FALSE;
  SDL_bool mouse_active = SDL_FALSE;
  SDL_bool mouse_hover = SDL_FALSE;

  std::string x;
  std::string y;
  std::ifstream fileReadForbiddenNodes("forbidden-nodes.txt");

  std::vector<Node *> forbiddenNodes;
  Node *node;

  while (getline(fileReadForbiddenNodes, x, ' '))
  {
    getline(fileReadForbiddenNodes, y, '\n');
    forbiddenNodes.push_back(new Node(std::stoi(x), std::stoi(y)));
  }
  fileReadForbiddenNodes.close();

  while (!quit)
  {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_w:
        case SDLK_UP:
          // grid_cursor.y -= grid_cell_size;
          break;
        case SDLK_s:
        case SDLK_DOWN:
          // grid_cursor.y += grid_cell_size;
          break;
        case SDLK_a:
        case SDLK_LEFT:
          // grid_cursor.x -= grid_cell_size;
          break;
        case SDLK_d:
        case SDLK_RIGHT:
          // grid_cursor.x += grid_cell_size;
          break;
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
      {

        bool nodeAllowed = true;
        int xx = (event.motion.x / grid_cell_size) * grid_cell_size;
        int yy = (event.motion.y / grid_cell_size) * grid_cell_size;

        for (Node *forbiddenNode : forbiddenNodes)
        {
          if (forbiddenNode->x * grid_cell_size == xx && forbiddenNode->y * grid_cell_size == yy)
          {
            nodeAllowed = false;
          }
        }
        if (nodeAllowed)
        {
          bool isNotDuplicate = true;
          for (SDL_Rect node : selectedNodes)
          {
            if (node.x == xx && node.y == yy)
            {
              isNotDuplicate = false;
            }
          }
          if (isNotDuplicate)
          {

            selectedNodes.push_back(SDL_Rect{
                .x = (event.motion.x / grid_cell_size) * grid_cell_size,
                .y = (event.motion.y / grid_cell_size) * grid_cell_size,
                .w = grid_cell_size,
                .h = grid_cell_size,
            });
          }
        }
      }
      break;

        // if (grid_cursor.x == NULL)
        // {
        //   std::cout << grid_cursor.x;
        //   grid_cursor.x = (event.motion.x / grid_cell_size) * grid_cell_size;
        //   grid_cursor.y = (event.motion.y / grid_cell_size) * grid_cell_size;
        // }
        // grid_cursor.x = (event.motion.x / grid_cell_size) * grid_cell_size;
        // grid_cursor.y = (event.motion.y / grid_cell_size) * grid_cell_size;
        // SDL_Rect cell = {

      case SDL_MOUSEMOTION:
        grid_cursor_ghost.x = (event.motion.x / grid_cell_size) * grid_cell_size;
        grid_cursor_ghost.y = (event.motion.y / grid_cell_size) * grid_cell_size;

        if (!mouse_active)
        {
          mouse_active = SDL_TRUE;
        }
        break;
      case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_ENTER && !mouse_hover)
          mouse_hover = SDL_TRUE;
        else if (event.window.event == SDL_WINDOWEVENT_LEAVE && mouse_hover)
          mouse_hover = SDL_FALSE;
        break;
      case SDL_QUIT:
        quit = SDL_TRUE;
        break;
      }
    }

    // Draw grid background.
    SDL_SetRenderDrawColor(renderer, grid_background.r, grid_background.g,
                           grid_background.b, grid_background.a);
    SDL_RenderClear(renderer);

    // Draw grid lines.
    SDL_SetRenderDrawColor(renderer, grid_line_color.r, grid_line_color.g,
                           grid_line_color.b, grid_line_color.a);

    for (int x = 0; x < 1 + grid_width * grid_cell_size;
         x += grid_cell_size)
    {
      SDL_RenderDrawLine(renderer, x, 0, x, window_height);
    }

    for (int y = 0; y < 1 + grid_height * grid_cell_size;
         y += grid_cell_size)
    {
      SDL_RenderDrawLine(renderer, 0, y, window_width, y);
    }

    // Draw grid ghost cursor.
    if (mouse_active && mouse_hover)
    {
      SDL_SetRenderDrawColor(renderer, grid_cursor_ghost_color.r,
                             grid_cursor_ghost_color.g,
                             grid_cursor_ghost_color.b,
                             grid_cursor_ghost_color.a);
      SDL_RenderFillRect(renderer, &grid_cursor_ghost);
    }

    // Draw grid cursor.
    SDL_SetRenderDrawColor(renderer, grid_cursor_color.r,
                           grid_cursor_color.g, grid_cursor_color.b,
                           grid_cursor_color.a);
    SDL_RenderFillRect(renderer, &grid_cursor);
    for (auto cell : selectedNodes)
    {
      SDL_RenderFillRect(renderer, &cell);
    }

    for (Node *node : forbiddenNodes)
    {
      drawCell(renderer, node);
    }

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (SDL_Rect rect : pathRects)
    {

      SDL_RenderFillRect(renderer, &rect);
    }

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  std::remove("selected-nodes.txt");
  std::ofstream fileWriteSelectedNodes("selected-nodes.txt");
  for (auto cell : selectedNodes)
  {
    // std::cout << "(" << cell.x << ", " << cell.y << ")\n";
    fileWriteSelectedNodes << "(" << cell.x << ", " << cell.y << ")\n";
  }
  fileWriteSelectedNodes.close();

  return EXIT_SUCCESS;
}