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
  graph = graph;
  graph2 = graph2;
  xNodes = graph->xNodes;
  yNodes = graph->yNodes;
  cellSize = cellSize;
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
  showSnapshots = true;
  selectingNodesAllowed = false;
  autoMode = false;

  forbiddenNodes = graph->getForbiddenNodes();

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
  ending = {0, 255, 255, 255};

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
}

void GUI::getPath(Node *a, Node *b)
{
  pathNodes.clear();

  autoMode = false;
  selectingNodesAllowed = false;

  auto start = std::chrono::high_resolution_clock::now();
  pathNodes = graph->getPath(selectedNodes[0], selectedNodes[1]);
  auto stop = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cout << duration.count() << std::endl;
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

  // auto start = std::chrono::high_resolution_clock::now();
  pathNodesSnapshots = graph->getPathSnapshots(selectedNodes[0], selectedNodes[1]);
  // auto stop = std::chrono::high_resolution_clock::now();

  // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  // std::cout << duration.count() << std::endl;
  // pathsFound++;
  // durationTotal += duration.count();
}

void GUI::getRandomPath()
{
  pathNodes.clear();

  autoMode = false;
  selectingNodesAllowed = false;

  auto start = std::chrono::high_resolution_clock::now();
  pathNodes = graph->getRandomPath();
  auto stop = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cout << duration.count() << std::endl;
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

  // auto start = std::chrono::high_resolution_clock::now();
  pathNodesSnapshots = graph->getRandomPathSnapshots();
  // auto stop = std::chrono::high_resolution_clock::now();

  // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  // std::cout << duration.count() << std::endl;
  // pathsFound++;
  // durationTotal += duration.count();
  std::cout << "QQQQQQQQQQ\n";
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
          // RANDOMIZE PATH SNAPSHOTS OR PATH
          if (showSnapshots)
          {
            getRandomPathSnapshots();
            // for (std::vector<Node *> *vec : pathNodesSnapshots)
            // {
            //   vec->clear();
            //   delete vec;
            // }
            // pathNodesSnapshots.clear();

            // snapshotNumber = 0;
            // autoMode = false;

            // auto start = std::chrono::high_resolution_clock::now();
            // pathNodesSnapshots = graph->getRandomPathSnapshots();
            // auto stop = std::chrono::high_resolution_clock::now();

            // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            // std::cout << duration.count() << std::endl;
            // pathsFound++;
            // durationTotal += duration.count();
          }
          else
          {
            getRandomPath();
            // pathNodes.clear();

            // auto start = std::chrono::high_resolution_clock::now();
            // pathNodes = graph->getRandomPath();
            // auto stop = std::chrono::high_resolution_clock::now();

            // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            // std::cout << duration.count() << std::endl;
            // pathsFound++;
            // durationTotal += duration.count();
            // autoMode = false;
          }
          break;
        case SDLK_TAB:
          switchModes();
          break;
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_RIGHT)
        {
          // RESET PATH SNAPSHOTS AND PATH
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
        }
        else if (event.button.button == SDL_BUTTON_LEFT && selectingNodesAllowed)
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
                  pathNodesSnapshots = graph->getPathSnapshots(selectedNodes[0], selectedNodes[1]);
                  auto stop = std::chrono::high_resolution_clock::now();

                  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
                  std::cout << duration.count() << std::endl;
                  pathsFound++;
                  durationTotal += duration.count();
                }
                else if (nodesSelected > 2 || nodesSelected < 0)
                {
                  throw "Z";
                }
              }
            }
            break;
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
                  pathNodes = graph->getPath(selectedNodes[0], selectedNodes[1]);
                  auto stop = std::chrono::high_resolution_clock::now();

                  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
                  std::cout << duration.count() << std::endl;
                  pathsFound++;
                  durationTotal += duration.count();
                }
                else if (nodesSelected > 2 || nodesSelected < 0)
                {
                  throw "Z";
                }
              }
            }
            break;
          }
        }
        break;
      case SDL_MOUSEMOTION:
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
        break;
      case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_ENTER && !mouse_hover)
          mouse_hover = SDL_TRUE;
        else if (event.window.event == SDL_WINDOWEVENT_LEAVE && mouse_hover)
          mouse_hover = SDL_FALSE;
        break;
      case SDL_MOUSEWHEEL:
        // scroll up
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

    // Draw grid background.
    SDL_SetRenderDrawColor(renderer, gridBackgroundColor.r, gridBackgroundColor.g,
                           gridBackgroundColor.b, gridBackgroundColor.a);
    SDL_RenderClear(renderer);

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

    // Draw grid ghost cursor.
    if (mouse_active && mouse_hover)
    {
      SDL_SetRenderDrawColor(renderer, gridCursorColor.r,
                             gridCursorColor.g,
                             gridCursorColor.b,
                             gridCursorColor.a);
      SDL_RenderFillRect(renderer, &gridCursor);
    }

    for (Node *node : forbiddenNodes)
    {
      drawCell(renderer, node, forbiddenNodesColor);
    }

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
    else if (pathNodes.size() > 0)
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

    SDL_RenderPresent(renderer);
  }
}

int GUI::run()
{
  // if (SDL_Init(SDL_INIT_VIDEO) < 0)
  // {
  //   SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Initialize SDL: %s", SDL_GetError());
  //   return EXIT_FAILURE;
  // }

  // if (SDL_CreateWindowAndRenderer(gridWidth, gridHeight, 0, &window, &renderer) < 0)
  // {
  //   SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Create window and renderer: %s", SDL_GetError());
  //   return EXIT_FAILURE;
  // }

  // enum Mode
  // {
  //   Simple,
  //   Snapshots
  // };
  // Mode mode = Snapshots;

  // std::vector<Node *> pathNodes;
  // std::vector<std::vector<Node *> *> pathNodesSnapshots;

  if (showSnapshots)
  {
    std::cout << "QQQQQQQQQQ\n";
    getRandomPathSnapshots();
    std::cout << "AAAAAAA\n";
  }

  if (!showSnapshots)
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
