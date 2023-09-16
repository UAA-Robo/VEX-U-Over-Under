#include "Graph.h"
#include <vector>
#include "Node.h"

class GUI
{
private:
  Graph *graph;
  std::vector<Node *> forbiddenNodes;
  int CELL_SIZE;
  int yOffset;
  int xPadding;
  int yPadding;
  // SDL_Renderer *renderer;
  // SDL_Window *window;

  void drawCell(SDL_Renderer *renderer, Node *node, SDL_Color color);

public:
  GUI(Graph *graph, int CELL_SIZE);
  int run(bool showSnapshots);
};

// GUI CONTROLS
//
// TAB - switch between show pathfinding history mode (default) and simple show path mode
// SPACE - randomly generate two points and generate the path or path history between them, depending on the mode
// RIGHT CLICK - clear current path and allows user to click to nodes to show the path between
// LEFT CLICK - selects node to use in pathfinding (max 2 nodes)
