#include "GraphVG.h"
#include <time.h>
#include <random>
#include <iostream>
#include <iostream>
#include <time.h>
#include <random>
#include <stdexcept>
#include <cmath>
#include "Graph.h"
#include "Enums.h"

GraphVG::GraphVG(int xNodes, int yNodes, double cellSize) : Graph::Graph(xNodes, yNodes, cellSize)
{
}

void GraphVG::clearHits()
{
    for (int y = 0; y < yNodes; y++)
    {
        for (int x = 0; x < xNodes; x++)
        {
            nodes[y][x]->hit = false;
        }
    }
}

void GraphVG::createVG()
{
    findWaypoints();
    addNeighboringWaypoints();
    clearHits();
}

void GraphVG::insertWaypoint(Node *node)
{

    for (Node *waypoint : waypoints)
    {
        bool cond = hasLOS(node, waypoint);

        if (cond)
        {
            node->addWaypointNeighbor(waypoint);
        }
    }

    waypoints.insert(node);
}

// TODO - IMPORTANT - editing list/set as you're iterating over it, look for it elsewhere
void GraphVG::removeWaypoint(Node *node)
{
    std::set<Node *> toBeRemoved;
    int x = node->waypointNeighbors.size();

    for (Node *waypointNeighbor : node->waypointNeighbors)
    {
        toBeRemoved.insert(waypointNeighbor);
        // waypointNeighbor->waypointNeighbors.erase(node);
        // node->waypointNeighbors.erase(waypointNeighbor);
    }

    for (Node *waypointNeighbor : toBeRemoved)
    {
        waypointNeighbor->waypointNeighbors.erase(node);
        node->waypointNeighbors.erase(waypointNeighbor);
    }

    waypoints.erase(node);
}

// Octile distance
int GraphVG::getHCost(Node *currentNode, Node *destination)
{
    int x1 = currentNode->x;
    int y1 = currentNode->y;
    int x2 = destination->x;
    int y2 = destination->y;
    // return std::sqrt(std::pow(x2 - x1, 2) * 10 + std::pow(y2 - y1, 2) * 10);
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

std::vector<Node *> GraphVG::getPath(Node *origin, Node *destination)
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

    insertWaypoint(origin);
    insertWaypoint(destination);

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

            // if (getHCost(node, destination) < lowestHScore)
            // {
            //     lowestHScore = getHCost(node, destination);
            //     currentNode = node;
            // }
            // else if (getHCost(node, destination) == lowestHScore && gScores[node] < lowestGScore)
            // {
            //     lowestGScore = gScores[node];
            //     currentNode = node;
            // }

            if (fScores[node] < lowestFScore)
            {
                lowestFScore = fScores[node];
                currentNode = node;
            }
            else if (fScores[node] == lowestFScore && getHCost(node, destination) < getHCost(currentNode, destination))
            {
                currentNode = node;
            }
        }

        frontier.erase(currentNode);

        if (currentNode == destination)
        {

            removeWaypoint(origin);
            removeWaypoint(destination);
            return reconstructPath(currentNode, cameFrom);
        }

        // for (Node *neighbor : currentNode->neighbors)
        // {
        //   int neighborGScore = gScores[currentNode] + getEdgeCost(currentNode, neighbor);

        //   if (gScores.find(neighbor) == gScores.end() || neighborGScore < gScores[neighbor])
        //   {
        //     cameFrom[neighbor] = currentNode;
        //     gScores[neighbor] = neighborGScore;
        //     fScores[neighbor] = neighborGScore + getEdgeCost(neighbor, destination);

        //     if (frontier.find(neighbor) == frontier.end())
        //     {
        //       frontier.insert(neighbor);
        //     }
        //   }
        // }
        for (Node *neighbor : currentNode->waypointNeighbors)
        {
            if (closed.find(neighbor) == closed.end())
            {
                // int neighborGScore = gScores[currentNode] + getEdgeCost(currentNode, neighbor);
                int neighborGScore = gScores[currentNode] + getEdgeCost(currentNode, neighbor);

                if (gScores.find(neighbor) == gScores.end() || neighborGScore < gScores[neighbor])
                {
                    cameFrom[neighbor] = currentNode;
                    gScores[neighbor] = neighborGScore;
                    fScores[neighbor] = neighborGScore + getEdgeCost(neighbor, destination);

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

std::vector<std::vector<Node *> *> GraphVG::getPathSnapshots(Node *origin, Node *destination)
{
    std::set<Node *> frontier;
    std::set<Node *> closed;
    std::map<Node *, Node *> cameFrom;
    std::map<Node *, int> gScores;
    std::map<Node *, int> fScores;
    std::vector<std::vector<Node *> *> snapshots;

    frontier.insert(origin);
    cameFrom[origin] = origin;
    gScores[origin] = 0;
    fScores[origin] = getEdgeCost(origin, destination);

    insertWaypoint(origin);
    insertWaypoint(destination);

    while (frontier.size() > 0)
    {
        snapshots.push_back(new std::vector<Node *>);
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


        Node *temp = currentNode;
        snapshots.back()->push_back(temp);
        while (temp != cameFrom[temp])
        {
            snapshots.back()->push_back(cameFrom[temp]);
            temp = cameFrom[temp];
        }

        frontier.erase(currentNode);

        if (currentNode == destination)
        {
            // return reconstructPath(currentNode, cameFrom);
            removeWaypoint(origin);
            removeWaypoint(destination);
            return snapshots;
        }

        for (Node *neighbor : currentNode->waypointNeighbors)
        {
            if (closed.find(neighbor) == closed.end())
            {
                // int neighborGScore = gScores[currentNode] + getEdgeCost(currentNode, neighbor);
                int neighborGScore = gScores[currentNode] + getEdgeCost(currentNode, neighbor);

                if (gScores.find(neighbor) == gScores.end() || neighborGScore < gScores[neighbor])
                {
                    cameFrom[neighbor] = currentNode;
                    gScores[neighbor] = neighborGScore;
                    fScores[neighbor] = neighborGScore + getEdgeCost(neighbor, destination);

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

std::vector<Node *> GraphVG::getRandomPath()
{
    srand(time(0));

    // std::random_device rd;
    std::mt19937 rng(rand());
    std::uniform_int_distribution<int> randX(0, xNodes - 1);
    std::uniform_int_distribution<int> randY(0, yNodes - 1);

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
    } while (nodes[originY][originX]->forbidden || nodes[destinationY][destinationX]->forbidden || (originX == destinationX && originY == destinationY));

    if (originX == destinationX && originY == destinationY)
    {
        throw std::runtime_error("ERROR SAME ORIGIN AND DESTINATION");
    }

    return getPath(nodes[originY][originX], nodes[destinationY][destinationX]);
}

std::vector<std::vector<Node *> *> GraphVG::getRandomPathSnapshots()
{
    srand(time(0));

    // std::random_device rd;
    std::mt19937 rng(rand());
    std::uniform_int_distribution<int> randX(0, xNodes - 1);
    std::uniform_int_distribution<int> randY(0, yNodes - 1);

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
    } while (nodes[originY][originX]->forbidden || nodes[destinationY][destinationX]->forbidden || (originX == destinationX && originY == destinationY));

    if (originX == destinationX && originY == destinationY)
    {
        throw std::runtime_error("ERROR SAME ORIGIN AND DESTINATION");
    }


    return getPathSnapshots(nodes[originY][originX], nodes[destinationY][destinationX]);
}

std::set<Node *> GraphVG::getWaypoints()
{


    return waypoints;
}

// can store this data on the node itself
// is a waypoint if:
// * is not forbidden
// * top, left, bottom, right are not forbidden
// * topleft topright, topright bottomright, bottomright bottomleft, bottomleft topleft are not forbidden
// * topleft, topright, bottomright, or bottomleft is forbidden
// FIX THIS, idk how
WAYPOINT GraphVG::isWaypoint(Node *node)
{
    int x = node->x;
    int y = node->y;

    bool hasTopNeighbor = y - 1 >= 0;
    bool hasBottomNeighbor = y + 1 <= yNodes - 1;
    bool hasLeftNeighbor = x - 1 >= 0;
    bool hasRightNeighbor = x + 1 <= xNodes - 1;

    bool hasTopLeftNeighbor = hasTopNeighbor && hasLeftNeighbor;
    bool hasTopRightNeighbor = hasTopNeighbor && hasRightNeighbor;
    bool hasBottomRightNeighbor = hasBottomNeighbor && hasRightNeighbor;
    bool hasBottomLeftNeighbor = hasBottomNeighbor && hasLeftNeighbor;

    bool topNeighborForbidden = hasTopNeighbor && nodes[y - 1][x]->forbidden;
    bool rightNeighborForbidden = hasRightNeighbor && nodes[y][x + 1]->forbidden;
    bool bottomNeighborForbidden = hasBottomNeighbor && nodes[y + 1][x]->forbidden;
    bool leftNeighborForbidden = hasLeftNeighbor && nodes[y][x - 1]->forbidden;

    bool topLeftNeighborForbidden = hasTopLeftNeighbor && nodes[y - 1][x - 1]->forbidden;
    bool topRightNeighborForbidden = hasTopRightNeighbor && nodes[y - 1][x + 1]->forbidden;
    bool bottomRightNeighborForbidden = hasBottomRightNeighbor && nodes[y + 1][x + 1]->forbidden;
    bool bottomLeftNeighborForbidden = hasBottomLeftNeighbor && nodes[y + 1][x - 1]->forbidden;

    // if ((!node->forbidden) &&
    //     (!topNeighborForbidden && !rightNeighborForbidden && !bottomNeighborForbidden && !leftNeighborForbidden) &&
    //     ((!(topLeftNeighborForbidden && topRightNeighborForbidden) && !(topRightNeighborForbidden && bottomRightNeighborForbidden) && !(bottomRightNeighborForbidden && bottomLeftNeighborForbidden) && !(bottomLeftNeighborForbidden && topLeftNeighborForbidden))))
    // {
    if ((!node->forbidden) &&
        (!topNeighborForbidden && !rightNeighborForbidden && !bottomNeighborForbidden && !leftNeighborForbidden) &&
        ((!(topLeftNeighborForbidden && topRightNeighborForbidden) && !(topRightNeighborForbidden && bottomRightNeighborForbidden) && !(bottomRightNeighborForbidden && bottomLeftNeighborForbidden) && !(bottomLeftNeighborForbidden && topLeftNeighborForbidden))))
    {
        if (topLeftNeighborForbidden)
        {
            return BOTTOMRIGHT;
        }
        else if (topRightNeighborForbidden)
        {
            return BOTTOMLEFT;
        }
        else if (bottomRightNeighborForbidden)
        {
            return TOPLEFT;
        }
        else if (bottomLeftNeighborForbidden)
        {
            return TOPRIGHT;
        }
    }
    else
    {
        return NO;
        throw std::runtime_error("ERROR 110");
    }

    if ((topLeftNeighborForbidden && topRightNeighborForbidden) ||
        (topRightNeighborForbidden && bottomRightNeighborForbidden) ||
        (bottomRightNeighborForbidden && bottomLeftNeighborForbidden) ||
        (bottomLeftNeighborForbidden && topLeftNeighborForbidden))
    {
        if (!topNeighborForbidden && !rightNeighborForbidden && !bottomNeighborForbidden && !leftNeighborForbidden)
        {
            throw std::runtime_error("ERROR 200");
        }
    }

    if (topNeighborForbidden ||
        bottomNeighborForbidden ||
        leftNeighborForbidden ||
        rightNeighborForbidden)
    {
        return NO;
    }
    else if (bottomRightNeighborForbidden)
    {
        return TOPLEFT;
    }
    else if (bottomLeftNeighborForbidden)
    {
        return TOPRIGHT;
    }
    if (topLeftNeighborForbidden)
    {
        return BOTTOMRIGHT;
    }
    else if (topRightNeighborForbidden)
    {
        return BOTTOMLEFT;
    }

    return NO;
}

// Checks if b is taut to a
bool GraphVG::isTaut(Node *a, Node *b)
{
    switch (a->waypoint)
    {
    case TOPLEFT:
    {
        switch (getPositionRelative(a, b))
        {
        case B_TOP_A:
        case B_RIGHT_A:
        case B_BOTTOM_A:
        case B_LEFT_A:
        case B_TOPRIGHT_A:
        case B_BOTTOMLEFT_A:
        {
            return true;
            break;
        }
        }
    }
    case TOPRIGHT:
    {
        switch (getPositionRelative(a, b))
        {
        case B_TOP_A:
        case B_RIGHT_A:
        case B_BOTTOM_A:
        case B_LEFT_A:
        case B_TOPLEFT_A:
        case B_BOTTOMRIGHT_A:
        {
            return true;
            break;
        }
        }
        break;
    }
    case BOTTOMRIGHT:
    {
        switch (getPositionRelative(a, b))
        {
        case B_TOP_A:
        case B_RIGHT_A:
        case B_BOTTOM_A:
        case B_LEFT_A:
        case B_TOPRIGHT_A:
        case B_BOTTOMLEFT_A:
        {
            return true;
            break;
        }
        }
        break;
    }
    case BOTTOMLEFT:
    {
        switch (getPositionRelative(a, b))
        {
        case B_TOP_A:
        case B_RIGHT_A:
        case B_BOTTOM_A:
        case B_LEFT_A:
        case B_TOPLEFT_A:
        case B_BOTTOMRIGHT_A:
        {
            return true;
            break;
        }
        }
        break;
    }
    }

    return false;
}

// Checks if both a and b are taut waypoints to each other
bool GraphVG::areTautWaypoints(Node *a, Node *b)
{
    if (isTaut(a, b) && isTaut(b, a))
    {
        return true;
    }
    return false;
}

// TODO - IMPORTANT - make this function check from center of a to center of b
// TODO - top, right, bottom, left trivial, no slope need, go one node at a time
bool GraphVG::hasLOS(Node *a, Node *b)
{

    if (a == b)
    {
        return true;
    }

    double x = a->x + (1 / 2);
    double y = a->y + (1 / 2);
    // double dx = (b->x + (cellSize / 2)) - a->x;
    // double dy = (b->y + (cellSize / 2)) - a->y;
    double dx = ((b->x - a->x) + (1 / 2));
    double dy = ((b->y - a->y) + (1 / 2));
    double xSlope;
    double ySlope;

    if (dy == 0)
    {
        xSlope = dx / (100 * dx);
    }
    else
    {
        xSlope = (dx / dy) / 100;
    }

    if (dx == 0)
    {
        ySlope = dy / (100 * dy);
    }
    else
    {
        ySlope = (dy / dx) / 100;
    }

    ySlope = dy / 140;
    xSlope = dx / 140;


    if (xSlope < 0)
    {
        xSlope *= (-1.0);
    }
    if (ySlope < 0)
    {
        ySlope *= (-1.0);
    }


    switch (getPositionRelative(a, b))
    {
    case B_TOPLEFT_A:
    {
        xSlope *= (-1.0);
        ySlope *= (-1.0);
        while (y + ySlope >= b->y && x + xSlope >= b->x)
        {
            y += ySlope;
            x += xSlope;

            if (nodes[int(y)][int(x)]->forbidden)
            {
                nodes[int(y)][int(x)]->hit = true;
                return false;
            }
        }
        break;
    }
    case B_TOPRIGHT_A:
    case B_TOP_A:
    {
        ySlope *= (-1);
        while (y + ySlope >= b->y && x + xSlope <= b->x)
        {
            y += ySlope;
            x += xSlope;

            if (nodes[int(y)][int(x)]->forbidden)
            {
                nodes[int(y)][int(x)]->hit = true;
                return false;
            }
        }
        break;
    }
    case B_BOTTOMRIGHT_A:
    case B_BOTTOM_A:
    case B_RIGHT_A:
        // FIX: same y, different x, xSlope = dx / (10 * dx), dx = number of nodes (+1 for inclusive?)
        {
            while (y + ySlope <= b->y && x + xSlope <= b->x)
            {
                y += ySlope;
                x += xSlope;

                if (nodes[int(y)][int(x)]->forbidden)
                {
                    nodes[int(y)][int(x)]->hit = true;
                    return false;
                }
            }
            break;
        }
    case B_BOTTOMLEFT_A:
    case B_LEFT_A:
    {
        xSlope *= (-1);
        while (y + ySlope <= b->y && x + xSlope >= b->x)
        {
            y += ySlope;
            x += xSlope;

            if (nodes[int(y)][int(x)]->forbidden)
            {
                nodes[int(y)][int(x)]->hit = true;
                return false;
            }
        }
        break;
    }
    default:
        throw std::runtime_error("9000");
        break;
    }

    return true;

    // while ((y + ySlope >= 0 && y + ySlope <= yNodes - 1) && (x + xSlope >= 0 && x + xSlope <= xNodes - 1))
    while ((y + ySlope >= 0 && y + ySlope <= yNodes - 1) && (x + xSlope >= 0 && x + xSlope <= xNodes - 1))
    {
        y += ySlope;
        x += xSlope;

        if (nodes[int(y)][int(x)]->forbidden)
        {
            return false;
        }
    }

    return true;
}

// Before doing a line of check, make sure they are taut waypoints
void GraphVG::findWaypoints()
{
    for (int y = 0; y < yNodes; y++)
    {
        for (int x = 0; x < xNodes; x++)
        {
            Node *node = nodes[y][x];

            if (node->forbidden)
            {
                continue;
            }

            WAYPOINT corner = isWaypoint(node);
            node->waypoint = corner;

            switch (corner)
            {
            case NO:
            {
                break;
            }
            case TOPLEFT:
            case TOPRIGHT:
            case BOTTOMRIGHT:
            case BOTTOMLEFT:
            {
                waypoints.insert(node);
                break;
            }
            default:
            {
                throw std::runtime_error("ERROR 700");
                break;
            }
            }
        }
    }
}

void GraphVG::addNeighboringWaypoints()
{
    for (Node *nodeA : waypoints)
    {
        for (Node *nodeB : waypoints)
        {
            if (nodeA != nodeB)
            {
                if (areTautWaypoints(nodeA, nodeB) && hasLOS(nodeA, nodeB))
                {
                    nodeA->waypointNeighbors.insert(nodeB);
                    nodeB->waypointNeighbors.insert(nodeA);
                }
            }
        }
    }
}
