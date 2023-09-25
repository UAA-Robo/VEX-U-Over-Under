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
  this->graph = graph;

  xNodes = graph->xNodes;
  yNodes = graph->yNodes;
  this->cellSize = cellSize;
  gridWidth = (xNodes * cellSize) + 1;
  gridHeight = (yNodes * cellSize) + 1;
  xPadding = 100;
  yPadding = 100;
  scrollSensitivity = 100;
  yOffset = 0;
  pathsFound = 0;
  durationTotal = 0;
  snapshotNumber = 0;
  nodesSelected = 0;

  showSnapshots = false;
  selectingNodesAllowed = false;
  autoMode = false;

  forbiddenNodes = graph->getForbiddenNodes();
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
  forbiddenNodesColor = {255, 0, 0, 255};
  pathNodesColor = {0, 255, 0, 255};
  pathNodesPastColor = {0, 0, 255, 255};
  starting = {255, 255, 0, 255};
  ending = {0, 0, 0, 255};

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

  run();
}

void GUI::drawNormalPath()
{
  // if (showSnapshots == false && pathNodes.size() > 0)
  // {
  for (Node *node : pathNodes)
  {
    drawCell(renderer, node, pathNodesColor);
  }

  drawCell(renderer, pathNodes.front(), ending);
  drawCell(renderer, pathNodes.back(), starting);
  // }
}

void GUI::drawNormalSnapshots()
{
  // if (showSnapshots)
  // {
  if (pathNodesSnapshots.size() == 0 && snapshotNumber != 0)
  {
    snapshotNumber = 0;
  }
  if (snapshotNumber >= pathNodesSnapshots.size())
  {
    snapshotNumber = pathNodesSnapshots.size() - 1;
  }
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

    drawCell(renderer, pathNodesSnapshots.back()->front(), ending);
    drawCell(renderer, pathNodesSnapshots.back()->back(), starting);
  }
  // }
}

void GUI::drawVGPath()
{
  SDL_SetRenderDrawColor(renderer, pathNodesColor.r, pathNodesColor.g, pathNodesColor.b, pathNodesColor.a);
  for (int i = 0; i < pathNodes.size() - 1; i++)
  {
    int x1 = (pathNodes[i]->x * cellSize) + xPadding + (cellSize / 2);
    int y1 = (pathNodes[i]->y * cellSize) + yOffset + yPadding + (cellSize / 2);
    int x2 = (pathNodes[i + 1]->x * cellSize) + xPadding + (cellSize / 2);
    int y2 = (pathNodes[i + 1]->y * cellSize) + yOffset + yPadding + (cellSize / 2);

    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
  }

  drawCell(renderer, pathNodes.front(), ending);
  drawCell(renderer, pathNodes.back(), starting);
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

  SDL_SetRenderDrawColor(renderer, pathNodesColor.r, pathNodesColor.g, pathNodesColor.b, pathNodesColor.a);
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
    // int x = ((event.motion.x - xPadding) / cellSize) * cellSize;
    // int y = ((event.motion.y - yPadding - yOffset) / cellSize) * cellSize;

    int x = (double(event.motion.x - xPadding) / cellSize) * cellSize;
    int y = (double(event.motion.y - yPadding - yOffset) / cellSize) * cellSize;

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

        // if (node->x * cellSize == x && node->y * cellSize == y)
        // if (node->x * cellSize >= x && node->x * cellSize <= x + cellSize && node->y * cellSize >= y && node->y * cellSize <= y + cellSize)
        // if (node->x * cellSize <= x && node->x * cellSize > x + cellSize && node->y * cellSize < y && node->y * cellSize > y + cellSize)
        if (clickedInsideCell(node, event.motion.x, event.motion.y))
        {
          isNotDuplicate = false;
          // break;
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

          auto stop = std::chrono::high_resolution_clock::now();

          auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
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

bool GUI::clickedInsideCell(Node *node, int eventX, int eventY)
{
  double nodeStartX = (node->x * cellSize) + xPadding;
  double nodeStartY = (node->y * cellSize) + yOffset + yPadding;
  double nodeEndtX = ((node->x * cellSize) + xPadding) + cellSize;
  double nodeEndtY = ((node->y * cellSize) + yOffset + yPadding) + cellSize;

  if (eventX >= nodeStartX && eventX <= nodeEndtX && eventY >= nodeStartY && eventY <= nodeEndtY)
  {
    return true;
  }

  return false;
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
  switch (graph->type)
  {
  case NORMAL:
    drawNormalPath();
    break;
  case VG:
    drawVGPath();
    break;
  }
}

void GUI::drawSnapshots()
{
  switch (graph->type)
  {
  case NORMAL:
    drawNormalSnapshots();
    break;
  case VG:
    drawVGSnapshots();
    break;
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

  gridCursor.x = ((double(event.motion.x) / cellSize) * cellSize);
  gridCursor.y = ((double(event.motion.y) / cellSize) * cellSize);
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

  // hitNode = nullptr;
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
            snapshotNumber--;
          }
          break;
        case SDLK_d:
        case SDLK_RIGHT:
          if (snapshotNumber + 1 < pathNodesSnapshots.size())
          {
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

    if (autoMode)
    {
      snapshotNumber++;
    }

    drawBackground();
    drawGrid();
    drawForbiddenNodes();
    drawWaypoints();
    // drawSnapshots();
    drawPath();
    // drawVGPath();
    // drawVGSnapshots();
    drawCursor();

    // if (a != nullptr && b != nullptr)
    // {
    //   drawLOS(a, b);
    // }

    // if (hitNode != nullptr)
    // {
    //   if (hitNode->hit)
    //   {
    //     drawCell(renderer, hitNode, {0, 0, 0, 255});
    //   }
    //   // else
    //   // {
    //   //   drawCell(renderer, hitNode, forbiddenNodesColor);
    //   // }
    // }

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

// Propogate those fixes to other parts of the code