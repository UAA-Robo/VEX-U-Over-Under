#include <vector>
#include "Graph.h"
#include <SDL2/SDL.h>

class GUI
{
private:
  Graph *graph;
  Graph *graph2;
  int xNodes;
  int yNodes;
  int gridWidth;
  int gridHeight;
  bool showSnapshots;
  std::vector<Node *> forbiddenNodes;
  int cellSize;
  int yOffset;
  int xPadding;
  int yPadding;
  int scrollSensitivity;
  int nodesSelected;
  int snapshotNumber;
  bool autoMode;
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_Rect grid_cursor_ghost;
  SDL_Color grid_background;
  SDL_Color grid_line_color;
  SDL_Color grid_cursor_ghost_color;
  SDL_Color grid_cursor_color;
  SDL_bool quit;
  SDL_bool mouse_active;
  SDL_bool mouse_hover;
  int pathsFound;
  int durationTotal;
  std::vector<Node *> pathNodes;
  std::vector<std::vector<Node *> *> pathNodesSnapshots;
  std::vector<Node *> selectedNodes;
  bool selectingNodesAllowed;
  SDL_Color forbiddenNodesColor;
  SDL_Color pathNodesColor;
  SDL_Color pathNodesPastColor;
  SDL_Color starting;
  SDL_Color ending;
  

  void drawCell(SDL_Renderer *renderer, Node *node, SDL_Color color);
  void drawCellSecond(SDL_Renderer *renderer, Node *node, SDL_Color color, int xDifference);
  void init();

  void getPath(Node *a, Node *b);
  void getRandomPath();
  void getPathSnapshots(Node *a, Node *b);
  void getRandomPathSnapshots();

  void getVGPath(Node *a, Node *b);
  void getVGRandomPath();
  void getVGPathSnapshots(Node *a, Node *b);
  void getVGRandomPathSnapshots();

  void eventLoop();

public:
  GUI(Graph *graph, int cellSize, bool showSnapshots);
  GUI(Graph *graph, Graph *graph2, int cellSize, bool showSnapshots);
  int run();
  // int run(SDL_Window *window, SDL_Renderer *renderer);

  // int runTwo(bool showSnapshots);
};

// GUI CONTROLS
//
// TAB - switch between show pathfinding history mode (default) and simple show path mode
// SPACE - randomly generate two points and generate the path or path history between them, depending on the mode
// RIGHT CLICK - clear current path and allows user to click to nodes to show the path between
// LEFT CLICK - selects node to use in pathfinding (max 2 nodes)
// UP ARROW - (during pathfinding histoy mode) automatically steps through the pathfinding history
// DOWN ARROW - (during pathfinding histoy mode) stops the auto mode
// LEFT ARROW - manually go to the previous snapshot
// RIGHT ARROW - manually got to the next snapshot