#include <iostream>
#include <set>
#include "Enums.h"
#include "Graph.h"
#include "Node.h"
#include <algorithm>

Node::Node(const int x, const int y, Graph *const graph) : x(x),
                                                           y(y),
                                                           graph(graph),
                                                           neighbors(new std::set<Node *>),
                                                           waypoint_neighbors(new std::set<Node *>)
{
  is_forbidden = false;
  waypoint = Waypoint::NONE;
};

bool Node::get_is_forbidden()
{
  return is_forbidden;
}

void Node::print_neighbors()
{
  for (Node *neighbor_node : (*neighbors))
  {
    neighbor_node->println();
  }
}

void Node::print_waypoint_neighbors()
{
  for (Node *waypoint_neighbor_node : (*waypoint_neighbors))
  {
    waypoint_neighbor_node->println();
  }
}

Waypoint Node::set_waypoint(Waypoint waypoint_value)
{
  waypoint = waypoint_value;
}

Waypoint Node::get_waypoint()
{
  return waypoint;
}

std::set<Node *> *Node::get_neighbors()
{
  return neighbors;
}

std::set<Node *> *Node::get_waypoint_neighbors()
{
  return waypoint_neighbors;
}

void Node::forbid()
{
  is_forbidden = true;

  for (Node *neighbor : (*neighbors))
  {
    neighbor->neighbors->erase(this);
  }
  for (Node *neighbor : (*waypoint_neighbors))
  {
    neighbor->waypoint_neighbors->erase(this);
  }

  graph->add_forbidden_node(this);
  neighbors->clear();
}

void Node::add_neighbor(Node *neighbor_node)
{
  neighbors->insert(neighbor_node);
}

void Node::remove_neighbor(Node *neighbor_node)
{
  neighbors->erase(neighbor_node);
}

void Node::add_waypoint_neighbor(Node *neighbor_node)
{
  waypoint_neighbors->insert(neighbor_node);
}

void Node::remove_waypoint_neighbor(Node *neighbor_node)
{
  waypoint_neighbors->erase(neighbor_node);
}

void Node::print()
{
  std::cout << "(" << x << ", " << y << ")\t";
}

void Node::println()
{
  std::cout << "(" << x << ", " << y << ")\n";
}