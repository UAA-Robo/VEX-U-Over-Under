#include <stdio.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include "Graph.h"
#include "Node.h"

using namespace std;

int CELLS_NUMBER = 100;
float CELL_SIZE = 0.01;
float MAP_SIZE = 3.566414;


struct NodeCompare
{
  bool operator()(const Node &leftNode, const Node &rightNode) const
  {
    // return (leftNode.x < rightNode.x) || (leftNode.y < rightNode.y);
    if (leftNode.x == rightNode.x)
    {
      return leftNode.y < rightNode.y;
    }
    else
    {
      return leftNode.x < rightNode.x;
    }
  }
};

std::ostream &operator<<(std::ostream &os, Node const &m)
{
  return os << "(" << m.x << ", " << m.y << ")";
}

// float generateMaps(int cellsNumber)
// {
//   map<Node, Node[]> maps[cellsNumber][cellsNumber];

//   for (int x = 0; x < cellsNumber; x++)
//   {
//     for (int y = 0; y < cellsNumber; y++)
//     {
//       // maps[x][y][Node(0, 0)] = [Node(1, 2)];
//     }
//   }
// };


/*

INPUT
(x, y) current coordinates
(a, b) destination coordinates

OUTPUT
(currentNode, node2, node3, ..., destinationNode)

Algorithm
(x, y) based on current coordinates, Maps[x][y] returns a dictionary dict where dict[CustomObject(a, b)] returns the pre-computed optimal path from (x, y) to (a, b)

Questions / Problems
* What to do when coordinates are floats but map coordinates are integers
* Should the dictionary be a 2-d array containing a list of nodes/tuples?
* If the map is 1000x1000, then the distance between 2 adjacent nodes is very small so the robot will barely move at all, stop, then move a little, stop, etc.
* Node I/O
* How accurate is the GPS? 1.435 vs 1.4? How many digits does it report?
* Am I leaving out the last cell because of loop limit end error?
* What if, by accident, the GPS tells the robot the destination point is inside an obstacle?
* Define forbidden zones, which return an empty path for invalid destination points
* Coordinate-based or Zone/Node-based
* Converting GPS coordinates to centimeter X and Y
* Angling the robot first, middle, last points when it goes to grab the triball
* Make forbidden zones / Nodes

Misc
* Dictionary: input: object containing coordinates a and b as member variables
              output: a list of objects, each containing coordinates x and y as member variables

*/

int main()
{
  // map<Node, map<Node, vector<Node>>> maps = {};
  // map<Node, vector<Node>, NodeCompare> dict;
  // vector<Node> v;
  // v.push_back(Node(1, 0));
  // v.push_back(Node(1, 1));
  // v.push_back(Node(1, 2));
  // dict[Node(0, 0)] = v;

  // Node x = Node(0, 0);
  // // cout << x.x << ", " << x.y;

  // for (const auto &elem : dict)
  // {
  //   cout << elem.first << endl
  //        << elem.second.size() << endl;
  // }

  // map<Node, map<Node, vector<Node>>, NodeCompare> maps;
  // // [431][23]

  Node originNode;
  Node destinationNode;
  // vector<Node> path;
  // map<Node, vector<Node>, NodeCompare> pathsMap;
  Graph graph;

  // generate graph and nodes

  for (float originX = 0; originX < MAP_SIZE; originX += CELL_SIZE)
  {
    for (float originY = 0; originY < MAP_SIZE; originY += CELL_SIZE)
    {
      originNode = Node(originX, originY);
      vector<Node> v;
      v.push_back(originNode);
      graph.addNode(originNode, v);
    }
  }

  for (float originX = 0; originX < MAP_SIZE; originX += CELL_SIZE)
  {
    for (float originY = 0; originY < MAP_SIZE; originY += CELL_SIZE)
    {
      // graph.addEdge(Node(originX, originY), Node(originX - CELL_SIZE, originY));
      graph.addEdge(Node(originX, originY), Node(originX + CELL_SIZE, originY));
      // graph.addEdge(Node(originX, originY), Node(originX, originY - CELL_SIZE));
      graph.addEdge(Node(originX, originY), Node(originX, originY + CELL_SIZE));
    }
  }
  {
    Node originNode = Node(0, 0);
    // Mark all the vertices as not visited
    for (int i = 0; i < graph.graph.size(); i++)
    {
      graph.graph[i].visited = false;
    }

    // Create a queue for BFS
    vector<Node> queue;
    int front = 0;
    int rear = 0;

    // Mark the current node as visited and enqueue it
    originNode.visited = true;
    queue[rear++] = originNode;

    Node node;
    while (front != rear)
    {

      // Dequeue a vertex from queue and print it
      node = queue[front++];
      printf("%d ", node);

      // Get all adjacent vertices of the dequeued
      // vertex s.
      // If an adjacent has not been visited,
      // then mark it visited and enqueue it
      for (int adjacent = 0; adjacent < graph.graph.size();
           adjacent++)
      {
        if (graph.adj[s][adjacent] && !visited[adjacent])
        {
          visited[adjacent] = true;
          queue[rear++] = adjacent;
        }
      }
    }
  }

  // for (float originX = 0; originX < MAP_SIZE; originX += CELL_SIZE)
  // {
  //   for (float originY = 0; originY < MAP_SIZE; originY += CELL_SIZE)
  //   {
  //     originNode = Node(originX, originY);

  //     for (float destinationX = 0; destinationX < MAP_SIZE; destinationX += CELL_SIZE)
  //     {
  //       for (float destinationY = 0; destinationY < MAP_SIZE; destinationY += CELL_SIZE)
  //       {
  //         destinationNode = Node(destinationX, destinationY);
  //       }
  //     }
  //   }
  // }

  // map<Node, string, NodeCompare> example;
  // example[Node(0, 0)] = "A";
  // example[Node(0, 1)] = "B";
  // example[Node(0, 2)] = "C";
  // example[Node(1, 0)] = "D";
  // example[Node(1, 1)] = "E";
  // example[Node(1, 2)] = "F";

  // map<Node, string>::iterator iterator = example.begin();
  // while (iterator != example.end())
  // {
  //   cout << "Key: " << iterator->first << ", Value: " << iterator->second << endl
  //        << endl;
  //   iterator++;
  // }

  // cout << example[Node(1, 1)];
}