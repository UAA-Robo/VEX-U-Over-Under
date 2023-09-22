#pragma once
#include "Graph.h"

class GraphVG : public Graph
{
private:
    WAYPOINT isWaypoint(Node *node);
    bool isTaut(Node *a, Node *b);
    bool areTautWaypoints(Node *a, Node *b);
    void findWaypoints();
    void addNeighboringWaypoints();
    void removeWaypoint(Node *node);
    void insertWaypoint(Node *node);
    std::set<Node *> getWaypoints();

    int getHCost(Node *currentNode, Node *destination);

public:
    std::set<Node *> waypoints;
    GraphVG(int xNodes, int yNodes, double cellSize);
    bool hasLOS(Node *a, Node *b);
    void createVG();

    std::vector<Node *> getPath(Node *origin, Node *destination);
    std::vector<Node *> getRandomPath();
    std::vector<std::vector<Node *> *> getPathSnapshots(Node *origin, Node *destination);
    std::vector<std::vector<Node *> *> getRandomPathSnapshots();
};