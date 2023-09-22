#include "GraphVG.h"
#include <time.h>
#include <random>
#include <iostream>
#include <iostream>
#include <time.h>
#include <random>
#include <stdexcept>
#include <cmath>

void GraphVG::createVG()
{
    findWaypoints();
    addNeighboringWaypoints();
}

void GraphVG::insertWaypoint(Node *node)
{

    for (Node *waypoint : waypoints)
    {
        // std::cout << node->forbidden << "\n";
        // std::cout << waypoint->forbidden << "\n";
        bool cond = hasLOS(node, waypoint);

        if (cond)
        {
            node->addWaypointNeighbor(waypoint);
        }
    }

    waypoints.insert(node);
}

void GraphVG::removeWaypoint(Node *node)
{
    for (Node *waypointNeighbor : node->waypointNeighbors)
    {
        waypointNeighbor->waypointNeighbors.erase(node);
        node->waypointNeighbors.erase(waypointNeighbor);
    }
    waypoints.erase(node);
}

std::vector<Node *> GraphVG::getVGPath(Node *origin, Node *destination)
{
    std::set<Node *> frontier;
    std::set<Node *> closed;
    std::map<Node *, Node *> cameFrom;
    std::map<Node *, int> gScores;
    std::map<Node *, int> fScores;

    frontier.insert(origin);
    cameFrom[origin] = origin;
    gScores[origin] = 0;
    fScores[origin] = this->getEdgeCost(origin, destination);

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
            // else if (fScores[node] == lowestFScore && heuristic(node, destination) < heuristic(currentNode, destination))
            // {
            //   currentNode = node;
            // }

            if (heuristic(node, destination) < lowestHScore)
            {
                lowestHScore = heuristic(node, destination);
                currentNode = node;
            }
            else if (heuristic(node, destination) == lowestHScore && gScores[node] < lowestGScore)
            {
                lowestGScore = gScores[node];
                currentNode = node;
            }
        }

        frontier.erase(currentNode);

        if (currentNode == destination)
        {
            return reconstructPath(currentNode, cameFrom);
        }

        // for (Node *neighbor : currentNode->neighbors)
        // {
        //   int neighborGScore = gScores[currentNode] + getEdgeCost(currentNode, neighbor);

        //   if (gScores.find(neighbor) == gScores.end() || neighborGScore < gScores[neighbor])
        //   {
        //     cameFrom[neighbor] = currentNode;
        //     gScores[neighbor] = neighborGScore;
        //     fScores[neighbor] = neighborGScore + this->getEdgeCost(neighbor, destination);

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
                int neighborGScore = gScores[currentNode] + this->getEdgeCost(currentNode, neighbor);

                if (gScores.find(neighbor) == gScores.end() || neighborGScore < gScores[neighbor])
                {
                    cameFrom[neighbor] = currentNode;
                    gScores[neighbor] = neighborGScore;
                    fScores[neighbor] = neighborGScore + this->getEdgeCost(neighbor, destination);

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

std::vector<std::vector<Node *> *> GraphVG::getVGPathSnapshots(Node *origin, Node *destination)
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
    fScores[origin] = this->getEdgeCost(origin, destination);

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
            // else if (fScores[node] == lowestFScore && heuristic(node, destination) < heuristic(currentNode, destination))
            // {
            //   currentNode = node;
            // }

            if (heuristic(node, destination) < lowestHScore)
            {
                lowestHScore = heuristic(node, destination);
                currentNode = node;
            }
            else if (heuristic(node, destination) == lowestHScore && gScores[node] < lowestGScore)
            {
                lowestGScore = gScores[node];
                currentNode = node;
            }
        }

        // std::cout << "\n\nSNAPSHOT #" << snapshots.size() << "\n";
        // for (int y = 0; y < yNodes; y++)
        // {
        //   for (int x = 0; x < xNodes; x++)
        //   {
        //     Node *node = nodes[y][x];
        //     std::cout << "(" << node->x << ", " << node->y << ") "
        //               << "F-" << fScores[node] << " H-" << this->getEdgeCost(node, destination) << "   ";
        //   }

        //   std::cout << "\n";
        // }

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
                int neighborGScore = gScores[currentNode] + this->getEdgeCost(currentNode, neighbor);

                if (gScores.find(neighbor) == gScores.end() || neighborGScore < gScores[neighbor])
                {
                    cameFrom[neighbor] = currentNode;
                    gScores[neighbor] = neighborGScore;
                    fScores[neighbor] = neighborGScore + this->getEdgeCost(neighbor, destination);

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

std::vector<Node *> GraphVG::getVGRandomPath()
{
    srand(time(0));

    // std::random_device rd;
    std::mt19937 rng(rand());
    std::uniform_int_distribution<int> randX(0, this->xNodes - 1);
    std::uniform_int_distribution<int> randY(0, this->yNodes - 1);

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
    } while (this->nodes[originY][originX]->forbidden || this->nodes[destinationY][destinationX]->forbidden || (originX == destinationX && originY == destinationY));

    if (originX == destinationX && originY == destinationY)
    {
        throw std::runtime_error("ERROR SAME ORIGIN AND DESTINATION");
    }

    return getVGPath(this->nodes[originY][originX], this->nodes[destinationY][destinationX]);
}

std::vector<std::vector<Node *> *> GraphVG::getVGRandomPathSnapshots()
{
    srand(time(0));

    // std::random_device rd;
    std::mt19937 rng(rand());
    std::uniform_int_distribution<int> randX(0, this->xNodes - 1);
    std::uniform_int_distribution<int> randY(0, this->yNodes - 1);

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
    } while (this->nodes[originY][originX]->forbidden || this->nodes[destinationY][destinationX]->forbidden || (originX == destinationX && originY == destinationY));

    if (originX == destinationX && originY == destinationY)
    {
        throw std::runtime_error("ERROR SAME ORIGIN AND DESTINATION");
    }

    // this->nodes[originY][originX]->println();
    // this->nodes[destinationY][destinationX]->println();

    return getVGPathSnapshots(this->nodes[originY][originX], this->nodes[destinationY][destinationX]);
}

std::set<Node *> GraphVG::getWaypoints()
{

    // std::set<Node *>
    // for(Node* waypoint: waypoints) {

    // }

    return waypoints;
}

// can store this data on the node itself
// is a waypoint if:
// * is not forbidden
// * top, left, bottom, right are not forbidden
// * topleft topright, topright bottomright, bottomright bottomleft, bottomleft topleft are not forbidden
// * topleft, topright, bottomright, or bottomleft is forbidden
WAYPOINT GraphVG::isWaypoint(Node *node)
{
    int x = node->x;
    int y = node->y;

    bool hasTopNeighbor = y - 1 >= 0;
    bool hasBottomNeighbor = y + 1 <= this->yNodes - 1;
    bool hasLeftNeighbor = x - 1 >= 0;
    bool hasRightNeighbor = x + 1 <= this->xNodes - 1;

    bool hasTopLeftNeighbor = hasTopNeighbor && hasLeftNeighbor;
    bool hasTopRightNeighbor = hasTopNeighbor && hasRightNeighbor;
    bool hasBottomLeftNeighbor = hasBottomNeighbor && hasLeftNeighbor;
    bool hasBottomRightNeighbor = hasBottomNeighbor && hasRightNeighbor;

    // bool topNeighborForbidden = hasTopNeighbor && this->nodes[y - 1][x];
    // bool rightNeighborForbidden = hasTopNeighbor && this->nodes[y][x + 1];
    // bool bottomNeighborForbidden = hasTopNeighbor && this->nodes[y + 1][x];
    // bool leftNeighborForbidden = hasTopNeighbor && this->nodes[y][x - 1];

    bool topNeighborForbidden = hasTopNeighbor && this->nodes[y - 1][x]->forbidden;
    bool rightNeighborForbidden = hasRightNeighbor && this->nodes[y][x + 1]->forbidden;
    bool bottomNeighborForbidden = hasBottomNeighbor && this->nodes[y + 1][x]->forbidden;
    bool leftNeighborForbidden = hasLeftNeighbor && this->nodes[y][x - 1]->forbidden;

    bool topLeftNeighborForbidden = hasTopLeftNeighbor && this->nodes[y - 1][x - 1]->forbidden;
    bool topRightNeighborForbidden = hasTopRightNeighbor && this->nodes[y - 1][x + 1]->forbidden;
    bool bottomRightNeighborForbidden = hasBottomRightNeighbor && this->nodes[y + 1][x + 1]->forbidden;
    bool bottomLeftNeighborForbidden = hasBottomLeftNeighbor && this->nodes[y + 1][x - 1]->forbidden;

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
        std::cout << topNeighborForbidden << std::endl;
        std::cout << rightNeighborForbidden << std::endl;
        std::cout << bottomNeighborForbidden << std::endl;
        std::cout << leftNeighborForbidden << std::endl;

        std::cout << topLeftNeighborForbidden << std::endl;
        std::cout << topRightNeighborForbidden << std::endl;
        std::cout << bottomRightNeighborForbidden << std::endl;
        std::cout << bottomLeftNeighborForbidden << std::endl;

        node->println();

        throw std::runtime_error("ERROR 110");
    }

    if ((topLeftNeighborForbidden && topRightNeighborForbidden) ||
        (topRightNeighborForbidden && bottomRightNeighborForbidden) ||
        (bottomRightNeighborForbidden && bottomLeftNeighborForbidden) ||
        (bottomLeftNeighborForbidden && topLeftNeighborForbidden))
    {
        if (!topNeighborForbidden && !rightNeighborForbidden && !bottomNeighborForbidden && !leftNeighborForbidden)
        {
            std::cout << topLeftNeighborForbidden << std::endl;
            std::cout << topRightNeighborForbidden << std::endl;
            std::cout << bottomRightNeighborForbidden << std::endl;
            std::cout << bottomLeftNeighborForbidden << std::endl;
            node->println();
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
    // node->println();
    // throw std::runtime_error("ERROR 300");
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

bool GraphVG::hasLOS(Node *a, Node *b)
{

    if (a == b)
    {
        return true;
    }

    int x = a->x;
    int y = a->y;
    double dx = b->x - a->x;
    double dy = b->y - a->y;
    double xSlope;
    double ySlope;

    if (dy == 0)
    {
        xSlope = dx;
    }
    else
    {
        xSlope = dx / dy;
    }

    if (dx == 0)
    {
        ySlope = dy;
    }
    else
    {
        ySlope = dy / dx;
    }

    // double xSlope = abs(double((double(b->x - a->x) / double(b->y - a->y)) / 10.0));
    // double xSlope = abs(double(double(b->x - a->x) / double(b->y - a->y)) / 10.0);
    // double ySlope = abs(double((double(b->y - a->y) / double(b->x - a->x)) / 10.0));
    // double xSlope = (b->x - a->x) / (b->y - a->y);
    // double ySlope = (b->y - a->y) / (b->x - a->x);

    // std::cout << a->x << ", " << a->y << "\n";
    // std::cout << b->x << ", " << b->y << "\n";
    // std::cout << double(b->x - a->x) << ", " << double(b->y - a->y) << "\n";
    // std::cout << double(b->x - a->x) / double(b->y - a->y) << "\n";
    // std::cout << abs(double(double(b->x - a->x) / double(b->y - a->y)) / 10.0) << "\n";

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

            if (nodes[y][x]->forbidden)
            {
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

            if (nodes[y][x]->forbidden)
            {
                return false;
            }
        }
        break;
    }
    case B_BOTTOMRIGHT_A:
    case B_BOTTOM_A:
    case B_RIGHT_A:
    {
        while (y + ySlope <= b->y && x + xSlope <= b->x)
        {
            y += ySlope;
            x += xSlope;

            if (nodes[y][x]->forbidden)
            {
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

            if (nodes[y][x]->forbidden)
            {
                return false;
            }
        }
        break;
    }
    default:
        std::cout << getPositionRelative(a, b);
        throw std::runtime_error("9000");
        break;
    }

    return true;

    // while ((y + ySlope >= 0 && y + ySlope <= yNodes - 1) && (x + xSlope >= 0 && x + xSlope <= xNodes - 1))
    while ((y + ySlope >= 0 && y + ySlope <= yNodes - 1) && (x + xSlope >= 0 && x + xSlope <= xNodes - 1))
    {
        y += ySlope;
        x += xSlope;

        if (nodes[y][x]->forbidden)
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
                if (areTautWaypoints(nodeA, nodeB))
                {
                    nodeA->waypointNeighbors.insert(nodeB);
                    nodeB->waypointNeighbors.insert(nodeA);
                }
            }
        }
    }
}
