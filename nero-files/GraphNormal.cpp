#include <cmath>
#include <iostream>
#include <time.h>
#include <random>
#include <stdexcept>
#include <cmath>
#include "Graph.h"
#include "GraphNormal.h"

GraphNormal::GraphNormal(int X_NODES_COUNT, int Y_NODES_COUNT, double NODE_SIZE) : Graph::Graph(X_NODES_COUNT, Y_NODES_COUNT, NODE_SIZE, GraphType::NORMAL)
{
}

std::vector<Node *> *GraphNormal::get_path(Node *origin, Node *destination)
{
    std::set<Node *> frontier;
    std::set<Node *> closed;
    std::map<Node *, Node *> cameFrom;
    std::map<Node *, int> gScores;
    std::map<Node *, int> fScores;

    frontier.insert(origin);
    cameFrom[origin] = origin;
    gScores[origin] = 0;
    fScores[origin] = getHCost(origin, destination);

    while (frontier.size() > 0)
    {
        int lowestFScore = 2147483647;
        int lowestHScore = 2147483647;
        int lowestGScore = 2147483647;
        int highestGScore = 0;
        Node *currentNode;

        for (Node *node : frontier)
        {
            // if (fScores[node] < lowestFScore)
            // {
            //   lowestFScore = fScores[node];
            //   currentNode = node;
            // }
            // else if (fScores[node] == lowestFScore && getHCost(node, destination) < getHCost(currentNode, destination))
            // {
            //   currentNode = node;
            // }

            if (getHCost(node, destination) < lowestHScore)
            {
                lowestHScore = getHCost(node, destination);
                currentNode = node;
            }
            else if (getHCost(node, destination) == lowestHScore && gScores[node] < lowestGScore)
            {
                lowestGScore = gScores[node];
                currentNode = node;
            }
        }

        frontier.erase(currentNode);

        if (currentNode == destination)
        {
            return reconstruct_path(currentNode, cameFrom);
        }

        // for (Node *neighbor : currentNode->neighbors)
        // {
        //   int neighborGScore = gScores[currentNode] + get_edge_cost(currentNode, neighbor);

        //   if (gScores.find(neighbor) == gScores.end() || neighborGScore < gScores[neighbor])
        //   {
        //     cameFrom[neighbor] = currentNode;
        //     gScores[neighbor] = neighborGScore;
        //     fScores[neighbor] = neighborGScore + getHCost(neighbor, destination);

        //     if (frontier.find(neighbor) == frontier.end())
        //     {
        //       frontier.insert(neighbor);
        //     }
        //   }
        // }
        for (Node *neighbor : (*currentNode->get_neighbors()))
        {
            if (closed.find(neighbor) == closed.end())
            {
                int neighborGScore = gScores[currentNode] + get_edge_cost(currentNode, neighbor);

                if (gScores.find(neighbor) == gScores.end() || neighborGScore < gScores[neighbor])
                {
                    cameFrom[neighbor] = currentNode;
                    gScores[neighbor] = neighborGScore;
                    fScores[neighbor] = neighborGScore + getHCost(neighbor, destination);

                    if (closed.find(neighbor) == closed.end() && frontier.find(neighbor) == frontier.end())
                    {
                        frontier.insert(neighbor);
                    }
                }
            }
        }

        // Remove this?
        closed.insert(currentNode);
    }
    throw std::runtime_error("ERROR 002");
}

std::vector<Node *> *GraphNormal::get_random_path()
{
    srand(time(0));

    // std::random_device rd;
    std::mt19937 rng(rand());
    std::uniform_int_distribution<int> randX(0, X_NODES_COUNT - 1);
    std::uniform_int_distribution<int> randY(0, Y_NODES_COUNT - 1);

    int originX;
    int originY;
    int destinationX;
    int destinationY;

    do
    {
        originX = randX(rng);
        originY = randY(rng);
        destinationX = randX(rng);
        destinationY = randY(rng);
    } while (nodes[originY][originX]->get_is_forbidden() || nodes[destinationY][destinationX]->get_is_forbidden());

    return get_path(nodes[originY][originX], nodes[destinationY][destinationX]);
}

std::vector<std::vector<Node *> *> *GraphNormal::get_path_snapshots(Node *origin, Node *destination)
{

    // for (std::vector<Node *> *vec : (*snapshots))
    // {
    //     delete vec;
    // }
    // snapshots->clear();

    std::set<Node *> frontier;
    std::set<Node *> closed;
    std::map<Node *, Node *> cameFrom;
    std::map<Node *, int> gScores;
    std::map<Node *, int> fScores;
    // std::vector<std::vector<Node *> *> *snapshots;
    snapshots = new std::vector<std::vector<Node *> *>;

    frontier.insert(origin);
    cameFrom[origin] = origin;
    gScores[origin] = 0;
    fScores[origin] = getHCost(origin, destination);

    while (frontier.size() > 0)
    {
        snapshots->push_back(new std::vector<Node *>);
        int lowestFScore = 2147483647;
        int lowestHScore = 2147483647;
        int lowestGScore = 2147483647;
        int highestGScore = 0;
        Node *currentNode;

        for (Node *node : frontier)
        {
            if (getHCost(node, destination) < lowestHScore)
            {
                lowestHScore = getHCost(node, destination);
                currentNode = node;
            }
            else if (getHCost(node, destination) == lowestHScore && gScores[node] < lowestGScore)
            {
                lowestGScore = gScores[node];
                currentNode = node;
            }
        }

        Node *temp = currentNode;
        snapshots->back()->push_back(temp);
        while (temp != cameFrom[temp])
        {
            snapshots->back()->push_back(cameFrom[temp]);
            temp = cameFrom[temp];
        }

        frontier.erase(currentNode);

        if (currentNode == destination)
        {
            // return reconstruct_path(currentNode, cameFrom);
            return snapshots;
        }

        for (Node *neighbor : (*currentNode->get_neighbors()))
        {
            if (closed.find(neighbor) == closed.end())
            {
                int neighborGScore = gScores[currentNode] + get_edge_cost(currentNode, neighbor);

                if (gScores.find(neighbor) == gScores.end() || neighborGScore < gScores[neighbor])
                {
                    cameFrom[neighbor] = currentNode;
                    gScores[neighbor] = neighborGScore;
                    fScores[neighbor] = neighborGScore + getHCost(neighbor, destination);

                    if (closed.find(neighbor) == closed.end() && frontier.find(neighbor) == frontier.end())
                    {
                        frontier.insert(neighbor);
                    }
                }
            }
        }

        // Remove this?
        closed.insert(currentNode);
    }
    throw std::runtime_error("ERROR 100");
}

std::vector<std::vector<Node *> *> *GraphNormal::get_random_path_snapshots()
{
    srand(time(0));

    // std::random_device rd;
    std::mt19937 rng(rand());
    std::uniform_int_distribution<int> randX(0, this->X_NODES_COUNT - 1);
    std::uniform_int_distribution<int> randY(0, this->Y_NODES_COUNT - 1);

    int originX;
    int originY;
    int destinationX;
    int destinationY;

    do
    {
        originX = randX(rng);
        originY = randY(rng);
        destinationX = randX(rng);
        destinationY = randY(rng);

    } while (nodes[originY][originX]->get_is_forbidden() || nodes[destinationY][destinationX]->get_is_forbidden());

    return get_path_snapshots(nodes[originY][originX], nodes[destinationY][destinationX]);
}

// Octile distance
int GraphNormal::getHCost(Node *currentNode, Node *destination)
{
    int dx = abs(currentNode->x - destination->x);
    int dy = abs(currentNode->y - destination->y);

    if (dx > dy)
    {
        return 14 * dy + 10 * (dx - dy);
    }
    else
    {
        return 14 * dx + 10 * (dy - dx);
    }

    // return 10 * (dx + dy) + (14 - 2 * 10) * std::min(dx, dy);
}

// Manhattan distance
// int GraphNormal::getHCost(Node *currentNode, Node *destination)
// {
//   int dx = abs(currentNode->x - destination->x);
//   int dy = abs(currentNode->y - destination->y);

//   return 10 * (dx + dy);
//   // return 10 * abs(currentNode->x - destination->x) + 10 * abs(currentNode->y - destination->y);
// }

// Euclidean distance
// int GraphNormal::getHCost(Node *currentNode, Node *destination)
// {
//     int x1 = currentNode->x;
//     int y1 = currentNode->y;
//     int x2 = destination->x;
//     int y2 = destination->y;
//     // return std::sqrt(std::pow(x2 - x1, 2) * 10 + std::pow(y2 - y1, 2) * 10);
//     return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
// }
