#pragma once
#include <vector>
#include <map>
#include <cstdlib>
#include <random>
#include "Node.h"

class Graph
{
private:
  Node ***nodes;
  // Node ***waypoints;
  std::vector<Node *> waypoints;
  Node ***VG;

  int heuristic(Node *currentNode, Node *destination);
  std::vector<Node *> reconstructPath(Node *currentNode, std::map<Node *, Node *> cameFrom);
  // std::vector<std::vector<Node *> *> reconstructPath(Node *currentNode, std::map<Node *, Node *> cameFrom);
  int getEdgeCost(Node *a, Node *b);
  void reset();

public:
  int xNodes;
  int yNodes;

  Graph(int xNodes, int yNodes);
  Node *getNode(int x, int y);
  std::vector<Node *> getForbiddenNodes();
  std::vector<Node *> getPath(Node *origin, Node *destination);
  std::vector<Node *> getRandomPath();
  std::vector<std::vector<Node *> *> getRandomPathSnapshots();
  std::vector<std::vector<Node *> *> getPathSnapshots(Node *origin, Node *destination);
  std::vector<Node *> getVisibleNodes(Node *node);
  bool LOS(Node *a, Node *b);
  bool shouldKeepNode(Node *a);
  int getPositionRelative(Node *a, Node *b);
  bool isCornerNode(Node *node);
  void ENLSVG(int xNodes, int yNodes);
  std::set<Node *> getTautVisibleWaypoints(Node *node);
};

// Edge N-Level Sparse Visibility Graph
// 1. Mark corner nodes as waypoints
// 2. For each node, get its visible neighbors through LOS scan
//     * Only if a can see b and b can see a should you add them as neighbors
// 3. Prune edges

// Edge weight?
// Taut A*?
// all direction vs taut points line of sight scans
