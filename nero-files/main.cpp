#include <map>
#include <vector>
#include <cmath>
#include <iostream>
// #include <utility>
// #include <tuple>
#include <random>
// #include <chrono>
#include <fstream>
// #include <ostream>
// #include <string>
// #include <cstdio>
#include <SDL2/SDL.h>
#include <stdexcept>

#include "Node.h"
#include "Graph.h"
#include "Triangle.h"
#include "Rectangle.h"

// in inches
double const FIELD_SIZE = 138.73;
// length and width of nodes in inches, for real-life movement
double const NODE_SIZE = 1;
// length and width of nodes in pixels, for visualization
int const CELL_SIZE = 5;
// taking advantage of the map's symmetry. Not exact, but close to real values. Used for computing where to place obstacles
double const ZONE_SIZE = FIELD_SIZE / 6;
int const BUFFER_NODES_NUMBER = 2;
// number of nodes per row and column
int const X_NODES = FIELD_SIZE / NODE_SIZE;
int const Y_NODES = FIELD_SIZE / NODE_SIZE;

int heuristic(Node *currentNode, Node *destination)
{
  int dx = abs(currentNode->x - destination->x);
  int dy = abs(currentNode->y - destination->y);

  return 10 * (dx + dy) + (14 - 2 * 10) * std::min(dx, dy);
}

std::vector<Node *> reconstructPath(Node *currentNode)
{
  std::vector<Node *> path;
  path.push_back(currentNode);

  while (currentNode->parent != NULL)
  {
    currentNode = currentNode->parent;
    path.push_back(currentNode);
  }

  return path;
}

int getEdgeCost(Node *a, Node *b)
{
  // if nodes are diagonal neighbors
  if ((b->x == a->x + 1 && b->y == a->y) || (b->x == a->x - 1 && b->y == a->y) || (b->x == a->x && b->y == a->y + 1) || (b->x == a->x && b->y == a->y - 1))
  {
    return 10;
  }
  // if nodes are orthogonal neighbors
  else if ((b->x == a->x + 1 && b->y == a->y + 1) || (b->x == a->x + 1 && b->y == a->y - 1) || (b->x == a->x - 1 && b->y == a->y + 1) || (b->x == a->x - 1 && b->y == a->y - 1))
  {
    return 14;
  }
  else
  {
    throw std::runtime_error("ERROR 001");
  }
}

std::vector<Node *> AStar(Graph graph, Node *origin, Node *destination, int (*heuristic)(Node *, Node *))
{
  std::set<Node *> frontier;
  std::map<Node *, int> gScores;
  std::map<Node *, int> fScores;

  frontier.insert(origin);
  gScores[origin] = 0;
  fScores[origin] = heuristic(origin, destination);

  while (frontier.size() > 0)
  {
    int lowestFScore = 2147483647;
    Node *currentNode;

    for (Node *node : frontier)
    {
      if (fScores.find(node) == fScores.end() || fScores[node] < lowestFScore)
      {
        lowestFScore = fScores[node];
        currentNode = node;
      }
    }

    frontier.erase(currentNode);

    if (currentNode == destination)
    {
      return reconstructPath(currentNode);
    }

    for (Node *neighbor : currentNode->neighbors)
    {
      int neighborGScore = gScores[currentNode] + getEdgeCost(currentNode, neighbor);

      if (gScores.find(neighbor) == gScores.end() || neighborGScore < gScores[neighbor])
      {
        neighbor->parent = currentNode;
        gScores[neighbor] = neighborGScore;
        fScores[neighbor] = neighborGScore + heuristic(neighbor, destination);

        if (frontier.find(neighbor) == frontier.end())
        {
          frontier.insert(neighbor);
        }
      }
    }
  }
  throw std::runtime_error("ERROR 002");
}

std::vector<Node *> ThetaStar(Graph graph, Node *origin, Node *destination, int (*h)(Node *, Node *))
{
  std::set<Node *> frontier;
  std::set<Node *> explored;
  std::map<Node *, int> gScores;
  std::map<Node *, int> fScores;

  frontier.insert(origin);
  gScores[origin] = 0;
  fScores[origin] = heuristic(origin, destination);

  while (frontier.size() > 0)
  {
    int lowestFScore = 2147483647;
    Node *currentNode;

    for (Node *node : frontier)
    {
      if (fScores.find(node) == fScores.end() || fScores[node] < lowestFScore)
      {
        lowestFScore = fScores[node];
        currentNode = node;
      }
    }

    frontier.erase(currentNode);

    if (currentNode == destination)
    {
      return reconstructPath(currentNode);
    }

    for (Node *neighbor : currentNode->neighbors)
    {
      int neighborGScore = gScores[currentNode] + getEdgeCost(currentNode, neighbor);
      if (gScores.find(neighbor) == gScores.end() || neighborGScore < gScores[neighbor])
      {
        neighbor->parent = currentNode;
        gScores[neighbor] = neighborGScore;
        fScores[neighbor] = neighborGScore + heuristic(neighbor, destination);
        if (frontier.find(neighbor) == frontier.end())
        {
          frontier.insert(neighbor);
        }
      }
    }
  }
  throw std::runtime_error("ERROR 003");
}

std::tuple<int, int> GPStoLocal(std::tuple<int, int> point)
{
  return std::make_tuple(std::get<1>(point) + 30, std::get<0>(point) + 30);
}

std::tuple<int, int> LocaltoGPS(std::tuple<int, int> point)
{
  return std::make_tuple(std::get<1>(point) - 30, std::get<0>(point) - 30);
}

int getPositionRelative(Node *a, Node *b)
{
  int ax = a->x;
  int ay = a->y;
  int bx = b->x;
  int by = b->y;

  // b is top left of a
  if (bx < ax && by < ay)
  {
    return 0;
  }
  // b is top right of a
  else if (bx > ax && by < ay)
  {
    return 1;
  }
  // b is bottom left of a
  else if (bx < ax && by > ay)
  {
    return 2;
  }
  // b is bottom right of a
  else if (bx > ax && by > ay)
  {
    return 3;
  }
  else
  {
    throw std::runtime_error("ERROR 004");
  }
}

void forbidTriangle(Graph graph, Node *a, Node *b)
{

  int bx = b->x;
  int by = b->y;
  int x = a->x;
  int y = a->y;
  int xInitial = x;

  // b is top left of a
  if (getPositionRelative(a, b) == 0)
  {
    while (x >= bx && y >= by)
    {
      for (int xx = xInitial; xx >= x; xx--)
      {
        graph.getNode(xx, y)->forbid();
      }

      x--;
      y--;
    }
  }
  // b is top right of a
  else if (getPositionRelative(a, b) == 1)
  {
    while (x <= bx && y >= by)
    {
      for (int xx = xInitial; xx <= x; xx++)
      {
        graph.getNode(xx, y)->forbid();
      }

      x++;
      y--;
    }
  }
  // b is bottom left of a
  else if (getPositionRelative(a, b) == 2)
  {
    while (x >= bx && y <= by)
    {
      for (int xx = xInitial; xx >= x; xx--)
      {
        graph.getNode(xx, y)->forbid();
      }

      x--;
      y++;
    }
  }
  // b is bottom right of a
  else if (getPositionRelative(a, b) == 3)
  {
    while (x <= bx && y <= by)
    {
      for (int xx = xInitial; xx <= x; xx++)
      {
        graph.getNode(xx, y)->forbid();
      }

      x++;
      y++;
    }
  }
}

void drawCell(SDL_Renderer *renderer, Node *node, SDL_Color color)
{
  SDL_Rect cell{
      .x = node->x * CELL_SIZE,
      .y = node->y * CELL_SIZE,
      .w = CELL_SIZE,
      .h = CELL_SIZE,
  };

  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect(renderer, &cell);
}

int SDL(Graph graph, std::vector<Node *> forbiddenNodes, std::vector<Node *> pathNodes)
{
  std::vector<Node *> selectedNodes;
  bool selectingNodesAllowed = false;

  // + 1 so that the last grid lines fit in the screen.
  int window_width = (graph.xNodes * CELL_SIZE) + 1;
  int window_height = (graph.yNodes * CELL_SIZE) + 1;

  // Place the grid cursor in the middle of the screen.
  SDL_Rect grid_cursor = {
      // .x = (grid_width - 1) / 2 * CELL_SIZE,
      // .y = (grid_height - 1) / 2 * CELL_SIZE,
      .w = CELL_SIZE,
      .h = CELL_SIZE,
  };

  // The cursor ghost is a cursor that always shows in the cell below the
  // mouse cursor.
  // SDL_Rect grid_cursor_ghost = {grid_cursor.x, grid_cursor.y, CELL_SIZE,
  //                               CELL_SIZE};
  SDL_Rect grid_cursor_ghost = {0, 0, CELL_SIZE,
                                CELL_SIZE};

  // Dark theme.
  // SDL_Color grid_background = {22, 22, 22, 255}; // Barely Black
  // SDL_Color grid_line_color = {44, 44, 44, 255}; // Dark grey
  // SDL_Color grid_cursor_ghost_color = {44, 44, 44, 255};
  // SDL_Color grid_cursor_color = {255, 255, 255, 255}; // White

  // Light Theme.
  SDL_Color grid_background = {233, 233, 233, 255}; // Barely white
  SDL_Color grid_line_color = {200, 200, 200, 255}; // Very light grey
  SDL_Color grid_cursor_ghost_color = {200, 200, 200, 255};
  SDL_Color grid_cursor_color = {160, 160, 160, 255}; // Grey

  SDL_Color forbiddenNodesColor = {255, 0, 0, 255};

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Initialize SDL: %s",
                 SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_Window *window;
  SDL_Renderer *renderer;
  if (SDL_CreateWindowAndRenderer(window_width, window_height, 0, &window,
                                  &renderer) < 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Create window and renderer: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
  // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
  SDL_SetWindowTitle(window, "Pathfinding");

  SDL_bool quit = SDL_FALSE;
  SDL_bool mouse_active = SDL_FALSE;
  SDL_bool mouse_hover = SDL_FALSE;

  while (!quit)
  {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_w:
        case SDLK_UP:
          // grid_cursor.y -= CELL_SIZE;
          break;
        case SDLK_s:
        case SDLK_DOWN:
          // grid_cursor.y += CELL_SIZE;
          break;
        case SDLK_a:
        case SDLK_LEFT:
          // grid_cursor.x -= CELL_SIZE;
          break;
        case SDLK_d:
        case SDLK_RIGHT:
          // grid_cursor.x += CELL_SIZE;
          break;
        case SDLK_ESCAPE:
          quit = SDL_TRUE;
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
      {
        // if (selectingNodesAllowed)
        // {
        //   bool nodeAllowed = true;
        //   int xx = (event.motion.x / CELL_SIZE) * CELL_SIZE;
        //   int yy = (event.motion.y / CELL_SIZE) * CELL_SIZE;

        //   for (Node *forbiddenNode : forbiddenNodes)
        //   {
        //     if (forbiddenNode->x * CELL_SIZE == xx && forbiddenNode->y * CELL_SIZE == yy)
        //     {
        //       nodeAllowed = false;
        //     }
        //   }
        //   if (nodeAllowed)
        //   {
        //     bool isNotDuplicate = true;
        //     for (SDL_Rect node : selectedNodes)
        //     {
        //       if (node.x == xx && node.y == yy)
        //       {
        //         isNotDuplicate = false;
        //       }
        //     }
        //     if (isNotDuplicate)
        //     {

        //       selectedNodes.push_back(SDL_Rect{
        //           .x = (event.motion.x / CELL_SIZE) * CELL_SIZE,
        //           .y = (event.motion.y / CELL_SIZE) * CELL_SIZE,
        //           .w = CELL_SIZE,
        //           .h = CELL_SIZE,
        //       });
        //     }
        //   }
        // }
      }
      break;

        // if (grid_cursor.x == NULL)
        // {
        //   std::cout << grid_cursor.x;
        //   grid_cursor.x = (event.motion.x / CELL_SIZE) * CELL_SIZE;
        //   grid_cursor.y = (event.motion.y / CELL_SIZE) * CELL_SIZE;
        // }
        // grid_cursor.x = (event.motion.x / CELL_SIZE) * CELL_SIZE;
        // grid_cursor.y = (event.motion.y / CELL_SIZE) * CELL_SIZE;
        // SDL_Rect cell = {

      case SDL_MOUSEMOTION:
        grid_cursor_ghost.x = (event.motion.x / CELL_SIZE) * CELL_SIZE;
        grid_cursor_ghost.y = (event.motion.y / CELL_SIZE) * CELL_SIZE;

        if (!mouse_active)
        {
          mouse_active = SDL_TRUE;
        }
        break;
      case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_ENTER && !mouse_hover)
          mouse_hover = SDL_TRUE;
        else if (event.window.event == SDL_WINDOWEVENT_LEAVE && mouse_hover)
          mouse_hover = SDL_FALSE;
        break;
      case SDL_QUIT:
        quit = SDL_TRUE;
        break;
      }
    }

    // Draw grid background.
    SDL_SetRenderDrawColor(renderer, grid_background.r, grid_background.g,
                           grid_background.b, grid_background.a);
    SDL_RenderClear(renderer);

    // Draw grid lines.
    SDL_SetRenderDrawColor(renderer, grid_line_color.r, grid_line_color.g,
                           grid_line_color.b, grid_line_color.a);

    for (int x = 0; x < 1 + graph.xNodes * CELL_SIZE;
         x += CELL_SIZE)
    {
      SDL_RenderDrawLine(renderer, x, 0, x, window_height);
    }

    for (int y = 0; y < 1 + graph.yNodes * CELL_SIZE;
         y += CELL_SIZE)
    {
      SDL_RenderDrawLine(renderer, 0, y, window_width, y);
    }

    // Draw grid ghost cursor.
    if (mouse_active && mouse_hover)
    {
      SDL_SetRenderDrawColor(renderer, grid_cursor_ghost_color.r,
                             grid_cursor_ghost_color.g,
                             grid_cursor_ghost_color.b,
                             grid_cursor_ghost_color.a);
      SDL_RenderFillRect(renderer, &grid_cursor_ghost);
    }

    // Draw grid cursor.
    SDL_SetRenderDrawColor(renderer, grid_cursor_color.r,
                           grid_cursor_color.g, grid_cursor_color.b,
                           grid_cursor_color.a);
    SDL_RenderFillRect(renderer, &grid_cursor);

    for (Node *node : forbiddenNodes)
    {
      drawCell(renderer, node, forbiddenNodesColor);
    }

    SDL_Color pathNodesColor = {0, 255, 0, 255};
    SDL_SetRenderDrawColor(renderer, pathNodesColor.r, pathNodesColor.g, pathNodesColor.b, pathNodesColor.a);
    for (Node *node : pathNodes)
    {
      drawCell(renderer, node, pathNodesColor);
    }

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}

void forbidRectangle(Graph graph, Node *topLeftPoint, Node *topRightPoint, Node *bottomLeftPoint)
{
  for (int y = topLeftPoint->y; y <= bottomLeftPoint->y; y++)
  {
    for (int x = topLeftPoint->x; x <= topRightPoint->x; x++)
    {
      graph.getNode(x, y)->forbid();
    }
  }
}

int main(int argv, char **args)
{
  Graph graph = Graph(X_NODES, Y_NODES);

  double const ROLLER_SIZE = ZONE_SIZE;

  double const GOAL_START_X = 2 * ZONE_SIZE;
  double const GOAL_START_Y = 0;
  double const GOAL_SIZE_X = 2 * ZONE_SIZE;
  double const GOAL_SIZE_Y = ZONE_SIZE;

  double const BAR_LEFT_START_X = ZONE_SIZE;
  double const BAR_LEFT_START_Y = 2 * ZONE_SIZE;
  double const BAR_LEFT_SIZE_X = 2.375;
  double const BAR_LEFT_SIZE_Y = 2 * ZONE_SIZE;
  double const BAR_RIGHT_START_X = 5 * ZONE_SIZE;
  double const BAR_RIGHT_START_Y = 2 * ZONE_SIZE;
  double const BAR_RIGHT_SIZE_X = 2.375;
  double const BAR_RIGHT_SIZE_Y = 2 * ZONE_SIZE;

  double const BAR_MAIN_START_X = ZONE_SIZE;
  double const BAR_MAIN_START_Y = 3 * ZONE_SIZE;
  double const BAR_MAIN_SIZE_X = 4 * ZONE_SIZE;
  double const BAR_MAIN_SIZE_Y = 2.375;

  std::vector<Triangle> forbiddenZonesTriangles;
  std::vector<Rectangle> forbiddenZonesRectangles;

  forbiddenZonesTriangles.push_back(Triangle(graph.getNode(0, 0),
                                             graph.getNode(0, ROLLER_SIZE),
                                             graph.getNode(ROLLER_SIZE, 0)));
  forbiddenZonesTriangles.push_back(Triangle(graph.getNode(FIELD_SIZE - 1, 0),
                                             graph.getNode(FIELD_SIZE - 1, ROLLER_SIZE),
                                             graph.getNode(FIELD_SIZE - 1 - ROLLER_SIZE, 0)));
  forbiddenZonesTriangles.push_back(Triangle(graph.getNode(0, FIELD_SIZE - 1),
                                             graph.getNode(0, FIELD_SIZE - 1 - ROLLER_SIZE),
                                             graph.getNode(ROLLER_SIZE, FIELD_SIZE - 1)));
  forbiddenZonesTriangles.push_back(Triangle(graph.getNode(FIELD_SIZE - 1, FIELD_SIZE - 1),
                                             graph.getNode(FIELD_SIZE - 1, FIELD_SIZE - 1 - ROLLER_SIZE),
                                             graph.getNode(FIELD_SIZE - 1 - ROLLER_SIZE, FIELD_SIZE - 1)));

  forbiddenZonesRectangles.push_back(Rectangle(graph.getNode(GOAL_START_X, GOAL_START_Y),
                                               graph.getNode(GOAL_START_X + GOAL_SIZE_X, GOAL_START_Y),
                                               graph.getNode(GOAL_START_X, GOAL_START_Y + GOAL_SIZE_Y),
                                               graph.getNode(GOAL_START_X + GOAL_SIZE_X, GOAL_START_Y + GOAL_SIZE_Y)));
  forbiddenZonesRectangles.push_back(Rectangle(graph.getNode(GOAL_START_X, FIELD_SIZE - 1 - GOAL_SIZE_Y),
                                               graph.getNode(GOAL_START_X + GOAL_SIZE_X, FIELD_SIZE - 1 - GOAL_SIZE_Y),
                                               graph.getNode(GOAL_START_X, FIELD_SIZE - 1),
                                               graph.getNode(GOAL_START_X + GOAL_SIZE_X, FIELD_SIZE - 1)));
  forbiddenZonesRectangles.push_back(Rectangle(graph.getNode(BAR_LEFT_START_X, BAR_LEFT_START_Y),
                                               graph.getNode(BAR_LEFT_START_X + BAR_LEFT_SIZE_X, BAR_LEFT_START_Y),
                                               graph.getNode(BAR_LEFT_START_X, BAR_LEFT_START_Y + BAR_LEFT_SIZE_Y),
                                               graph.getNode(BAR_LEFT_START_X + BAR_LEFT_SIZE_X, BAR_LEFT_START_Y + BAR_LEFT_SIZE_Y)));
  forbiddenZonesRectangles.push_back(Rectangle(graph.getNode(BAR_RIGHT_START_X, BAR_RIGHT_START_Y),
                                               graph.getNode(BAR_RIGHT_START_X + BAR_RIGHT_SIZE_X, BAR_RIGHT_START_Y),
                                               graph.getNode(BAR_RIGHT_START_X, BAR_RIGHT_START_Y + BAR_RIGHT_SIZE_Y),
                                               graph.getNode(BAR_RIGHT_START_X + BAR_RIGHT_SIZE_X, BAR_RIGHT_START_Y + BAR_RIGHT_SIZE_Y)));
  forbiddenZonesRectangles.push_back(Rectangle(graph.getNode(BAR_MAIN_START_X, BAR_MAIN_START_Y),
                                               graph.getNode(BAR_MAIN_START_X + BAR_MAIN_SIZE_X, BAR_MAIN_START_Y),
                                               graph.getNode(BAR_MAIN_START_X, BAR_MAIN_START_Y + BAR_MAIN_SIZE_Y),
                                               graph.getNode(BAR_MAIN_START_X + BAR_MAIN_SIZE_X, BAR_MAIN_START_Y + BAR_MAIN_SIZE_Y)));

  for (Triangle triangle : forbiddenZonesTriangles)
  {
    forbidTriangle(graph, triangle.sidePointA, triangle.sidePointB);
  }

  for (Rectangle rectangle : forbiddenZonesRectangles)
  {
    forbidRectangle(graph, rectangle.topLeftPoint, rectangle.topRightPoint, rectangle.bottomLeftPoint);
  }

  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<int> randX(0, X_NODES - 1);
  std::uniform_int_distribution<int> randY(0, Y_NODES - 1);

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
  } while (graph.getNode(originX, originY)->forbidden || graph.getNode(destinationX, destinationY)->forbidden);

  std::vector<Node *> pathNodes = AStar(graph, graph.getNode(originX, originY), graph.getNode(destinationX, destinationY), heuristic);
  std::vector<Node *> forbiddenNodes = graph.getForbiddenNodes();

  SDL(graph, forbiddenNodes, pathNodes);
}
