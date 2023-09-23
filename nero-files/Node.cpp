#include <iostream>
#include <set>
#include "Enums.h"
#include "Graph.h"
#include "Node.h"

//=============================================================================
//=============================================================================
// PUBLIC METHODS
//=============================================================================
//=============================================================================

Node::Node(int x, int y, Graph *graph)
{
  this->x = x;
  this->y = y;
  this->graph = graph;
  this->hit = false;
  forbidden = false;
  waypoint = NO;
};

void Node::addNeighbor(Node *neighborNode)
{
  neighbors.insert(neighborNode);
  neighborNode->neighbors.insert(this);
};

void Node::addWaypointNeighbor(Node *neighborNode)
{
  waypointNeighbors.insert(neighborNode);
  neighborNode->waypointNeighbors.insert(this);
}

void Node::forbid()
{
  forbidden = true;

  for (Node *neighbor : neighbors)
  {
    neighbor->neighbors.erase(this);
  }

  graph->addForbiddenNode(this);
  neighbors.clear();
}

void Node::print()
{
  std::cout << "(" << x << ", " << y << ")\t";
}

void Node::println()
{
  std::cout << "(" << x << ", " << y << ")\n";
}
