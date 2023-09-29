// Remove redundant this->
// Standarize between node, currentNode, a, b
// do constructors and deconstructors need to be virtual?
// pass heavy stuff around with pointers or by reference

#pragma once
#include <vector>
#include <map>
#include "Node.h"

class Graph
{
public:
  const int X_NODES_COUNT;
  const int Y_NODES_COUNT;
  const double NODE_SIZE;

  Graph(int X_NODES_COUNT, int Y_NODES_COUNT, double NODE_SIZE);
  ~Graph();
  Node *get_node(int x, int y);
  void add_forbidden_node(Node *node);
  std::vector<Node *> *get_forbidden_nodes();
  std::set<Node *> *get_waypoints();
  bool check_LOS(Node *a, Node *b);
  std::vector<Node *> *get_path(Node *origin, Node *destination);
  std::vector<Node *> *get_random_path();
  std::vector<std::vector<Node *> *> *get_path_snapshots(Node *origin, Node *destination);
  std::vector<std::vector<Node *> *> *get_random_path_snapshots();

private:
  Node ***const nodes;
  std::vector<Node *> *const forbidden_nodes;
  std::set<Node *> *const waypoints;
  std::vector<Node *> *path_nodes;
  std::vector<std::vector<Node *> *> *path_nodes_snapshots;

  void forbid_triangle(Node *a, Node *b, int robot_zones_count, int buffer_zones_count);
  void forbid_rectangle(Node *topLeftPoint, Node *topRightPoint, Node *bottomLeftPoint, int robot_zones_count, int buffer_zones_count);
  void add_forbidden_zones(Graph *graph);
  void add_edge(Node *a, Node *b);
  void remove_edge(Node *a, Node *b);
  Position get_relative_position(Node *a, Node *b);
  Position get_is_waypoint(Node *node);
  Position get_node_position(int x, int y);
  int get_edge_cost(Node *a, Node *b);
  bool get_is_taut(Node *a, Node *b);
  bool get_are_taut_waypoints(Node *a, Node *b);
  void find_waypoints();
  void add_neighbor_waypoints();
  void insert_waypoint(Node *node);
  void remove_waypoint(Node *node);
  int get_h_cost(Node *currentNode, Node *destination);
  std::vector<Node *> *reconstruct_path(Node *currentNode, std::map<Node *, Node *> cameFrom);

  ForbiddenType get_forbidden_type(int loc, int start, int end, int robot_zones_count, int buffer_zones_count);
  ForbiddenType get_forbidden_type(int x, int y, int start, int end, int robot_zones_count, int buffer_zones_count);
};

// // Remove redundant this->
// // Standarize between node, currentNode, a, b
// // do constructors and deconstructors need to be virtual?
// // pass heavy stuff around with pointers or by reference

// #pragma once
// #include <vector>
// #include <map>
// #include "Node.h"

// class Graph
// {
// public:
//   const int X_NODES_COUNT;
//   const int Y_NODES_COUNT;
//   const double NODE_SIZE;
//   std::set<Node *> *const waypoints;
//   Position get_node_position(int x, int y);

//   Graph(int X_NODES_COUNT, int Y_NODES_COUNT, double NODE_SIZE);
//   ~Graph();
//   Node *get_node(int x, int y);
//   void add_forbidden_node(Node *node);
//   std::vector<Node *> *get_forbidden_nodes();
//   virtual void forbid_triangle(Node *a, Node *b);
//   virtual void forbid_rectangle(Node *topLeftPoint, Node *topRightPoint, Node *bottomLeftPoint);
//   void add_edge(Node *a, Node *b);
//   void remove_edge(Node *a, Node *b);
//   void add_forbidden_zones(Graph *graph);

// private:
//   Node ***const nodes;
//   std::vector<Node *> *const forbidden_nodes;
//   std::vector<Node *> *path_nodes;
//   std::vector<std::vector<Node *> *> *snapshots;

//   Position get_relative_position(Node *a, Node *b);
//   int get_edge_cost(Node *a, Node *b);
//   std::vector<Node *> *reconstruct_path(Node *currentNode, std::map<Node *, Node *> cameFrom);

// private:
//   Position isWaypoint(Node *node);
//   bool isTaut(Node *a, Node *b);
//   bool areTautWaypoints(Node *a, Node *b);
//   void findWaypoints();
//   void addNeighboringWaypoints();
//   void removeWaypoint(Node *node);
//   void insertWaypoint(Node *node);
//   std::set<Node *> *getWaypoints();

//   int getHCost(Node *currentNode, Node *destination);

// public:
//   // std::set<Node *> *const waypoints;
//   bool check_LOS(Node *a, Node *b);

//   std::vector<Node *> *get_path(Node *origin, Node *destination);
//   std::vector<Node *> *get_random_path();
//   std::vector<std::vector<Node *> *> *get_path_snapshots(Node *origin, Node *destination);
//   std::vector<std::vector<Node *> *> *get_random_path_snapshots();
// };