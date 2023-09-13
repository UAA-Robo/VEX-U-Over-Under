#include <map>
#include <vector>
#include <set>
#include <climits>
#include <cmath>
#include <iostream>
#include <utility>
#include <tuple>
#include <random>
#include <queue>

#include "Node.h"
#include "Graph.h"

using namespace std;

std::ostream &operator<<(std::ostream &os, Node const &m)
{
  return os << "(" << m.x << ", " << m.y << ")";
}

bool hasLOS(Graph *graph, Node *origin, Node *destination)
{
  double slope_y = (destination->y - origin->y) / (destination->x - origin->x);
  double slope_x = (destination->x - origin->x) / (destination->y - origin->y);
  double x = origin->x;
  double y = origin->y;

  while ((x + slope_x <= destination->x) && (y + slope_y <= destination->y))
  {
    x += slope_x;
    y += slope_y;

    if (graph->getNode(x, y)->forbidden)
    {
      return false;
    }
  }
  return true;
}

int const BUFFER_NODES = 5;

int const X_NODES = 50;
int const Y_NODES = 50;

double const FIELD_SIZE = 138.73;
double const ZONE_SIZE = FIELD_SIZE / 6;

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
  return 10 * (dx + dy) + (14 - 2 * 10) * min(dx, dy);
}

vector<Node *> reconstructPath(Node *currentNode)
{
  vector<Node *> path;
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

vector<Node *> AStar(Graph *graph, Node *origin, Node *destination, int (*h)(Node *, Node *))
{
  set<Node *> frontier;
  map<Node *, int> gScores;
  map<Node *, int> fScores;

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

vector<Node *> ThetaStar(Graph *graph, Node *origin, Node *destination, int (*h)(Node *, Node *))
{
  set<Node *> frontier;
  set<Node *> explored;
  map<Node *, int> gScores;
  map<Node *, int> fScores;

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

int main()
{
  random_device rd;
  mt19937 rng(rd());
  uniform_int_distribution<int> randX(0, X_NODES - 1);
  uniform_int_distribution<int> randY(0, Y_NODES - 1);

  int originX = randX(rng);
  int originY = randY(rng);
  int destinationX = randX(rng);
  int destinationY = randY(rng);

  cout << "Path from (" << originX << ", " << originY << ") to (" << destinationX << ", " << destinationY << ")\n\n";

  Graph *graph = new Graph(X_NODES, Y_NODES);
  vector<Node *> path = AStar(graph, graph->getNode(originX, originY), graph->getNode(destinationX, destinationY), h);
  for (int i = path.size() - 1; i >= 0; i--)
  {
    cout << "(" << path[i]->x << ", " << path[i]->y << ")\n";
  }

  double const ROLLER_SIZE = ZONE_SIZE;

  vector<tuple<int, int>> ROLLER_RED_RIGHT;
  ROLLER_RED_RIGHT.push_back(make_tuple(FIELD_SIZE, 0));
  ROLLER_RED_RIGHT.push_back(make_tuple(FIELD_SIZE, ROLLER_SIZE));
  ROLLER_RED_RIGHT.push_back(make_tuple(FIELD_SIZE - ROLLER_SIZE, 0));
  vector<tuple<int, int>> ROLLER_BLUE_LEFT;
  ROLLER_BLUE_LEFT.push_back(make_tuple(0, FIELD_SIZE));
  ROLLER_BLUE_LEFT.push_back(make_tuple(0, FIELD_SIZE - ROLLER_SIZE));
  ROLLER_BLUE_LEFT.push_back(make_tuple(ROLLER_SIZE, FIELD_SIZE));
  vector<tuple<int, int>> ROLLER_BLUE_RIGHT;
  ROLLER_BLUE_RIGHT.push_back(make_tuple(FIELD_SIZE, FIELD_SIZE));
  ROLLER_BLUE_RIGHT.push_back(make_tuple(FIELD_SIZE, FIELD_SIZE - ROLLER_SIZE));
  ROLLER_BLUE_RIGHT.push_back(make_tuple(FIELD_SIZE - ROLLER_SIZE, FIELD_SIZE));

  double const GOAL_START_X = 2 * ZONE_SIZE;
  double const GOAL_START_Y = 0;
  double const GOAL_SIZE_X = 2 * ZONE_SIZE;
  double const GOAL_SIZE_Y = ZONE_SIZE;

  vector<tuple<int, int>> GOAL_RED;
  vector<tuple<int, int>> GOAL_BLUE;
  GOAL_BLUE.push_back(make_tuple(GOAL_START_X, GOAL_START_Y));
  GOAL_BLUE.push_back(make_tuple(GOAL_START_X + GOAL_SIZE_X, GOAL_START_Y));
  GOAL_BLUE.push_back(make_tuple(GOAL_START_X, GOAL_START_Y + GOAL_SIZE_Y));
  GOAL_BLUE.push_back(make_tuple(GOAL_START_X + GOAL_SIZE_X, GOAL_START_Y + GOAL_SIZE_Y));

  GOAL_RED.push_back(make_tuple(GOAL_START_X, FIELD_SIZE));
  GOAL_RED.push_back(make_tuple(GOAL_START_X + GOAL_SIZE_X, FIELD_SIZE));
  GOAL_RED.push_back(make_tuple(GOAL_START_X, FIELD_SIZE - GOAL_SIZE_Y));
  GOAL_RED.push_back(make_tuple(GOAL_START_X + GOAL_SIZE_X, FIELD_SIZE - GOAL_SIZE_Y));

  double const BAR_LEFT_START_X = ZONE_SIZE;
  double const BAR_LEFT_START_Y = 2 * ZONE_SIZE;
  double const BAR_LEFT_SIZE_X = 2.375;
  double const BAR_LEFT_SIZE_Y = 2 * ZONE_SIZE;
  double const BAR_RIGHT_START_X = 5 * ZONE_SIZE;
  double const BAR_RIGHT_START_Y = 2 * ZONE_SIZE;
  double const BAR_RIGHT_SIZE_X = 2.375;
  double const BAR_RIGHT_SIZE_Y = 2 * ZONE_SIZE;

  vector<tuple<int, int>> BAR_LEFT;
  vector<tuple<int, int>> BAR_RIGHT;
  BAR_LEFT.push_back(make_tuple(BAR_LEFT_START_X, BAR_LEFT_START_Y));
  BAR_LEFT.push_back(make_tuple(BAR_LEFT_START_X + BAR_LEFT_SIZE_X, BAR_LEFT_START_Y));
  BAR_LEFT.push_back(make_tuple(BAR_LEFT_START_X, BAR_LEFT_START_Y + BAR_LEFT_SIZE_Y));
  BAR_LEFT.push_back(make_tuple(BAR_LEFT_START_X + BAR_LEFT_SIZE_X, BAR_LEFT_START_Y + BAR_LEFT_SIZE_Y));

  BAR_RIGHT.push_back(make_tuple(BAR_RIGHT_START_X, BAR_RIGHT_START_Y));
  BAR_RIGHT.push_back(make_tuple(BAR_RIGHT_START_X + BAR_RIGHT_SIZE_X, BAR_RIGHT_START_Y));
  BAR_RIGHT.push_back(make_tuple(BAR_RIGHT_START_X, BAR_RIGHT_START_Y + BAR_RIGHT_SIZE_Y));
  BAR_RIGHT.push_back(make_tuple(BAR_RIGHT_START_X + BAR_RIGHT_SIZE_X, BAR_RIGHT_START_Y + BAR_RIGHT_SIZE_Y));

  double const BAR_MAIN_START_X = ZONE_SIZE;
  double const BAR_MAIN_START_Y = 2 * ZONE_SIZE;
  double const BAR_MAIN_SIZE_X = 4 * ZONE_SIZE;
  double const BAR_MAIN_SIZE_Y = 2.375;

  vector<tuple<int, int>> BAR_MAIN;

  BAR_MAIN.push_back(make_tuple(BAR_MAIN_START_X, BAR_MAIN_START_Y));
  BAR_MAIN.push_back(make_tuple(BAR_MAIN_START_X + BAR_MAIN_SIZE_X, BAR_MAIN_START_Y));
  BAR_MAIN.push_back(make_tuple(BAR_MAIN_START_X, BAR_MAIN_START_Y + BAR_MAIN_SIZE_Y));
  BAR_MAIN.push_back(make_tuple(BAR_MAIN_START_X + BAR_MAIN_SIZE_X, BAR_MAIN_START_Y + BAR_MAIN_SIZE_Y));
}

tuple<int, int> GPStoLocal(tuple<int, int> point)
{
  return make_tuple(get<1>(point) + 30, get<0>(point) + 30);
}

tuple<int, int> LocaltoGPS(tuple<int, int> point)
{
  return make_tuple(get<1>(point) - 30, get<0>(point) - 30);
}
