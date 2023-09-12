// #include <iostream>
// #include <vector>
// #include <queue>
// #include "random"
// #include <future>
// #include <string>
// #include <thread>
// #include <fstream>
// #include <chrono>

// #include "Node.h"
// #include "Graph.h"

// using namespace std;

// // Algorithm
// // 1. Given: 1. Origin position (x, y) 2. Destination position (a, b)
// // 2. What should the robot do to get there? And for how long?
// //
// // 1. Generate the map and find the path from (x, y) to (a, b)
// // 2. Take that path and

// // TODO - add diagonal edges

// int const X_NODES = 360;
// int const Y_NODES = 360;

// void createGraph(Graph &graph)
// {
//   for (int x = 0; x < X_NODES; x++)
//   {
//     for (int y = 0; y < Y_NODES; y++)
//     {
//       // graph.nodes[x][y] = new Node(x, y);
//       Node *tempNode = new Node(x, y);
//       graph.nodes[x][y] = tempNode;
//       if (x > 0)
//       {
//         graph.nodes[x][y]->addNeighbor(graph.nodes[x - 1][y]);
//         graph.nodes[x - 1][y]->addNeighbor(graph.nodes[x][y]);
//       };
//       if (y > 0)
//       {
//         graph.nodes[x][y]->addNeighbor(graph.nodes[x][y - 1]);
//         graph.nodes[x][y - 1]->addNeighbor(graph.nodes[x][y]);
//       };
//     };
//   };
// };

// void BFS(Graph &graph, int originX, int originY)
// {
//   Node *originNode = graph.getNode(originX, originY);
//   originNode->visited = true;
//   queue<Node *> frontierQueue;
//   frontierQueue.push(originNode);
//   Node *currentNode;

//   while (frontierQueue.size() != 0)
//   {
//     currentNode = frontierQueue.front();
//     frontierQueue.pop();
//     for (Node *neighbor : currentNode->neighbors)
//     {
//       if (!neighbor->visited)
//       {
//         neighbor->visited = true;
//         neighbor->parent = currentNode;
//         frontierQueue.push(neighbor);
//       }
//     }
//   };
// }

// vector<Node *> getPath(Graph &graph, int originX, int originY, int destinationX, int destinationY)
// {
//   Node *destinationNode = graph.nodes[destinationX][destinationY];
//   Node *currentNode = destinationNode;
//   vector<Node *> path;

//   while (currentNode->parent != nullptr)
//   {
//     path.push_back(currentNode);
//     currentNode = currentNode->parent;
//   }
//   path.push_back(currentNode);

//   return path;
// }

// void printPath(vector<Node *> path)
// {
//   for (int j = path.size() - 1; j > 0; j--)
//   {
//     cout << "(" << path[j]->x << ", " << path[j]->y << ") -> (" << path[j - 1]->x << ", " << path[j - 1]->y << ")\n";
//   }

//   cout << endl;
// }

// int main()
// {

//   using chrono::duration;
//   using chrono::duration_cast;
//   using chrono::high_resolution_clock;
//   using chrono::milliseconds;

//   auto t1 = high_resolution_clock::now();
//   Graph graph = Graph(X_NODES, Y_NODES);
//   createGraph(graph);
//   // int originX = 0;
//   // int originY = 0;
//   BFS(graph, 0, 0);
//   vector<Node *> path = getPath(graph, 0, 0, 5, 5);
//   // printPath(path);
//   auto t2 = high_resolution_clock::now();
//   duration<double, std::milli> ms_double = t2 - t1;
//   std::cout << ms_double.count() << "ms\n";
// }