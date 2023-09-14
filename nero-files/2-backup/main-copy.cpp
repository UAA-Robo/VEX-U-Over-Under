// // TODO - IMPORTANT
// // get something working
// // * add obstacles
// // * path -> directions/instructions

// // TODO - free memory

// // visualizing the map
// // forbidden zones
// // all units in centimeters (int)
// // accounting for the robot's size by increasing the forbidden zones' sizes?
// // create and add forbidden nodes or don't add them at all?
// // not creating forbidden zones will leave holes in the graph, so you should verify that a node is not a nullptr before using it
// // or create them anyway but mark them with forbidden and check that a node is not forbidden before using it
// // |-> make sure to disconnect forbidden nodes from other nodes
// // forbidden zones' lines are staggered
// // taking angling into account
// // TODO - GPS's (0, 0) is the center of the field, yours is top left
// // TODO - resetting graph after returning path to prepare for next request
// // TODO - graph ready -> request destination -> get origin and destination (+direction origin and end) -> find and return path -> reset graph -> request again
// // TODO - threads: graph and pathfinding, GPS
// // TODO - benchmark robot's CPU
// // TODO - given 2 points, does a straight line between them cross through obstacles

// // vertices or nodes?
// // TODO - given current GPS position, facing direction, and destination GPS position, give robot instructions to reach the destination

// #include <map>
// #include <vector>
// #include <set>
// #include <climits>
// #include <cmath>
// #include <iostream>
// #include <utility>
// #include <tuple>
// #include <random>

// #include "Node.h"
// #include "Graph.h"

// using namespace std;

// // bool hasLOS(Node *origin, Node *destination)
// // {
// //   int dx = destination->x - origin->x;
// //   int dy = destination->y - origin->y;
// //   int d = 2 * dy - dx;
// //   int y = origin->y;

// //   for (int x = destination->x; x <= destination->x; x++)
// //   {
// //     // plot
// //     if (d > 0)
// //     {
// //       y++;
// //       d = d - 2 * dx;
// //     }
// //     d = d + 2 * dy;
// //   }
// // }

// // TODO - if sharp slope, may not work too well?
// // TODO - negative slope?
// bool hasLOS(Graph *graph, Node *origin, Node *destination)
// {
//   double slope_y = (destination->y - origin->y) / (destination->x - origin->x);
//   double slope_x = (destination->x - origin->x) / (destination->y - origin->y);
//   double x = origin->x;
//   double y = origin->y;

//   while ((x + slope_x <= destination->x) && (y + slope_y <= destination->y))
//   {
//     x += slope_x;
//     y += slope_y;

//     if (graph->getNode(x, y)->forbidden)
//     {
//       return false;
//     }
//   }
//   return true;
// }

// // TODO - for forbidden zones: truncate lower points and round up upper points

// // TODO - extra safety buffer nodes near forbidden zones
// int BUFFER_NODES = 5; // good enough?

// // double const FIELD_SIZE = 140.41;
// // double const FIELD_SIZE = 10;

// int const X_NODES = 50;
// int const Y_NODES = 50;
// // double const NODE_SIZE = 0.01;
// // int const X_NODES = int(FIELD_SIZE / NODE_SIZE);
// // int const Y_NODES = int(FIELD_SIZE / NODE_SIZE);

// // TODO - this is the conversion factor to METERS, not centimeters
// // double const CONVERSION_FACTOR = 0.0254;

// // double const FIELD_SIZE = 140.41;

// double const FIELD_SIZE = 138.73;
// double const ZONE_SIZE = FIELD_SIZE / 6;

// // TODO - convert these raw values into centimeters (int) before using them to determine which nodes are forbidden
// // double const ROLLER_SIZE = 23.08;
// double const ROLLER_SIZE = ZONE_SIZE;
// tuple<tuple<double, double>, tuple<double, double>, tuple<double, double>> const ROLLER_POINT = make_tuple(make_tuple(0, 0), make_tuple(23.08, 0), make_tuple(0, 23.08));
// vector<tuple<int, int>> ROLLER_RED_LEFT;
// ROLLER_RED_LEFT.push_back(make_tuple(0, 0));
// ROLLER_RED_LEFT.push_back(make_tuple(0, ROLLER_SIZE));
// ROLLER_RED_LEFT.push_back(make_tuple(ROLLER_SIZE, 0));

// vector<tuple<int, int>> ROLLER_RED_RIGHT;
// ROLLER_RED_RIGHT.push_back(make_tuple(FIELD_SIZE, 0));
// ROLLER_RED_RIGHT.push_back(make_tuple(FIELD_SIZE, ROLLER_SIZE));
// ROLLER_RED_RIGHT.push_back(make_tuple(FIELD_SIZE - ROLLER_SIZE, 0));
// vector<tuple<int, int>> ROLLER_BLUE_LEFT;
// ROLLER_BLUE_LEFT.push_back(make_tuple(0, FIELD_SIZE));
// ROLLER_BLUE_LEFT.push_back(make_tuple(0, FIELD_SIZE - ROLLER_SIZE));
// ROLLER_BLUE_LEFT.push_back(make_tuple(ROLLER_SIZE, FIELD_SIZE));
// vector<tuple<int, int>> ROLLER_BLUE_RIGHT;
// ROLLER_BLUE_RIGHT.push_back(make_tuple(FIELD_SIZE, FIELD_SIZE));
// ROLLER_BLUE_RIGHT.push_back(make_tuple(FIELD_SIZE, FIELD_SIZE - ROLLER_SIZE));
// ROLLER_BLUE_RIGHT.push_back(make_tuple(FIELD_SIZE - ROLLER_SIZE, FIELD_SIZE));
// // ROLLER_RED_LEFT   (0, 0),   (RL, 0),     (0, RL)
// // ROLLER_RED_RIGHT  (FL, 0),  (FL-RL, 0),  (FL, RL)
// // ROLLER_BLUE_RIGHT (FL, FL), (FL-RL, FL), (FL, FL-RL)
// // ROLLER_BLUE_LEFT  (0, FL),  (RL, FL),    (0, FL-RL)

// // double const GOAL_START_X = 46.64;
// // double const GOAL_START_Y = 0;
// // double const GOAL_SIZE_X = 93.77 - 46.64;
// // double const GOAL_SIZE_Y = 23.08;

// double const GOAL_START_X = 2 * ZONE_SIZE;
// double const GOAL_START_Y = 0;
// double const GOAL_SIZE_X = 2 * ZONE_SIZE;
// double const GOAL_SIZE_Y = ZONE_SIZE;
// // TODO - change width and height to y and x as appropriate
// // BLUE_GOAL (GS, 0), (GS+GL, 0), (GS+GL, GW), (GS, GW)
// // RED_GOAL  (GS, FL), (GS+GL, FL), (GS+GL, FL-GW), (GS, FL-GW)
// vector<tuple<int, int>> GOAL_RED;
// vector<tuple<int, int>> GOAL_BLUE;
// GOAL_BLUE.push_back(make_tuple(GOAL_START_X, GOAL_START_Y));
// GOAL_BLUE.push_back(make_tuple(GOAL_START_X + GOAL_SIZE_X, GOAL_START_Y));
// GOAL_BLUE.push_back(make_tuple(GOAL_START_X, GOAL_START_Y + GOAL_SIZE_Y));
// GOAL_BLUE.push_back(make_tuple(GOAL_START_X + GOAL_SIZE_X, GOAL_START_Y + GOAL_SIZE_Y));

// GOAL_RED.push_back(make_tuple(GOAL_START_X, FIELD_SIZE));
// GOAL_RED.push_back(make_tuple(GOAL_START_X + GOAL_SIZE_X, FIELD_SIZE));
// GOAL_RED.push_back(make_tuple(GOAL_START_X, FIELD_SIZE - GOAL_SIZE_Y));
// GOAL_RED.push_back(make_tuple(GOAL_START_X + GOAL_SIZE_X, FIELD_SIZE - GOAL_SIZE_Y));

// // double const BAR_LEFT_START_X = 22.89;
// // double const BAR_LEFT_START_Y = 46.83;
// // double const BAR_LEFT_SIZE_X = 2.375;
// // double const BAR_LEFT_SIZE_Y = 93.58 - BAR_LEFT_START_Y;
// // double const BAR_RIGHT_START_X = ;
// // double const BAR_RIGHT_START_Y = ;
// // double const BAR_RIGHT_SIZE_X = ;
// // double const BAR_RIGHT_SIZE_Y = ;

// double const BAR_LEFT_START_X = ZONE_SIZE;
// double const BAR_LEFT_START_Y = 2 * ZONE_SIZE;
// double const BAR_LEFT_SIZE_X = 2.375;
// double const BAR_LEFT_SIZE_Y = 2 * ZONE_SIZE;
// double const BAR_RIGHT_START_X = 5 * ZONE_SIZE;
// double const BAR_RIGHT_START_Y = 2 * ZONE_SIZE;
// double const BAR_RIGHT_SIZE_X = 2.375;
// double const BAR_RIGHT_SIZE_Y = 2 * ZONE_SIZE;

// vector<tuple<int, int>> BAR_LEFT;
// vector<tuple<int, int>> BAR_RIGHT;
// BAR_LEFT.push_back(make_tuple(BAR_LEFT_START_X, BAR_LEFT_START_Y));
// BAR_LEFT.push_back(make_tuple(BAR_LEFT_START_X + BAR_LEFT_SIZE_X, BAR_LEFT_START_Y));
// BAR_LEFT.push_back(make_tuple(BAR_LEFT_START_X, BAR_LEFT_START_Y + BAR_LEFT_SIZE_Y));
// BAR_LEFT.push_back(make_tuple(BAR_LEFT_START_X + BAR_LEFT_SIZE_X, BAR_LEFT_START_Y + BAR_LEFT_SIZE_Y));

// BAR_RIGHT.push_back(make_tuple(BAR_RIGHT_START_X, BAR_RIGHT_START_Y));
// BAR_RIGHT.push_back(make_tuple(BAR_RIGHT_START_X + BAR_RIGHT_SIZE_X, BAR_RIGHT_START_Y));
// BAR_RIGHT.push_back(make_tuple(BAR_RIGHT_START_X, BAR_RIGHT_START_Y + BAR_RIGHT_SIZE_Y));
// BAR_RIGHT.push_back(make_tuple(BAR_RIGHT_START_X + BAR_RIGHT_SIZE_X, BAR_RIGHT_START_Y + BAR_RIGHT_SIZE_Y));

// double const BAR_MAIN_START_X = ZONE_SIZE;
// double const BAR_MAIN_START_Y = 2 * ZONE_SIZE;
// double const BAR_MAIN_SIZE_X = 4 * ZONE_SIZE;
// double const BAR_MAIN_SIZE_Y = 2.375;

// vector<tuple<int, int>> BAR_MAIN;

// BAR_MAIN.push_back(make_tuple(BAR_MAIN_START_X, BAR_MAIN_START_Y));
// BAR_MAIN.push_back(make_tuple(BAR_MAIN_START_X + BAR_MAIN_SIZE_X, BAR_MAIN_START_Y));
// BAR_MAIN.push_back(make_tuple(BAR_MAIN_START_X, BAR_MAIN_START_Y + BAR_MAIN_SIZE_Y));
// BAR_MAIN.push_back(make_tuple(BAR_MAIN_START_X + BAR_MAIN_SIZE_X, BAR_MAIN_START_Y + BAR_MAIN_SIZE_Y));

// // double const SIDE_Y_START = 46.83;
// // double const SIDE_LENGTH = 93.58 - 46.83;
// // double const SIDE_X_START = 22.89;
// // double const SIDE_WIDTH = 2.375;
// // double const MAIN_LENGTH = 117.52 - 22.89; // why not: FL-RL-RL? Will being generous with forbidden zones block too much for pathing?
// // double const MAIN_WIDTH = SIDE_WIDTH;      // TODO - verify this assumption
// // double const MAIN_X_START = SIDE_X_START + SIDE_WIDTH;
// // double const MAIN_Y_START = SIDE_Y_START + (SIDE_LENGTH / 2) - (SIDE_WIDTH / 2);

// vector<tuple<int, int>> MAIN;
// main.push_back(make_tuple(MAIN_X_START, MAIN_Y_START));
// main.push_back(make_tuple(MAIN_X_START + MAIN_LENGTH, MAIN_Y_START));
// main.push_back(make_tuple(MAIN_X_START, MAIN_Y_START + MAIN_WIDTH));
// main.push_back(make_tuple(MAIN_X_START + MAIN_LENGTH, MAIN_Y_START + MAIN_WIDTH));

// vector<tuple<int, int>> SIDE_LEFT;
// vector<tuple<int, int>> SIDE_RIGHT;
// SIDE_LEFT.push_back(make_tuple(SIDE_X_START, SIDE_Y_START));
// SIDE_LEFT.push_back(make_tuple(SIDE_X_START + SIDE_LENGTH, SIDE_Y_START));
// SIDE_LEFT.push_back(make_tuple(SIDE_X_START, SIDE_Y_START + SIDE_WIDTH));
// SIDE_LEFT.push_back(make_tuple(SIDE_X_START + SIDE_LENGTH, SIDE_Y_START + SIDE_WIDTH));

// SIDE_RIGHT.push_back(make_tuple())
//     // TODO - add walls' forbidden zones by adding the robot's bigger dimension to the beginning and end of walls

//     // double const ROLLER_POINT_1 = 0  ;
//     // double const ROLLER_POINT_2 = 23.08 ;
//     // double const ROLLER_POINT_3 = 117.33 ;
//     // double const ROLLER_POINT_4 = 140.41 ;

//     // // tuple<Node *, Node *, Node *> const RED_ROLLER_LEFT = make_tuple(new Node(ROLLER_POINT_1, ROLLER_POINT_1), new Node(ROLLER_POINT_1, ROLLER_POINT_2), new Node(ROLLER_POINT_2, ROLLER_POINT_1));
//     // // tuple<Node *, Node *, Node *> const RED_ROLLER_RIGHT = make_tuple(new Node(ROLLER_POINT_1, ROLLER_POINT_4), new Node(ROLLER_POINT_1, ROLLER_POINT_3), new Node(ROLLER_POINT_2, ROLLER_POINT_4));
//     // // tuple<Node *, Node *, Node *> const BLUE_ROLLER_RIGHT = make_tuple(new Node(ROLLER_POINT_4, ROLLER_POINT_4), new Node(ROLLER_POINT_4, ROLLER_POINT_3), new Node(ROLLER_POINT_3, ROLLER_POINT_4));
//     // // tuple<Node *, Node *, Node *> const BLUE_ROLLER_LEFT = make_tuple(new Node(ROLLER_POINT_1, ROLLER_POINT_4), new Node(ROLLER_POINT_2, ROLLER_POINT_4), new Node(ROLLER_POINT_3, ROLLER_POINT_1));
//     // tuple<tuple<int, int>, tuple<int, int>, tuple<int, int>> const RED_ROLLER_LEFT = make_tuple(make_tuple(ROLLER_POINT_1, ROLLER_POINT_1), make_tuple(ROLLER_POINT_1, ROLLER_POINT_2), make_tuple(ROLLER_POINT_2, ROLLER_POINT_1));
//     // tuple<tuple<int, int>, tuple<int, int>, tuple<int, int>> const RED_ROLLER_RIGHT = make_tuple(make_tuple(ROLLER_POINT_1, ROLLER_POINT_4), make_tuple(ROLLER_POINT_1, ROLLER_POINT_3), make_tuple(ROLLER_POINT_2, ROLLER_POINT_4));
//     // tuple<tuple<int, int>, tuple<int, int>, tuple<int, int>> const BLUE_ROLLER_RIGHT = make_tuple(make_tuple(ROLLER_POINT_4, ROLLER_POINT_4), make_tuple(ROLLER_POINT_4, ROLLER_POINT_3), make_tuple(ROLLER_POINT_3, ROLLER_POINT_4));
//     // tuple<tuple<int, int>, tuple<int, int>, tuple<int, int>> const BLUE_ROLLER_LEFT = make_tuple(make_tuple(ROLLER_POINT_1, ROLLER_POINT_4), make_tuple(ROLLER_POINT_2, ROLLER_POINT_4), make_tuple(ROLLER_POINT_3, ROLLER_POINT_1));
//     // vector<tuple<tuple<int, int>, tuple<int, int>, tuple<int, int>>> const ROLLERS;

//     // double const BLUE_GOAL

//     // TODO - you're assuming that origin GPS point is the center of the robot
//     // TODO - different forbidden zones depending on robot's direction will make things too complex. For simplicity, you just add whichever is bigger to the forbidden zones
//     double const ROBOT_WIDTH = 5;
// double const ROBOT_LENGTH = 8;

// // tuple<tuple<int, int>, tuple<int, int>, tuple<int, int>, tuple<int, int>> const RED_GOAL = make_tuple(make_tuple())

// int getDistanceDirect(Node *a, Node *b)
// {
//   int dx = abs(b->x - a->x);
//   int dy = abs(b->y - a->y);
//   return 10 * sqrt(dx * dx + dy * dy);
// }

// int h(Node *currentNode, Node *destination)
// {
//   // https://theory.stanford.edu/~amitp/GameProgramming/Heuristics.html
//   int dx = abs(currentNode->x - destination->x);
//   int dy = abs(currentNode->y - destination->y);
//   return 10 * (dx + dy) + (14 - 2 * 10) * min(dx, dy);
//   // return 10 * (max(abs(currentNode->x - destination->x), abs(currentNode->y - destination->y)));
//   // return 10 * (abs(currentNode->x - destination->x) + abs(currentNode->y - destination->y));
//   // int xDelta = abs(currentNode->x - destination->x);
//   // int yDelta = abs(currentNode->y - destination->y);
//   // if (xDelta > yDelta)
//   // {
//   //   return 14 * yDelta + 10 * (xDelta - yDelta);
//   // }
//   // else
//   // {
//   //   return 14 * xDelta + 10 * (yDelta - xDelta);
//   // }
// }

// vector<Node *> reconstructPath(map<Node *, Node *> cameFrom, Node *currentNode)
// {
//   vector<Node *> path;
//   path.push_back(currentNode);
//   while (cameFrom.find(currentNode) != cameFrom.end())
//   {
//     currentNode = cameFrom[currentNode];
//     path.push_back(currentNode);
//   }
//   return path;
// }

// // vector<Node *> AStar(Node *origin, Node *destination, int (*h)(Node *, Node *))
// // {
// //   set<Node *> open;
// //   open.insert(origin);
// //   map<Node *, Node *> cameFrom;
// //   map<Node *, int> gScore;
// //   gScore[origin] = 0;
// //   map<Node *, int> fScore;
// //   fScore[origin] = gScore[origin] + h(origin, destination);

// //   while (open.size() > 0)
// //   {
// //     int lowestFScore = INT_MAX;
// //     Node *currentNode;

// //     for (Node *node : open)
// //     {
// //       if (fScore.find(node) == fScore.end() || fScore[node] < lowestFScore)
// //       {
// //         currentNode = node;
// //         lowestFScore = fScore[node];
// //       }
// //     }

// //     if (currentNode == destination)
// //     {
// //       return reconstructPath(cameFrom, currentNode);
// //     }

// //     open.erase(currentNode);
// //     for (Node *neighbor : currentNode->neighbors)
// //     {
// //       // TODO - is the node length 10 or 1?
// //       // TODO - is the diagonal node length more than the normal?
// //       // TODO - important
// //       int tempGScore = gScore[currentNode] + 10; // + d(weight of edge from currentNode to neighbor)
// //       if (gScore.find(neighbor) == gScore.end() || tempGScore < gScore[neighbor])
// //       {
// //         cameFrom[neighbor] = currentNode;
// //         gScore[neighbor] = tempGScore;
// //         // cout << neighbor->x << ", " << neighbor->y << " -> " << destination->x << ", " << destination->y << " || " << h(neighbor, destination) << endl;
// //         fScore[neighbor] = tempGScore + h(neighbor, destination);
// //         if (open.find(neighbor) == open.end())
// //         {
// //           open.insert(neighbor);
// //         }
// //       }
// //     }
// //   }
// //   vector<Node *> z;
// //   return z;
// // }

// int getEdgeCost(Node *a, Node *b)
// {
//   // Is the node non-diagonal?
//   if ((b->x == a->x + 1 && b->y == a->y) || (b->x == a->x - 1 && b->y == a->y) || (b->x == a->x && b->y == a->y + 1) || (b->x == a->x && b->y == a->y - 1))
//   {
//     return 10;
//   }
//   // Is the node diagonal?
//   else if ((b->x == a->x + 1 && b->y == a->y + 1) || (b->x == a->x + 1 && b->y == a->y - 1) || (b->x == a->x - 1 && b->y == a->y + 1) || (b->x == a->x - 1 && b->y == a->y + 1))
//   {
//     return 14;
//   }
//   else
//   {
//     throw "Error 010";
//   }
// }

// vector<Node *> AStar(Node *origin, Node *destination, int (*h)(Node *, Node *))
// {
//   set<Node *> open;
//   open.insert(origin);
//   map<Node *, Node *> cameFrom;
//   map<Node *, int> gScore;
//   gScore[origin] = 0;
//   map<Node *, int> fScore;
//   fScore[origin] = gScore[origin] + h(origin, destination);

//   while (open.size() > 0)
//   {
//     int lowestFScore = INT_MAX;
//     Node *currentNode;

//     for (Node *node : open)
//     {
//       if (fScore.find(node) == fScore.end() || fScore[node] < lowestFScore)
//       {
//         currentNode = node;
//         lowestFScore = fScore[node];
//       }
//     }

//     if (currentNode == destination)
//     {
//       return reconstructPath(cameFrom, currentNode);
//     }

//     open.erase(currentNode);
//     int edgeCost;
//     int edgeCostFromParent;
//     for (Node *neighbor : currentNode->neighbors)
//     {
//       edgeCost = getEdgeCost(currentNode, neighbor);
//       edgeCostFromParent = getDistanceDirect(cameFrom[currentNode], neighbor);
//       // TODO - is the node length 10 or 1?
//       // TODO - is the diagonal node length more than the normal?
//       // Could it be other than 10 or 14 if any-angled to a far away node?
//       // TODO - important
//       int tempGScore = gScore[currentNode] + edgeCost; // + d(weight of edge from currentNode to neighbor)
//       if (gScore.find(neighbor) == gScore.end() || tempGScore < gScore[neighbor])
//       {
//         if (hasLOS(new Graph(0, 0), currentNode, neighbor))
//         {
//           if (gScore[cameFrom[currentNode]] + edgeCost < gScore[neighbor])
//           {
//             gScore[neighbor] = gScore[cameFrom[currentNode]] + edgeCostFromParent;
//             cameFrom[neighbor] = cameFrom[currentNode];
//             if (open.find(neighbor) != open.end())
//             {
//               open.erase(neighbor);
//             }
//             open.insert(neighbor);
//           }
//         }
//         else
//         {
//           if (gScore[currentNode] + edgeCost < gScore[neighbor])
//           {
//             gScore[neighbor] = gScore[currentNode] + edgeCost;
//             cameFrom[neighbor] = currentNode;
//             if (open.find(neighbor) != open.end())
//             {
//               open.erase(neighbor);
//             }
//             open.insert(neighbor);
//           }
//         }
//         cameFrom[neighbor] = currentNode;
//         gScore[neighbor] = tempGScore;
//         // cout << neighbor->x << ", " << neighbor->y << " -> " << destination->x << ", " << destination->y << " || " << h(neighbor, destination) << endl;
//         fScore[neighbor] = tempGScore + h(neighbor, destination);
//         if (open.find(neighbor) == open.end())
//         {
//           open.insert(neighbor);
//         }
//       }
//     }
//   }
//   vector<Node *> z;
//   return z;
// }

// bool isInForbiddenZone(Node *node)
// {
// }

// // where is (0, 0)?
// // GPS -> my GPS coordinates
// // my GPS coordinates -> GPS

// // GPS -> centimeters:
// // centimeters -> GPS:
// // inches -> centimeters: inches * 2.54
// // centimeters -> inches: centimeters / 2.54 (no use-case)
// // double cm -> int cm:   int(double cm)
// // int cm -> double cm:   no use-case

// int main()
// {
//   // ROLLERS.push_back(RED_ROLLER_LEFT);
//   // ROLLERS.push_back(RED_ROLLER_RIGHT);
//   // ROLLERS.push_back(BLUE_ROLLER_LEFT);
//   // ROLLERS.push_back(BLUE_ROLLER_RIGHT);

//   random_device rd;
//   mt19937 rng(rd());
//   uniform_int_distribution<int> randX(0, X_NODES - 1);
//   uniform_int_distribution<int> randY(0, Y_NODES - 1);

//   int originX = randX(rng);
//   int originY = randY(rng);
//   int destinationX = randX(rng);
//   int destinationY = randY(rng);

//   cout << "Path from (" << originX << ", " << originY << ") to (" << destinationX << ", " << destinationY << ")\n\n";

//   Graph graph = Graph(X_NODES, Y_NODES);
//   vector<Node *> path = AStar(graph.getNode(originX, originY), graph.getNode(destinationX, destinationY), h);
//   for (int i = path.size() - 1; i >= 0; i--)
//   {
//     cout << "(" << path[i]->x << ", " << path[i]->y << ")\n";
//   }

//   for (int y = 0; y < Y_NODES; y++)
//   {
//     for (int x = 0; x < X_NODES; x++)
//     {
//       // cout << "Node (" << graph.getNode(x, y)->x << ", " << graph.getNode(x, y)->y << ")\n";
//       // cout << "Neighbors " << graph.getNode(x, y)->neighbors.size() << endl;
//       // for (Node *neightbor : graph.getNode(x, y)->neighbors)
//       // {
//       //   cout << "Neighbor (" << neightbor->x << ", " << neightbor->y << ")\n";
//       // }
//     }
//   }
//   vector<tuple<int, int>> ROLLER_RED_LEFT;
//   ROLLER_RED_LEFT.push_back(make_tuple(0, 0));
// }

// // gps from ecnter
// // square robot
// // 1 team, auto mode yes
// // gps from ceneter
// // inches, degrees from x ccw
// // y pos is up, x pos is right
// // degree angle is way robot is facing, wrt to field
// //

// // rbot directions: rotate, drive straight, drive backwards

// // GPS to Local (x, y) -> (y + 30, x + 30)
// // Local to GPS (x, y) -> (y - 30, x - 30)

// tuple<int, int> GPStoLocal(tuple<int, int> point)
// {
//   return make_tuple(get<1>(point) + 30, get<0>(point) + 30);
// }

// tuple<int, int> LocaltoGPS(tuple<int, int> point)
// {
//   return make_tuple(get<1>(point) - 30, get<0>(point) - 30);
// }

// // DRIVE: distance and angle

// // Theta* (few points)
// // Path
// // Directions

// // TODO - turning in place extra space needed

// // TODO - use priority queue

// // (moving horizontal, moving vertical)
