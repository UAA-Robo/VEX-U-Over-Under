#pragma once
#include "Graph.h"

class GraphNormal : public Graph
{
private:
    int getHCost(Node *currentNode, Node *destination);

public:
    GraphNormal(int xNodes, int yNodes, double cellSize);

    std::vector<Node *> getPath(Node *origin, Node *destination);
    std::vector<Node *> getRandomPath();
    std::vector<std::vector<Node *> *> getPathSnapshots(Node *origin, Node *destination);
    std::vector<std::vector<Node *> *> getRandomPathSnapshots();
};
