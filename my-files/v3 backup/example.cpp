// TODO - was it worth it to pre-compute, since it's very fast to find a path from origin to all destinations?

#include <iostream>
#include <vector>
#include <queue>
#include "random"
// TODO - async or threads? GCC vs other's implementations?
#include <future>
#include <string>

#include "Node.h"
#include "Graph.h"

// TODO - more higher-level functions to hide away the details

// TODO - add forbidden zones

using namespace std;

float const FIELD_SIDE = 3.566414;
// float const FIELD_SIDE = 10;

float const NODE_SIZE = 0.01;
float const MAP_SIZE_X = FIELD_SIDE;
float const MAP_SIZE_Y = FIELD_SIDE;

// TODO - I'm ignoring the last remaining space, shouldn't be a problem
int const X_NODES = int(MAP_SIZE_X / NODE_SIZE);
int const Y_NODES = int(MAP_SIZE_Y / NODE_SIZE);
int const TOTAL_NODES = X_NODES * Y_NODES;

bool operator==(const Node &left, const Node &right)
{
  return (left.x == right.x) && (left.y == right.y);
};

// Graph findPath(Graph graph, Node originNode)
// {
//   // create graph
//   // find path
// }

void createGraph(Graph &graph)
{
  for (int x = 0; x < X_NODES; x++)
  {
    for (int y = 0; y < Y_NODES; y++)
    {
      // graph.nodes[x][y] = new Node(x, y);
      Node *tempNode = new Node(x, y);
      graph.nodes[x][y] = tempNode;
      if (x > 0)
      {
        graph.nodes[x][y]->addNeighbor(graph.nodes[x - 1][y]);
        graph.nodes[x - 1][y]->addNeighbor(graph.nodes[x][y]);
      };
      if (y > 0)
      {
        graph.nodes[x][y]->addNeighbor(graph.nodes[x][y - 1]);
        graph.nodes[x][y - 1]->addNeighbor(graph.nodes[x][y]);
      };
    };
  };
};

void BFS(Graph &graph, int originX, int originY)
{
  Node *originNode = graph.getNode(originX, originY);
  originNode->visited = true;
  queue<Node *> frontierQueue;
  frontierQueue.push(originNode);
  Node *currentNode;

  while (frontierQueue.size() != 0)
  {
    currentNode = frontierQueue.front();
    frontierQueue.pop();
    for (Node *neighbor : currentNode->neighbors)
    {
      if (!neighbor->visited)
      {
        neighbor->visited = true;
        neighbor->parent = currentNode;
        frontierQueue.push(neighbor);
      }
    }
  };
}

// void printPath(Graph &graph, int originX, int originY)
// {
//   random_device rd;
//   mt19937 rng(rd());
//   uniform_int_distribution<int> randX(0, MAP_SIZE_X - 1);
//   uniform_int_distribution<int> randY(0, MAP_SIZE_Y - 1);
//   vector<Node *> path;
//   Node *currentNode;

//   for (int i = 0; i < 1; i++)
//   {
//     // TODD - could choose a forbidden node
//     currentNode = graph.nodes[randX(rng)][randY(rng)];
//     cout << "Destination: (" << currentNode->x << ", " << currentNode->y << ")" << endl;
//     while (currentNode->parent != nullptr)
//     {
//       path.push_back(currentNode);
//       currentNode = currentNode->parent;
//     }
//     path.push_back(currentNode);

//     for (int j = path.size() - 1; j > 0; j--)
//     {
//       cout << "(" << path[j]->x << ", " << path[j]->y << ") -> (" << path[j - 1]->x << ", " << path[j - 1]->y << ")\n";
//     }

//     cout << endl;
//     path.clear();
//   }
// }

void printPath(vector<Node *> path)
{
  for (int j = path.size() - 1; j > 0; j--)
  {
    cout << "(" << path[j]->x << ", " << path[j]->y << ") -> (" << path[j - 1]->x << ", " << path[j - 1]->y << ")\n";
  }

  cout << endl;
}

vector<Node *> getPath(Graph &graph, int originX, int originY, int destinationX, int destinationY)
{
  Node *destinationNode = graph.nodes[destinationX][destinationY];
  Node *currentNode = destinationNode;
  vector<Node *> path;

  while (currentNode->parent != nullptr)
  {
    path.push_back(currentNode);
    currentNode = currentNode->parent;
  }
  path.push_back(currentNode);

  return path;
}

int main()
{
  // Graph graph = Graph(X_NODES, Y_NODES);
  // createGraph(graph);
  // int originX = 0;
  // int originY = 0;
  // BFS(graph, originX, originY);
  // BFS(graph, 0, 0);
  // printPath(graph, originX, originY);

  // Graph graph;

  vector<Node *> ****paths;
  paths = new vector<Node *> ***[X_NODES];
  int nodesCompleted = 0;
  string garbage;

  for (int originX = 0; originX < X_NODES; originX++)
  {
    paths[originX] = new vector<Node *> **[Y_NODES];
    for (int originY = 0; originY < Y_NODES; originY++)
    {
      // TODO - why doesn't processing 2500/2500 print?
      // cout << "\n\n\n\n\nProcessing Origin (" << originX << ", " << originY << ") -> (" << destinationX << ", " << destinationY << ")\n\n\n\n\n" << endl;
      // cout << "\n\n\n\n\nProcessing paths for origin (" << originX << ", " << originY << ")\n\n\n\n\n";
      cout << "Processing paths for origin (";
      if (originX < 10)
      {
        cout << "0" << originX << ", ";
      }
      else
      {
        cout << originX << ", ";
      }
      if (originY < 10)
      {
        cout << "0" << originY;
      }
      else
      {
        cout << originY;
      }
      cout << "). Node " << to_string(nodesCompleted) << "/" << to_string(TOTAL_NODES) << " (" << to_string((nodesCompleted / float(TOTAL_NODES)) * 100.0) << "%)\n";
      // cout << "). Node " << to_string(nodesCompleted) << "/" << to_string(TOTAL_NODES) << " (" << to_string(((1.0 * (originX + 1) * (originY + 1) * 1.0) / float(TOTAL_NODES)) * 100.0) << "%)\n";

      Graph graph = Graph(X_NODES, Y_NODES);
      paths[originX][originY] = new vector<Node *> *[X_NODES];
      createGraph(graph);
      BFS(graph, originX, originY);
      for (int destinationX = 0; destinationX < X_NODES; destinationX++)
      {
        paths[originX][originY][destinationX] = new vector<Node *>[Y_NODES];
        for (int destinationY = 0; destinationY < Y_NODES; destinationY++)
        {
          // paths[originX][originY][destinationX][destinationY] = new vector<Node *>;
          paths[originX][originY][destinationX][destinationY] = getPath(graph, originX, originY, destinationX, destinationY);
          // printPath(paths[originX][originY][destinationX][destinationY]);
        }
      }
      nodesCompleted++;
    }

    // printPath(paths[originX][0][9][9]);
    // cin >> garbage;
  }

  cout << endl;
  // printPath(paths[5][6][38][9]);
  // printPath(paths[2][8][9][1]);
  printPath(paths[9][9][9][5]);

  // vector<Node *> *****paths;
  // paths = new vector<Node *> ****[X_NODES];

  // for (int originX = 0; originX < X_NODES; originX++)
  // {
  //   paths[originX] = new vector<Node *> ***[Y_NODES];
  //   for (int originY = 0; originY < Y_NODES; originY++)
  //   {
  //     Graph graph = Graph(originX, originY);
  //     paths[originX][originY] = new vector<Node *> **[X_NODES];
  //     for (int destinationX = 0; destinationX < X_NODES; destinationX++)
  //     {
  //       paths[originX][originY][destinationX] = new vector<Node *> *[Y_NODES];
  //       for (int destinationY = 0; destinationY < Y_NODES; destinationY++)
  //       {
  //         // paths[originX][originY][destinationX][destinationY] = new vector<Node *>;
  //         createGraph(graph);
  //         BFS(graph, originX, originY);
  //         paths[originX][originY][destinationX][destinationY] = getPath(graph, originX, originY, destinationX, destinationY);
  //       }
  //     }
  //   }
  // }

  // for (int originX = 0; originX < X_NODES; originX++)
  // {
  //   if (originX == 0)
  //   {
  //     paths[originX] = new vector<Node *> ***[Y_NODES];
  //   }
  //   for (int originY = 0; originY < Y_NODES; originY++)
  //   {
  //     if (originY == 0)
  //     {
  //       paths[originX][originY] = new vector<Node *> **[X_NODES];
  //     }
  //     for (int destinationX = 0; destinationX < X_NODES; destinationX++)
  //     {
  //       if (destinationX == 0)
  //       {
  //         paths[originX][originY][destinationX] = new vector<Node *> *[Y_NODES];
  //       }
  //       for (int destinationY = 0; destinationY < Y_NODES; destinationY++)
  //       {
  //         if (destinationY == 0)
  //         {
  //           paths[originX][originY][destinationX][destinationY] = new vector<Node *>;
  //         }
  //       }
  //     }
  //   }
  // }

  // for (int originX = 0; originX < X_NODES; originX++)
  // {
  //   for (int originY = 0; originY < Y_NODES; originY++)
  //   {
  //     for (int destinationX = 0; destinationX < X_NODES; destinationX++)
  //     {
  //       for (int destinationY = 0; destinationY < Y_NODES; destinationY++)
  //       {

  //       }
  //     }
  //   }
  // }
};

// TODO - convert vec of nodes path into a function for smooth driving