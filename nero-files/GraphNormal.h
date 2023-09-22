#pragma once
#include "Graph.h"

class GraphNormal : public Graph
{
private:
    int getHCost(Node *currentNode, Node *destination);

public:
    // int xNodes;
    // int yNodes;

    GraphNormal(int xNodes, int yNodes);
    // GraphNormal(int xNodes, int yNodes) : Graph(xNodes, yNodes){};

    std::vector<Node *> getPath(Node *origin, Node *destination);
    std::vector<Node *> getRandomPath();
    std::vector<std::vector<Node *> *> getPathSnapshots(Node *origin, Node *destination);
    std::vector<std::vector<Node *> *> getRandomPathSnapshots();
    // void forbidTriangle(Node *a, Node *b);
    // void forbidRectangle(Node *topLeftPoint, Node *topRightPoint, Node *bottomLeftPoint);
};
