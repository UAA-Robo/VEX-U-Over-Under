#pragma once
#include <set>
#include "Enums.h"

class Graph;

class Node
{
public:
  const int x;
  const int y;

  Node(const int x, const int y, Graph *const graph);

  bool get_is_forbidden();

  Waypoint set_waypoint(Waypoint waypoint_value);
  Waypoint get_waypoint();

  std::set<Node *> *get_neighbors();
  std::set<Node *> *get_waypoint_neighbors();

  void forbid();
  void add_neighbor(Node *neighbor_node);
  void remove_neighbor(Node *neighbor_node);
  void add_waypoint_neighbor(Node *neighbor_node);
  void remove_waypoint_neighbor(Node *neighbor_node);
  void print();
  void println();

private:
  // TODO - should this be capitalized, since underlying value can change?
  Graph *const graph;
  bool is_forbidden;
  Waypoint waypoint;
  std::set<Node *> *const neighbors;
  std::set<Node *> *const waypoint_neighbors;
};