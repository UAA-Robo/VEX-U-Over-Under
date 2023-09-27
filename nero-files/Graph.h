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
  GraphType type;
  int X_NODES_COUNT;
  int Y_NODES_COUNT;
  double NODE_SIZE;
  std::set<Node *> waypoints;
  virtual bool check_LOS(Node *a, Node *b){};
  virtual void create_VG(){};

  Graph(int X_NODES_COUNT, int Y_NODES_COUNT, double NODE_SIZE);
  ~Graph();
  Node *get_node(int x, int y);
  void add_forbidden_node(Node *node);
  std::vector<Node *> get_forbidden_nodes();
  virtual std::vector<Node *> get_path(Node *origin, Node *destination);
  virtual std::vector<Node *> get_random_path();
  virtual std::vector<std::vector<Node *> *> get_path_snapshots(Node *origin, Node *destination);
  virtual std::vector<std::vector<Node *> *> get_random_path_snapshots();
  virtual void forbid_triangle(Node *a, Node *b);
  virtual void forbid_rectangle(Node *topLeftPoint, Node *topRightPoint, Node *bottomLeftPoint);
  Node *getHit();

  void add_edge(Node *a, Node *b);
  void remove_edge(Node *a, Node *b);

  void add_forbidden_zones(Graph *graph);

protected:
  Node ***nodes;
  std::vector<Node *> forbidden_nodes;

  Position get_relative_position(Node *a, Node *b);
  int get_edge_cost(Node *a, Node *b);
  std::vector<Node *> reconstruct_path(Node *currentNode, std::map<Node *, Node *> cameFrom);

private:
};
