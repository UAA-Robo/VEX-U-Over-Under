#pragma once
#include "Graph.h"

class GraphVG : public Graph
{
private:
    std::set<Node *> waypoints;
    
    WAYPOINT isWaypoint(Node *node);
    bool isTaut(Node *a, Node *b);
    bool areTautWaypoints(Node *a, Node *b);
    void findWaypoints();
    void addNeighboringWaypoints();
    bool hasLOS(Node *a, Node *b);
    void createVG();
    std::vector<Node *> getVGPath(Node *origin, Node *destination);
    std::vector<Node *> getVGRandomPath();
    std::vector<std::vector<Node *> *> getVGPathSnapshots(Node *origin, Node *destination);
    std::vector<std::vector<Node *> *> getVGRandomPathSnapshots();
    void removeWaypoint(Node *node);
    void insertWaypoint(Node *node);
    std::set<Node *> getWaypoints();

public:
};