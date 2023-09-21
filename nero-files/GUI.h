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
  int cellSize;
  int gridWidth;
  int gridHeight;
  int xPadding;
  int yPadding;
  int yOffset;
  int scrollSensitivity;
  int pathsFound;
  int durationTotal;
  int snapshotNumber;
  int nodesSelected;
  bool showSnapshots;
  bool selectingNodesAllowed;
  bool autoMode;

  std::vector<Node *> forbiddenNodes;
  std::vector<Node *> selectedNodes;
  std::vector<Node *> pathNodes;
  std::vector<std::vector<Node *> *> pathNodesSnapshots;

  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Rect gridCursor;
  SDL_bool quit;
  SDL_bool mouse_active;
  SDL_bool mouse_hover;
  SDL_Color gridBackgroundColor;
  SDL_Color gridLineColor;
  SDL_Color gridCursorColor;
  SDL_Color forbiddenNodesColor;
  SDL_Color pathNodesColor;
  SDL_Color pathNodesPastColor;
  SDL_Color starting;
  SDL_Color ending;

  void drawCell(SDL_Renderer *renderer, Node *node, SDL_Color color);
  void drawCellSecond(SDL_Renderer *renderer, Node *node, SDL_Color color, int xDifference);
  void init(Graph *graph, Graph *graph2, int cellSize);

  void getPath(Node *a, Node *b);
  void getRandomPath();
  void getPathSnapshots(Node *a, Node *b);
  void getRandomPathSnapshots();

  void getVGPath(Node *a, Node *b);
  void getVGRandomPath();
  void getVGPathSnapshots(Node *a, Node *b);
  void getVGRandomPathSnapshots();

  void eventLoop();
  void switchModes();
  void generatePath();
  void resetGrid();
  void selectNodes(SDL_Event &event);
  void moveCursor(SDL_Event &event);
  void scroll(SDL_Event &event);
  void drawGrid();
  void drawCursor();
  void drawForbiddenNodes();
  void drawSnapshots();
  void drawPath();
  void drawBackground();

public:
  GUI(Graph *graph, int cellSize);
  GUI(Graph *graph, Graph *graph2, int cellSize);
  int run();

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