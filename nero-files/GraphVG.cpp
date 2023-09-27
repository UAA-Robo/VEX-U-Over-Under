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

GraphVG::GraphVG(int X_NODES_COUNT, int Y_NODES_COUNT, double NODE_SIZE) : Graph::Graph(X_NODES_COUNT, Y_NODES_COUNT, NODE_SIZE)
{
    type = GraphType::VG;
    create_VG();
}

void GraphVG::create_VG()
{
    findWaypoints();
    addNeighboringWaypoints();
}

void GraphVG::insertWaypoint(Node *node)
{
    for (Node *waypoint : waypoints)
    {
        bool cond = check_LOS(node, waypoint);

        if (cond)
        {
            node->add_waypoint_neighbor(waypoint);
            waypoint->add_waypoint_neighbor(node);
        }
    }

    waypoints.insert(node);
}

// TODO - IMPORTANT - editing list/set as you're iterating over it, look for it elsewhere
void GraphVG::removeWaypoint(Node *node)
{

    for (Node *waypoint_neighbor : (*node->get_waypoint_neighbors()))
    {
        node->remove_waypoint_neighbor(waypoint_neighbor);
        waypoint_neighbor->remove_waypoint_neighbor(node);
    }

    // std::set<Node *> toBeRemoved;
    // int x = node->get_waypoint_neighbors()->size();

    // for (Node *waypointNeighbor : (*node->get_waypoint_neighbors()))
    // {
    //     toBeRemoved.insert(waypointNeighbor);
    //     // waypointNeighbor->get_waypoint_neighbors().erase(node);
    //     // node->get_waypoint_neighbors()->erase(waypointNeighbor);
    // }

    // for (Node *waypointNeighbor : toBeRemoved)
    // {
    //     waypointNeighbor->get_waypoint_neighbors().erase(node);
    //     node->get_waypoint_neighbors()->erase(waypointNeighbor);
    // }

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

std::vector<Node *> GraphVG::get_path(Node *origin, Node *destination)
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
            return reconstruct_path(currentNode, cameFrom);
        }

        // for (Node *neighbor : currentNode->neighbors)
        // {
        //   int neighborGScore = gScores[currentNode] + get_edge_cost(currentNode, neighbor);

        //   if (gScores.find(neighbor) == gScores.end() || neighborGScore < gScores[neighbor])
        //   {
        //     cameFrom[neighbor] = currentNode;
        //     gScores[neighbor] = neighborGScore;
        //     fScores[neighbor] = neighborGScore + get_edge_cost(neighbor, destination);

        //     if (frontier.find(neighbor) == frontier.end())
        //     {
        //       frontier.insert(neighbor);
        //     }
        //   }
        // }
        for (Node *neighbor : (*currentNode->get_waypoint_neighbors()))
        {
            if (closed.find(neighbor) == closed.end())
            {
                // int neighborGScore = gScores[currentNode] + get_edge_cost(currentNode, neighbor);
                int neighborGScore = gScores[currentNode] + get_edge_cost(currentNode, neighbor);

                if (gScores.find(neighbor) == gScores.end() || neighborGScore < gScores[neighbor])
                {
                    cameFrom[neighbor] = currentNode;
                    gScores[neighbor] = neighborGScore;
                    fScores[neighbor] = neighborGScore + get_edge_cost(neighbor, destination);

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

std::vector<std::vector<Node *> *> GraphVG::get_path_snapshots(Node *origin, Node *destination)
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
    fScores[origin] = get_edge_cost(origin, destination);

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
            // return reconstruct_path(currentNode, cameFrom);
            removeWaypoint(origin);
            removeWaypoint(destination);
            return snapshots;
        }

        for (Node *neighbor : (*currentNode->get_waypoint_neighbors()))
        {
            if (closed.find(neighbor) == closed.end())
            {
                // int neighborGScore = gScores[currentNode] + get_edge_cost(currentNode, neighbor);
                int neighborGScore = gScores[currentNode] + get_edge_cost(currentNode, neighbor);

                if (gScores.find(neighbor) == gScores.end() || neighborGScore < gScores[neighbor])
                {
                    cameFrom[neighbor] = currentNode;
                    gScores[neighbor] = neighborGScore;
                    fScores[neighbor] = neighborGScore + get_edge_cost(neighbor, destination);

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

std::vector<Node *> GraphVG::get_random_path()
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
    } while (nodes[originY][originX]->get_is_forbidden() || nodes[destinationY][destinationX]->get_is_forbidden() || (originX == destinationX && originY == destinationY));

    if (originX == destinationX && originY == destinationY)
    {
        throw std::runtime_error("ERROR SAME ORIGIN AND DESTINATION");
    }

    return get_path(nodes[originY][originX], nodes[destinationY][destinationX]);
}

std::vector<std::vector<Node *> *> GraphVG::get_random_path_snapshots()
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
    } while (nodes[originY][originX]->get_is_forbidden() || nodes[destinationY][destinationX]->get_is_forbidden() || (originX == destinationX && originY == destinationY));

    if (originX == destinationX && originY == destinationY)
    {
        throw std::runtime_error("ERROR SAME ORIGIN AND DESTINATION");
    }

    return get_path_snapshots(nodes[originY][originX], nodes[destinationY][destinationX]);
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
Waypoint GraphVG::isWaypoint(Node *node)
{
    int x = node->x;
    int y = node->y;

    bool hasTopNeighbor = y - 1 >= 0;
    bool hasBottomNeighbor = y + 1 <= Y_NODES_COUNT - 1;
    bool hasLeftNeighbor = x - 1 >= 0;
    bool hasRightNeighbor = x + 1 <= X_NODES_COUNT - 1;

    bool hasTopLeftNeighbor = hasTopNeighbor && hasLeftNeighbor;
    bool hasTopRightNeighbor = hasTopNeighbor && hasRightNeighbor;
    bool hasBottomRightNeighbor = hasBottomNeighbor && hasRightNeighbor;
    bool hasBottomLeftNeighbor = hasBottomNeighbor && hasLeftNeighbor;

    bool topNeighborForbidden = hasTopNeighbor && nodes[y - 1][x]->get_is_forbidden();
    bool rightNeighborForbidden = hasRightNeighbor && nodes[y][x + 1]->get_is_forbidden();
    bool bottomNeighborForbidden = hasBottomNeighbor && nodes[y + 1][x]->get_is_forbidden();
    bool leftNeighborForbidden = hasLeftNeighbor && nodes[y][x - 1]->get_is_forbidden();

    bool topLeftNeighborForbidden = hasTopLeftNeighbor && nodes[y - 1][x - 1]->get_is_forbidden();
    bool topRightNeighborForbidden = hasTopRightNeighbor && nodes[y - 1][x + 1]->get_is_forbidden();
    bool bottomRightNeighborForbidden = hasBottomRightNeighbor && nodes[y + 1][x + 1]->get_is_forbidden();
    bool bottomLeftNeighborForbidden = hasBottomLeftNeighbor && nodes[y + 1][x - 1]->get_is_forbidden();

    // if ((!node->get_is_forbidden()) &&
    //     (!topNeighborForbidden && !rightNeighborForbidden && !bottomNeighborForbidden && !leftNeighborForbidden) &&
    //     ((!(topLeftNeighborForbidden && topRightNeighborForbidden) && !(topRightNeighborForbidden && bottomRightNeighborForbidden) && !(bottomRightNeighborForbidden && bottomLeftNeighborForbidden) && !(bottomLeftNeighborForbidden && topLeftNeighborForbidden))))
    // {
    if ((!node->get_is_forbidden()) &&
        (!topNeighborForbidden && !rightNeighborForbidden && !bottomNeighborForbidden && !leftNeighborForbidden) &&
        ((!(topLeftNeighborForbidden && topRightNeighborForbidden) && !(topRightNeighborForbidden && bottomRightNeighborForbidden) && !(bottomRightNeighborForbidden && bottomLeftNeighborForbidden) && !(bottomLeftNeighborForbidden && topLeftNeighborForbidden))))
    {
        if (topLeftNeighborForbidden)
        {
            return Waypoint::BOTTOM_RIGHT;
        }
        else if (topRightNeighborForbidden)
        {
            return Waypoint::BOTTOM_LEFT;
        }
        else if (bottomRightNeighborForbidden)
        {
            return Waypoint::TOP_LEFT;
        }
        else if (bottomLeftNeighborForbidden)
        {
            return Waypoint::TOP_RIGHT;
        }
    }
    else
    {
        return Waypoint::NONE;
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
        return Waypoint::NONE;
    }
    else if (bottomRightNeighborForbidden)
    {
        return Waypoint::TOP_LEFT;
    }
    else if (bottomLeftNeighborForbidden)
    {
        return Waypoint::TOP_RIGHT;
    }
    if (topLeftNeighborForbidden)
    {
        return Waypoint::BOTTOM_RIGHT;
    }
    else if (topRightNeighborForbidden)
    {
        return Waypoint::BOTTOM_LEFT;
    }

    return Waypoint::NONE;
}

// Checks if b is taut to a
bool GraphVG::isTaut(Node *a, Node *b)
{
    switch (a->get_waypoint())
    {
    case Waypoint::TOP_LEFT:
    {
        switch (get_relative_position(a, b))
        {
        case Position::TOP:
        case Position::RIGHT:
        case Position::BOTTOM:
        case Position::LEFT:
        case Position::TOP_RIGHT:
        case Position::BOTTOM_LEFT:
        {
            return true;
            break;
        }
        }
    }
    case Waypoint::TOP_RIGHT:
    {
        switch (get_relative_position(a, b))
        {
        case Position::TOP:
        case Position::RIGHT:
        case Position::BOTTOM:
        case Position::LEFT:
        case Position::TOP_LEFT:
        case Position::BOTTOM_RIGHT:
        {
            return true;
            break;
        }
        }
        break;
    }
    case Waypoint::BOTTOM_RIGHT:
    {
        switch (get_relative_position(a, b))
        {
        case Position::TOP:
        case Position::RIGHT:
        case Position::BOTTOM:
        case Position::LEFT:
        case Position::TOP_RIGHT:
        case Position::BOTTOM_LEFT:
        {
            return true;
            break;
        }
        }
        break;
    }
    case Waypoint::BOTTOM_LEFT:
    {
        switch (get_relative_position(a, b))
        {
        case Position::TOP:
        case Position::RIGHT:
        case Position::BOTTOM:
        case Position::LEFT:
        case Position::TOP_LEFT:
        case Position::BOTTOM_RIGHT:
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
bool GraphVG::check_LOS(Node *a, Node *b)
{

    if (a == b)
    {
        return true;
    }

    double x = a->x + (1 / 2);
    double y = a->y + (1 / 2);
    // double dx = (b->x + (NODE_SIZE / 2)) - a->x;
    // double dy = (b->y + (NODE_SIZE / 2)) - a->y;
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

    switch (get_relative_position(a, b))
    {
    case Position::TOP_LEFT:
    {
        xSlope *= (-1.0);
        ySlope *= (-1.0);
        while (y + ySlope >= b->y && x + xSlope >= b->x)
        {
            y += ySlope;
            x += xSlope;

            if (nodes[int(y)][int(x)]->get_is_forbidden())
            {
                // nodes[int(y)][int(x)]->hit = true;
                return false;
            }
        }
        break;
    }
    case Position::TOP_RIGHT:
    case Position::TOP:
    {
        ySlope *= (-1);
        while (y + ySlope >= b->y && x + xSlope <= b->x)
        {
            y += ySlope;
            x += xSlope;

            if (nodes[int(y)][int(x)]->get_is_forbidden())
            {
                // nodes[int(y)][int(x)]->hit = true;
                return false;
            }
        }
        break;
    }
    case Position::BOTTOM_RIGHT:
    case Position::BOTTOM:
    case Position::RIGHT:
        // FIX: same y, different x, xSlope = dx / (10 * dx), dx = number of nodes (+1 for inclusive?)
        {
            while (y + ySlope <= b->y && x + xSlope <= b->x)
            {
                y += ySlope;
                x += xSlope;

                if (nodes[int(y)][int(x)]->get_is_forbidden())
                {
                    // nodes[int(y)][int(x)]->hit = true;
                    return false;
                }
            }
            break;
        }
    case Position::BOTTOM_LEFT:
    case Position::LEFT:
    {
        xSlope *= (-1);
        while (y + ySlope <= b->y && x + xSlope >= b->x)
        {
            y += ySlope;
            x += xSlope;

            if (nodes[int(y)][int(x)]->get_is_forbidden())
            {
                // nodes[int(y)][int(x)]->hit = true;
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

    // while ((y + ySlope >= 0 && y + ySlope <= Y_NODES_COUNT - 1) && (x + xSlope >= 0 && x + xSlope <= X_NODES_COUNT - 1))
    while ((y + ySlope >= 0 && y + ySlope <= Y_NODES_COUNT - 1) && (x + xSlope >= 0 && x + xSlope <= X_NODES_COUNT - 1))
    {
        y += ySlope;
        x += xSlope;

        if (nodes[int(y)][int(x)]->get_is_forbidden())
        {
            return false;
        }
    }

    return true;
}

// Before doing a line of check, make sure they are taut waypoints
void GraphVG::findWaypoints()
{
    for (int y = 0; y < Y_NODES_COUNT; y++)
    {
        for (int x = 0; x < X_NODES_COUNT; x++)
        {
            Node *node = nodes[y][x];

            if (node->get_is_forbidden())
            {
                continue;
            }

            Waypoint corner = isWaypoint(node);
            node->set_waypoint(corner);

            switch (corner)
            {
            case Waypoint::NONE:
            {
                break;
            }
            case Waypoint::TOP_LEFT:
            case Waypoint::TOP_RIGHT:
            case Waypoint::BOTTOM_RIGHT:
            case Waypoint::BOTTOM_LEFT:
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
                if (areTautWaypoints(nodeA, nodeB) && check_LOS(nodeA, nodeB))
                {
                    nodeA->add_waypoint_neighbor(nodeB);
                    nodeB->add_waypoint_neighbor(nodeA);
                    // nodeA->get_waypoint_neighbors().insert(nodeB);
                    // nodeB->get_waypoint_neighbors().insert(nodeA);
                }
            }
        }
    }
}
