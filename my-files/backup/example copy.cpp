#include <iostream>
#include <vector>

using namespace std;

float CELL_SIZE = 1;
// TODO - different x and y map sizes
float MAP_SIZE = 500;

class Node
{
public:
  float x;
  float y;
  bool visited = false;
  Node *parent;
  vector<Node *> neighbors;

  Node(float x = 0, float y = 0)
  {
    this->x = x;
    this->y = y;
  };

  void addNeighbor(Node *node)
  {
    this->neighbors.push_back(node);
  }
};

bool operator==(const Node &left, const Node &right)
{
  return (left.x == right.x) && (left.y == right.y);
};

class Graph
{
public:
  vector<Node *> nodes;

  Graph(){};

  Node *getNode(float x, float y)
  {
    for (int i = 0; i < this->nodes.size(); i++)
    {
      if (this->nodes[i]->x == x && this->nodes[i]->y == y)
      {
        return this->nodes[i];
      }
    }
    throw "A";
    cout << "ERROR 001";
  };
};

int main()
{
  Graph graph;
  Node *tempNode;

  for (float x = 0; x < MAP_SIZE; x += CELL_SIZE)
  {
    for (float y = 0; y < MAP_SIZE; y += CELL_SIZE)
    {
      // tempNode = new Node(x, y);
      graph.nodes.push_back(new Node(x, y));
      // graph.nodes.push_back(&Node(x, y));
    };
  };

  for (float x = 0; x < MAP_SIZE - 1; x += CELL_SIZE)
  {
    for (float y = 0; y < MAP_SIZE - 1; y += CELL_SIZE)
    {
      if (x < MAP_SIZE)
      {
        // Node *currentNode = graph.getNode(x, y);
        // Node *futureNode = graph.getNode(x + CELL_SIZE, y);
        // currentNode->addNeighbor(futureNode);
        // vector<Node *> currentNodeNeighbors = currentNode->neighbors;
        // currentNodeNeighbors.push_back(futureNode);
        // graph.getNode(x, y)->neighbors.push_back(graph.getNode(x + CELL_SIZE, y));
        graph.getNode(x, y)->addNeighbor(graph.getNode(x + CELL_SIZE, y));
      };
      if (y < MAP_SIZE)
      {
        graph.getNode(x, y)->addNeighbor(graph.getNode(x, y + CELL_SIZE));
        // graph.getNode(x, y)->neighbors.push_back(graph.getNode(x, y + CELL_SIZE));
      };
    };
  };

  Node *originNode = graph.getNode(0, 0);
  Node *destinationNode = graph.getNode(3, 3);
  vector<Node *> queue;

  originNode->visited = true;
  queue.push_back(originNode);

  Node *currentNode;
  while (queue.size() != 0)
  {
    currentNode = queue[0];
    queue.erase(queue.begin());
    if (currentNode == destinationNode)
    {
      break;
    };
    for (int i = 0; i < currentNode->neighbors.size(); i++)
    {
      if (!currentNode->neighbors[i]->visited)
      {
        currentNode->neighbors[i]->visited = true;
        currentNode->neighbors[i]->parent = currentNode;
        queue.push_back(currentNode->neighbors[i]);
      }
    };
  };

  vector<Node *> path;
  while (currentNode->parent != NULL)
  {
    // cout << "(" << currentNode->x << ", " << currentNode->y << ") -> (" << currentNode->parent->x << ", " << currentNode->parent->y << ")\n";
    path.push_back(currentNode);
    currentNode = currentNode->parent;
  }
  path.push_back(currentNode);

  for (int i = path.size() - 1; i > 0; i--)
  {
    cout << "(" << path[i]->x << ", " << path[i]->y << ") -> (" << path[i - 1]->x << ", " << path[i - 1]->y << ")\n";
  }
};