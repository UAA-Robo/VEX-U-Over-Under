#pragma once
#include "Graph.h"

class GraphNormal : public Graph
{
private:
    int getHCost(Node *currentNode, Node *destination);

public:
    GraphNormal(int X_NODES_COUNT, int Y_NODES_COUNT, double NODE_SIZE);

    std::vector<Node *> get_path(Node *origin, Node *destination);
    std::vector<Node *> get_random_path();
    std::vector<std::vector<Node *> *> get_path_snapshots(Node *origin, Node *destination);
    std::vector<std::vector<Node *> *> get_random_path_snapshots();
};
