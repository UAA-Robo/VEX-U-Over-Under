#pragma once
#include <iostream>
#include <chrono>
#include "GUI.h"

//=============================================================================
//=============================================================================
// PUBLIC METHODS
//=============================================================================
//=============================================================================

GUI::GUI(Graph *graph, int cellSize)
{
  init(graph, nullptr, cellSize);
  run();
}

GUI::GUI(Graph *graph, Graph *graph2, int cellSize)
{
  init(graph, graph2, cellSize);
  run();
}

void GUI::init(Graph *graph, Graph *graph2, int cellSize)
{
  this->graph = graph;
  this->graph2 = graph2;
  xNodes = graph->xNodes;
  yNodes = graph->yNodes;
  this->cellSize = cellSize;
  gridWidth = (xNodes * cellSize) + 1;
  gridHeight = (yNodes * cellSize) + 1;
  xPadding = 100;
  yPadding = 100;
  yOffset = 0;
  scrollSensitivity = 100;
  pathsFound = 0;
  durationTotal = 0;
  snapshotNumber = 0;
  nodesSelected = 0;
  // showSnapshots = true;
  showSnapshots = false;
  selectingNodesAllowed = false;
  autoMode = false;

  forbiddenNodes = graph->getForbiddenNodes();
  // waypoints = graph->getWaypoints();
  waypoints = graph->waypoints;

  {
    SDL_Window *window;
    SDL_Renderer *renderer;
    this->renderer = renderer;
    this->window = window;
  }

  gridCursor = {0, 0, cellSize, cellSize};
  quit = SDL_FALSE;
  mouse_active = SDL_FALSE;
  mouse_hover = SDL_FALSE;
  gridBackgroundColor = {233, 233, 233, 255};
  gridLineColor = {200, 200, 200, 255};
  gridCursorColor = {200, 200, 200, 255};
  // gridCursorColor = {200, 200, 200, 255};
  // gridCursorColor = {160, 160, 160, 255};
  forbiddenNodesColor = {255, 0, 0, 255};
  pathNodesColor = {0, 255, 0, 255};
  pathNodesPastColor = {0, 0, 255, 255};
  starting = {255, 255, 0, 255};
  ending = {0, 0, 0, 255};
  VGPathColor = {0, 255, 0, 255};
  VGPathOldColor = {0, 0, 255, 255};

  // // Dark theme.
  // //  gridBackgroundColor = {22, 22, 22, 255};
  // //  gridLineColor = {44, 44, 44, 255};
  // //  gridCursorColor = {44, 44, 44, 255};
  // //  gridCursorColor = {255, 255, 255, 255};

  // // Light Theme.
  // gridBackgroundColor = {233, 233, 233, 255};
  // gridLineColor = {200, 200, 200, 255};
  // gridCursorColor = {200, 200, 200, 255};
  // // gridCursorColor = {160, 160, 160, 255};

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Initialize SDL: %s", SDL_GetError());
    // return EXIT_FAILURE;
  }

  if (SDL_CreateWindowAndRenderer(gridWidth, gridHeight, 0, &window, &renderer) < 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Create window and renderer: %s", SDL_GetError());
    // return EXIT_FAILURE;
  }

  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
  SDL_SetWindowTitle(window, "Pathfinding");

  LOS = false;
  a = nullptr;
  b = nullptr;
}

void GUI::drawVG()
{
  SDL_SetRenderDrawColor(renderer, VGPathColor.r, VGPathColor.g, VGPathColor.b, VGPathColor.a);
  // std::cout << "DDDDDDDDDDDDDDD\n";
  // std::cout << "DDDDDDDDDDDDDDD\n";
  // for (int i = 0; i < VGPathNodes.size() - 1; i++)
  // {
  //   int x1 = (VGPathNodes[i]->x * cellSize) + xPadding + (cellSize / 2);
  //   int y1 = (VGPathNodes[i]->y * cellSize) + yOffset + yPadding + (cellSize / 2);
  //   int x2 = (VGPathNodes[i + 1]->x * cellSize) + xPadding + (cellSize / 2);
  //   int y2 = (VGPathNodes[i + 1]->y * cellSize) + yOffset + yPadding + (cellSize / 2);

  //   SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
  // }
  for (int i = 0; i < pathNodes.size() - 1; i++)
  {
    int x1 = (pathNodes[i]->x * cellSize) + xPadding + (cellSize / 2);
    int y1 = (pathNodes[i]->y * cellSize) + yOffset + yPadding + (cellSize / 2);
    int x2 = (pathNodes[i + 1]->x * cellSize) + xPadding + (cellSize / 2);
    int y2 = (pathNodes[i + 1]->y * cellSize) + yOffset + yPadding + (cellSize / 2);

    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
  }
  // std::cout << "DDDDDDDDDDDDDDD\n";

  drawCell(renderer, pathNodes.front(), ending);
  drawCell(renderer, pathNodes.back(), starting);
  // std::cout << "DDDDDDDDDDDDDDD\n";
}

void GUI::drawVGSnapshots()
{

  if (showSnapshots)
  {
    if (VGPathNodesSnapshots.size() == 0 && snapshotNumber != 0)
    {
      snapshotNumber = 0;
    }
    if (snapshotNumber >= VGPathNodesSnapshots.size())
    {
      snapshotNumber = VGPathNodesSnapshots.size() - 1;
    }
    // TODO - start, left, shows full path: feature or bug?
    // only of you can go past end snapshot into first snapshot
    else if (snapshotNumber < 0)
    {
      snapshotNumber = 0;
    }
    if (VGPathNodesSnapshots.size() > 0)
    {
      for (int s = 0; s <= snapshotNumber; s++)
      {
        for (int n = 0; n < VGPathNodesSnapshots.at(s)->size() - 1; n++)
        {
          int x1 = (VGPathNodesSnapshots.at(s)->at(n)->x * cellSize) + xPadding + (cellSize / 2);
          int y1 = (VGPathNodesSnapshots.at(s)->at(n)->y * cellSize) + yOffset + yPadding + (cellSize / 2);
          int x2 = (VGPathNodesSnapshots.at(s)->at(n + 1)->x * cellSize) + xPadding + (cellSize / 2);
          int y2 = (VGPathNodesSnapshots.at(s)->at(n + 1)->y * cellSize) + yOffset + yPadding + (cellSize / 2);

          SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

          // drawCell(renderer, VGPathNodesSnapshots.at(s)->at(n), pathNodesPastColor);

          // if (s == snapshotNumber)
          // {
          // drawCell(renderer, VGPathNodesSnapshots.at(s)->at(n), pathNodesColor);
          // }
          // else
          // {
          // drawCell(renderer, VGPathNodesSnapshots.at(s)->at(n), pathNodesPastColor);
          // }
        }
      }

      // SDL_Color starting = {255, 255, 0, 255};
      // SDL_Color ending = {0, 255, 255, 255};
      drawCell(renderer, VGPathNodesSnapshots.back()->front(), ending);
      drawCell(renderer, VGPathNodesSnapshots.back()->back(), starting);
    }
  }

  SDL_SetRenderDrawColor(renderer, VGPathColor.r, VGPathColor.g, VGPathColor.b, VGPathColor.a);
  for (int i = 0; i < VGPathNodes.size() - 1; i++)
  {
    int x1 = (VGPathNodes[i]->x * cellSize) + xPadding + (cellSize / 2);
    int y1 = (VGPathNodes[i]->y * cellSize) + yOffset + yPadding + (cellSize / 2);
    int x2 = (VGPathNodes[i + 1]->x * cellSize) + xPadding + (cellSize / 2);
    int y2 = (VGPathNodes[i + 1]->y * cellSize) + yOffset + yPadding + (cellSize / 2);

    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
  }

  drawCell(renderer, pathNodes.front(), ending);
  drawCell(renderer, pathNodes.back(), starting);
}

void GUI::getPath(Node *a, Node *b)
{
  pathNodes.clear();

  autoMode = false;
  selectingNodesAllowed = false;

  auto start = std::chrono::high_resolution_clock::now();
  // pathNodes = graph->getVGPath(selectedNodes[0], selectedNodes[1]);
  pathNodes = graph->getPath(selectedNodes[0], selectedNodes[1]);
  auto stop = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cout << "Time: " << duration.count() << std::endl;
  pathsFound++;
  durationTotal += duration.count();
}

void GUI::getPathSnapshots(Node *a, Node *b)
{
  for (std::vector<Node *> *vec : pathNodesSnapshots)
  {
    delete vec;
  }
  pathNodesSnapshots.clear();

  snapshotNumber = 0;
  autoMode = false;
  selectingNodesAllowed = false;

  // pathNodesSnapshots = graph->getVGPathSnapshots(selectedNodes[0], selectedNodes[1]);
  pathNodesSnapshots = graph->getPathSnapshots(selectedNodes[0], selectedNodes[1]);
}

void GUI::getRandomPath()
{
  pathNodes.clear();

  autoMode = false;
  selectingNodesAllowed = false;

  auto start = std::chrono::high_resolution_clock::now();
  // pathNodes = graph->getVGRandomPath();
  pathNodes = graph->getRandomPath();
  auto stop = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  // std::cout << "B\n";
  std::cout << "Time: " << duration.count() << std::endl;
  pathsFound++;
  durationTotal += duration.count();
}

void GUI::getRandomPathSnapshots()
{
  for (std::vector<Node *> *vec : pathNodesSnapshots)
  {
    delete vec;
  }
  pathNodesSnapshots.clear();

  snapshotNumber = 0;
  autoMode = false;
  selectingNodesAllowed = false;

  // pathNodesSnapshots = graph->getVGRandomPathSnapshots();
  pathNodesSnapshots = graph->getRandomPathSnapshots();
  // std::cout << pathNodesSnapshots.size() << std::endl;
}

// TODO - refactor out shared lines
void GUI::selectNodes(SDL_Event &event)
{
  if (showSnapshots)
  {
    autoMode = false;
    snapshotNumber = 0;
    bool nodeAllowed = true;
    int x = ((event.motion.x - xPadding) / cellSize) * cellSize;
    int y = ((event.motion.y - yPadding - yOffset) / cellSize) * cellSize;

    for (Node *forbiddenNode : forbiddenNodes)
    {
      if (forbiddenNode->x * cellSize == x && forbiddenNode->y * cellSize == y)
      {
        nodeAllowed = false;
        break;
      }
    }
    if (nodeAllowed)
    {
      bool isNotDuplicate = true;
      for (Node *node : selectedNodes)
      {
        if (node->x * cellSize == x && node->y * cellSize == y)
        {
          isNotDuplicate = false;
          break;
        }
      }
      if (isNotDuplicate)
      {
        nodesSelected++;

        // selectedNodes.push_back(graph->getNode((x * cellSize) / cellSize,
        //                                        (y * cellSize) / cellSize));
        selectedNodes.push_back(graph->getNode(x / cellSize, y / cellSize));
        if (nodesSelected == 2)
        {
          selectingNodesAllowed = false;

          auto start = std::chrono::high_resolution_clock::now();
          // pathNodesSnapshots = graph->getVGPathSnapshots(selectedNodes[0], selectedNodes[1]);
          pathNodesSnapshots = graph->getPathSnapshots(selectedNodes[0], selectedNodes[1]);
          auto stop = std::chrono::high_resolution_clock::now();

          auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
          // std::cout << duration.count() << std::endl;
          // pathsFound++;
          // durationTotal += duration.count();
        }
        else if (nodesSelected > 2 || nodesSelected < 0)
        {
          throw "Z";
        }
      }
    }
    // break;
  }
  else
  {
    autoMode = false;
    bool nodeAllowed = true;
    int x = ((event.motion.x - xPadding) / cellSize) * cellSize;
    int y = ((event.motion.y - yPadding - yOffset) / cellSize) * cellSize;

    for (Node *forbiddenNode : forbiddenNodes)
    {
      if (forbiddenNode->x * cellSize == x && forbiddenNode->y * cellSize == y)
      {
        nodeAllowed = false;
      }
    }
    if (nodeAllowed)
    {
      bool isNotDuplicate = true;
      for (Node *node : selectedNodes)
      {
        if (node->x * cellSize == x && node->y * cellSize == y)
        {
          isNotDuplicate = false;
        }
      }
      if (isNotDuplicate)
      {
        nodesSelected++;

        selectedNodes.push_back(graph->getNode(x / cellSize, y / cellSize));
        if (nodesSelected == 2)
        {
          selectingNodesAllowed = false;
          auto start = std::chrono::high_resolution_clock::now();
          // pathNodes = graph->getVGPath(selectedNodes[0], selectedNodes[1]);
          pathNodes = graph->getPath(selectedNodes[0], selectedNodes[1]);
          // LOS = graph->hasLOS(selectedNodes[0], selectedNodes[1]);
          // drawLOS(selectedNodes[0], selectedNodes[1]);
          // a = selectedNodes[0];
          // b = selectedNodes[1];

          auto stop = std::chrono::high_resolution_clock::now();

          auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
          std::cout << "Time: " << duration.count() << std::endl;
          pathsFound++;
          durationTotal += duration.count();
        }
        else if (nodesSelected > 2 || nodesSelected < 0)
        {
          throw "Z";
        }
      }
    }
    // break;
  }
}

void GUI::drawLOS(Node *a, Node *b)
{
  // std::cout << "Z\n";

  SDL_Color fal = {255, 0, 0, 255};
  SDL_Color tru = {0, 255, 0, 255};
  // a->println();
  // b->println();
  if (LOS)
  {
    // std::cout << "T\n";
    drawCell(renderer, a, tru);
    drawCell(renderer, b, tru);
  }
  else
  {
    // std::cout << "F\n";
    drawCell(renderer, a, fal);
    drawCell(renderer, b, fal);
  }

  std::cout << "Z\n";
}

void GUI::drawBackground()
{
  // Draw grid background.
  SDL_SetRenderDrawColor(renderer, gridBackgroundColor.r, gridBackgroundColor.g,
                         gridBackgroundColor.b, gridBackgroundColor.a);
  SDL_RenderClear(renderer);
}

void GUI::drawWaypoints()
{
  SDL_Color waypointsColor = {0, 0, 0, 255};
  // SDL_SetRenderDrawColor(waypointsColor.r, waypointsColor.g, waypointsColor.b, waypointsColor.a);
  for (Node *waypoint : waypoints)
  {
    drawCell(renderer, waypoint, waypointsColor);
  }
}

void GUI::drawPath()
{
  if (showSnapshots == false && pathNodes.size() > 0)
  {
    // SDL_Color pathNodesColor = {0, 255, 0, 255};
    // TODO - start, left, shows full path: feature or bug?
    // only of you can go past end snapshot into first snapshot
    for (Node *node : pathNodes)
    {
      drawCell(renderer, node, pathNodesColor);
    }

    // TODO - redeclaring every loop, move outside loop
    // SDL_Color starting = {255, 255, 0, 255};
    // SDL_Color ending = {0, 255, 255, 255};
    drawCell(renderer, pathNodes.front(), ending);
    drawCell(renderer, pathNodes.back(), starting);
  }
}

void GUI::drawSnapshots()
{
  if (showSnapshots)
  {
    // SDL_Color pathNodesColor = {0, 255, 0, 255};
    // SDL_Color pathNodesPastColor = {0, 0, 255, 255};
    if (pathNodesSnapshots.size() == 0 && snapshotNumber != 0)
    {
      snapshotNumber = 0;
    }
    if (snapshotNumber >= pathNodesSnapshots.size())
    {
      snapshotNumber = pathNodesSnapshots.size() - 1;
    }
    // TODO - start, left, shows full path: feature or bug?
    // only of you can go past end snapshot into first snapshot
    else if (snapshotNumber < 0)
    {
      snapshotNumber = 0;
    }
    if (pathNodesSnapshots.size() > 0)
    {
      for (int s = 0; s <= snapshotNumber; s++)
      {
        for (int n = 0; n < pathNodesSnapshots.at(s)->size(); n++)
        {

          if (s == snapshotNumber)
          {
            drawCell(renderer, pathNodesSnapshots.at(s)->at(n), pathNodesColor);
          }
          else
          {
            drawCell(renderer, pathNodesSnapshots.at(s)->at(n), pathNodesPastColor);
          }
        }
      }

      // SDL_Color starting = {255, 255, 0, 255};
      // SDL_Color ending = {0, 255, 255, 255};
      drawCell(renderer, pathNodesSnapshots.back()->front(), ending);
      drawCell(renderer, pathNodesSnapshots.back()->back(), starting);
    }
  }
}

void GUI::drawForbiddenNodes()
{

  for (Node *node : forbiddenNodes)
  {
    drawCell(renderer, node, forbiddenNodesColor);
  }
}

void GUI::drawCursor()
{
  // Draw grid ghost cursor.
  if (mouse_active && mouse_hover)
  {
    SDL_SetRenderDrawColor(renderer, gridCursorColor.r,
                           gridCursorColor.g,
                           gridCursorColor.b,
                           gridCursorColor.a);
    SDL_RenderFillRect(renderer, &gridCursor);
  }
}

void GUI::drawGrid()
{

  // Draw grid lines.
  SDL_SetRenderDrawColor(renderer, gridLineColor.r, gridLineColor.g,
                         gridLineColor.b, gridLineColor.a);

  for (int x = 0; x < 1 + graph->xNodes * cellSize;
       x += cellSize)
  {
    SDL_RenderDrawLine(renderer, x + xPadding, 0 + yOffset + yPadding, x + xPadding, gridHeight + yOffset + yPadding);
  }

  for (int y = 0; y < 1 + graph->yNodes * cellSize;
       y += cellSize)
  {
    SDL_RenderDrawLine(renderer, 0 + xPadding, y + yOffset + yPadding, gridWidth + xPadding, y + yOffset + yPadding);
  }
}

void GUI::scroll(SDL_Event &event)
{

  if (event.wheel.y > 0)
  {
    yOffset = yOffset + (event.wheel.y * scrollSensitivity);
  }
  // scroll down
  else if (event.wheel.y < 0)
  {
    yOffset = yOffset + (event.wheel.y * scrollSensitivity);
  }
  else
  {
    // yOffset = 0;
  }
}

void GUI::moveCursor(SDL_Event &event)
{

  gridCursor.x = (((event.motion.x) / cellSize) * cellSize);
  gridCursor.y = ((event.motion.y / cellSize) * cellSize);
  if ((event.motion.x >= xPadding && event.motion.x <= xPadding + gridWidth) &&
      (event.motion.y >= yPadding + yOffset && event.motion.y <= yPadding + yOffset + gridHeight))
  {

    gridCursor.x = (((event.motion.x - xPadding) / cellSize) * cellSize) + xPadding;
    gridCursor.y = (((event.motion.y - yPadding - yOffset) / cellSize) * cellSize) + yPadding + yOffset;
  }
  else
  {
    gridCursor.x = -1000;
    gridCursor.y = -1000;
  }

  if (!mouse_active)
  {
    mouse_active = SDL_TRUE;
  }
}
void GUI::resetGrid()
{
  if (showSnapshots)
  {
    snapshotNumber = 0;
    selectingNodesAllowed = true;
    nodesSelected = 0;
    for (std::vector<Node *> *vec : pathNodesSnapshots)
    {
      vec->clear();
      delete vec;
    }
    pathNodesSnapshots.clear();
    selectedNodes.clear();
    autoMode = false;
  }
  else
  {
    selectingNodesAllowed = true;
    nodesSelected = 0;
    pathNodes.clear();
    selectedNodes.clear();
    autoMode = false;
  }

  a = nullptr;
  b = nullptr;
  LOS = false;
}

void GUI::generatePath()
{
  // if (showSnapshots)
  // {
  //   getRandomPathSnapshots();
  // }
  // else
  // {
  //   getRandomPath();
  // }

  if (showSnapshots)
  {
    getRandomPathSnapshots();
  }
  else
  {
    getRandomPath();
  }
}

void GUI::switchModes()
{
  if (showSnapshots)
  {
    for (std::vector<Node *> *vec : pathNodesSnapshots)
    {
      delete vec;
    }
    pathNodesSnapshots.clear();
    snapshotNumber = 0;
    autoMode = false;
    selectingNodesAllowed = false;
  }
  else
  {
    pathNodes.clear();
    autoMode = false;
    selectingNodesAllowed = false;
  }
  showSnapshots = !showSnapshots;
}

void GUI::eventLoop()
{
  // std::cout << "EEEEEEEEEEEEEEEEE\n";
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
          autoMode = true;
          break;
        case SDLK_s:
        case SDLK_DOWN:
          autoMode = false;
          break;
        case SDLK_a:
        case SDLK_LEFT:
          if (snapshotNumber - 1 > 0)
          {
            std::cout << "DECREMENT\n";
            snapshotNumber--;
          }
          break;
        case SDLK_d:
        case SDLK_RIGHT:
          if (snapshotNumber + 1 < pathNodesSnapshots.size())
          {
            std::cout << "INCREMENT\n";
            // std::cout << pathNodesSnapshots.size();
            snapshotNumber++;
          }
          break;
        case SDLK_ESCAPE:
          quit = SDL_TRUE;
          break;
        case SDLK_SPACE:
          generatePath();
          break;
        case SDLK_TAB:
          switchModes();
          break;
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_RIGHT)
        {
          resetGrid();
        }
        else if (event.button.button == SDL_BUTTON_LEFT && selectingNodesAllowed)
        {
          selectNodes(event);
        }
        break;
      case SDL_MOUSEMOTION:
        moveCursor(event);
        break;
      case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_ENTER && !mouse_hover)
          mouse_hover = SDL_TRUE;
        else if (event.window.event == SDL_WINDOWEVENT_LEAVE && mouse_hover)
          mouse_hover = SDL_FALSE;
        break;
      case SDL_MOUSEWHEEL:
        scroll(event);
        // scroll up
        break;
      case SDL_QUIT:
        quit = SDL_TRUE;
        break;
      }
    }
    // std::cout << "EEEEEEEEEEEEEEEEE\n";

    if (autoMode)
    {
      snapshotNumber++;
    }

    drawBackground();
    drawGrid();
    drawForbiddenNodes();
    drawWaypoints();
    // drawSnapshots();
    // drawPath();
    // std::cout << "EEEEEEEEEEEEEEEEE\n";
    drawVG();
    // std::cout << "EEEEEEEEEEEEEEEEE\n";
    // drawVGSnapshots();
    // std::cout << "EEEEEEEEEEEEEEEEE\n";
    drawCursor();
    if (a != nullptr && b != nullptr)
    {
      drawLOS(a, b);
    }

    // std::cout << waypoints.size() << std::endl;

    SDL_RenderPresent(renderer);
  }
}

int GUI::run()
{
  if (showSnapshots)
  {
    getRandomPathSnapshots();
  }
  else
  {
    getRandomPath();
  }

  // std::cout << pathNodes.size() << std::endl;

  eventLoop();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}

//=============================================================================
//=============================================================================
// PRIVATE METHODS
//=============================================================================
//=============================================================================

void GUI::drawCell(SDL_Renderer *renderer, Node *node, SDL_Color color)
{
  SDL_Rect cell{
      .x = (node->x * cellSize) + xPadding,
      .y = (node->y * cellSize) + yOffset + yPadding,
      .w = cellSize,
      .h = cellSize,
  };

  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect(renderer, &cell);
}

void GUI::drawCellSecond(SDL_Renderer *renderer, Node *node, SDL_Color color, int xDifference)
{
  SDL_Rect cell{
      .x = (node->x * cellSize) + (xPadding * 2),
      .y = (node->y * cellSize) + yOffset + yPadding,
      .w = cellSize,
      .h = cellSize,
  };

  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect(renderer, &cell);
}
