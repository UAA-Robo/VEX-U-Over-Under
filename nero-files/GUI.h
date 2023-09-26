#include <vector>
#include <set>
#include "Graph.h"
#include <SDL2/SDL.h>

class GUI
{
private:
  // GLOBAL STATE
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Color starting;
  SDL_Color ending;

  Graph *graph;

  int xNodes;
  int yNodes;
  int cellSize;
  int gridWidth;
  int gridHeight;
  int xPadding;
  int yPadding;
  int scrollSensitivity;
  int yOffset;
  int pathsFound;
  int durationTotal;
  int snapshotNumber;
  int nodesSelected;

  // bool showSnapshots;
  GUI_MODE mode;
  bool selectingNodesAllowed;
  bool autoMode;

  std::vector<Node *> forbiddenNodes;
  std::set<Node *> waypoints;
  std::vector<Node *> selectedNodes;
  std::vector<Node *> pathNodes;
  std::vector<std::vector<Node *> *> pathNodesSnapshots;
  // std::vector<Node *> VGPathNodes;
  // std::vector<std::vector<Node *> *> VGPathNodesSnapshots;

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
  SDL_Color VGPastPathsColors;

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
  void drawWaypoints();

  void drawLine(SDL_Renderer *renderer, Node *a, Node *b, SDL_Color color);
  void drawVG();
  void drawVGSnapshots();
  void drawLOS(Node *a, Node *b);
  bool clickedInsideCell(Node *node, int eventX, int eventY);
  void drawNormalSnapshots();
  void drawNormalPath();
  void drawVGPath();
  void drawLinesBetweenNodes(std::vector<Node *> nodes, SDL_Color pathColor);
  void drawCell(SDL_Renderer *renderer, Node *node);
  void drawLine(SDL_Renderer *renderer, Node *a, Node *b);
  void drawMain();

  void drawLOS();
  void clearData();

  /////////////////////////////////////////////////////////////////////////////
  // CALCULATIONS
  /////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////
  // ACTIONS
  /////////////////////////////////////////////////////////////////////////////
  void drawVectorNodes(std::vector<Node *> nodes, SDL_Color pathColor, SDL_Color startingNodeColor, SDL_Color endingNodeColor);

public:
  GUI(Graph *graph, int cellSize);
  int run();
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