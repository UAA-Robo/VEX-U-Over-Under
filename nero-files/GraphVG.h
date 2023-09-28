#pragma once
#include "Graph.h"

class GraphVG : public Graph
{
private:
    Position isWaypoint(Node *node);
    bool isTaut(Node *a, Node *b);
    bool areTautWaypoints(Node *a, Node *b);
    void findWaypoints();
    void addNeighboringWaypoints();
    void removeWaypoint(Node *node);
    void insertWaypoint(Node *node);
    std::set<Node *> *getWaypoints();

    int getHCost(Node *currentNode, Node *destination);

public:
    std::set<Node *> *const waypoints;
    GraphVG(int X_NODES_COUNT, int Y_NODES_COUNT, double NODE_SIZE);
    bool check_LOS(Node *a, Node *b);

    std::vector<Node *> *get_path(Node *origin, Node *destination);
    std::vector<Node *> *get_random_path();
    std::vector<std::vector<Node *> *> *get_path_snapshots(Node *origin, Node *destination);
    std::vector<std::vector<Node *> *> *get_random_path_snapshots();
};