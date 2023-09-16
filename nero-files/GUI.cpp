#pragma once
#include <vector>
#include <SDL2/SDL.h>
// #include <SDL.h> // Maybe for other OSes?
#include <chrono>
#include "GUI.h"
#include "Node.h"
#include "Graph.h"
#include <iostream>

//=============================================================================
//=============================================================================
// PUBLIC METHODS
//=============================================================================
//=============================================================================

GUI::GUI(Graph *graph, int CELL_SIZE)
{
  this->graph = graph;
  this->CELL_SIZE = CELL_SIZE;
  this->forbiddenNodes = graph->getForbiddenNodes();
};

int GUI::run(bool showSnapshots)
{
  int windowWidth = (graph->xNodes * CELL_SIZE) + 1;
  int windowHeight = (graph->yNodes * CELL_SIZE) + 1;

  SDL_Rect grid_cursor_ghost = {0, 0, CELL_SIZE,
                                CELL_SIZE};

  // Dark theme.
  // SDL_Color grid_background = {22, 22, 22, 255};
  // SDL_Color grid_line_color = {44, 44, 44, 255};
  // SDL_Color grid_cursor_ghost_color = {44, 44, 44, 255};
  // SDL_Color grid_cursor_color = {255, 255, 255, 255};

  // Light Theme.
  SDL_Color grid_background = {233, 233, 233, 255};
  SDL_Color grid_line_color = {200, 200, 200, 255};
  SDL_Color grid_cursor_ghost_color = {200, 200, 200, 255};
  SDL_Color grid_cursor_color = {160, 160, 160, 255};

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Initialize SDL: %s",
                 SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_Window *window;
  SDL_Renderer *renderer;

  if (SDL_CreateWindowAndRenderer(windowWidth, windowHeight, 0, &window,
                                  &renderer) < 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Create window and renderer: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
  SDL_SetWindowTitle(window, "Pathfinding");

  SDL_bool quit = SDL_FALSE;
  SDL_bool mouse_active = SDL_FALSE;
  SDL_bool mouse_hover = SDL_FALSE;
  enum Mode
  {
    Simple,
    Snapshots
  };
  Mode mode = Snapshots;

  int pathsFound = 0;
  int durationTotal = 0;

  // INIT PATH SNAPSHOTS
  std::vector<std::vector<Node *> *> pathNodesSnapshots;
  if (showSnapshots)
  {
    auto start = std::chrono::high_resolution_clock::now();
    // std::vector<std::vector<Node *> *> pathNodesSnapshots = graph->getRandomPathSnapshots();
    pathNodesSnapshots = graph->getRandomPathSnapshots();
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() << std::endl;
    pathsFound++;
    durationTotal += duration.count();
  }

  // INIT PATH
  std::vector<Node *> pathNodes;
  if (!showSnapshots)
  {
    auto start = std::chrono::high_resolution_clock::now();
    pathNodes = graph->getRandomPath();
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() << std::endl;
    pathsFound++;
    durationTotal += duration.count();
  }

  std::vector<Node *> selectedNodes;
  bool selectingNodesAllowed = false;
  SDL_Color forbiddenNodesColor = {255, 0, 0, 255};

  yOffset = 0;
  xPadding = 100;
  yPadding = 100;
  int scrollSensitivity = 100;

  int nodesSelected = 0;

  int snapshotNumber = 0;
  bool autoMode = false;

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
          snapshotNumber--;
          break;
        case SDLK_d:
        case SDLK_RIGHT:
          snapshotNumber++;
          break;
        case SDLK_ESCAPE:
          quit = SDL_TRUE;
          break;
        case SDLK_SPACE:
          // RANDOMIZE PATH SNAPSHOTS OR PATH
          if (showSnapshots)
          {
            for (std::vector<Node *> *vec : pathNodesSnapshots)
            {
              vec->clear();
              delete vec;
            }
            pathNodesSnapshots.clear();

            snapshotNumber = 0;
            autoMode = false;

            auto start = std::chrono::high_resolution_clock::now();
            pathNodesSnapshots = graph->getRandomPathSnapshots();
            auto stop = std::chrono::high_resolution_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            std::cout << duration.count() << std::endl;
            pathsFound++;
            durationTotal += duration.count();
          }
          else
          {
            pathNodes.clear();

            auto start = std::chrono::high_resolution_clock::now();
            pathNodes = graph->getRandomPath();
            auto stop = std::chrono::high_resolution_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            std::cout << duration.count() << std::endl;
            pathsFound++;
            durationTotal += duration.count();
            autoMode = false;
          }
          break;
        case SDLK_TAB:
          showSnapshots = !showSnapshots;
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
            int x = ((event.motion.x - xPadding) / CELL_SIZE) * CELL_SIZE;
            int y = ((event.motion.y - yPadding - yOffset) / CELL_SIZE) * CELL_SIZE;

            for (Node *forbiddenNode : forbiddenNodes)
            {
              if (forbiddenNode->x * CELL_SIZE == x && forbiddenNode->y * CELL_SIZE == y)
              {
                nodeAllowed = false;
              }
            }
            if (nodeAllowed)
            {
              bool isNotDuplicate = true;
              for (Node *node : selectedNodes)
              {
                if (node->x * CELL_SIZE == x && node->y * CELL_SIZE == y)
                {
                  isNotDuplicate = false;
                }
              }
              if (isNotDuplicate)
              {
                nodesSelected++;

                // selectedNodes.push_back(graph->getNode((x * CELL_SIZE) / CELL_SIZE,
                //                                        (y * CELL_SIZE) / CELL_SIZE));
                selectedNodes.push_back(graph->getNode(x / CELL_SIZE, y / CELL_SIZE));
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
            int x = ((event.motion.x - xPadding) / CELL_SIZE) * CELL_SIZE;
            int y = ((event.motion.y - yPadding - yOffset) / CELL_SIZE) * CELL_SIZE;

            for (Node *forbiddenNode : forbiddenNodes)
            {
              if (forbiddenNode->x * CELL_SIZE == x && forbiddenNode->y * CELL_SIZE == y)
              {
                nodeAllowed = false;
              }
            }
            if (nodeAllowed)
            {
              bool isNotDuplicate = true;
              for (Node *node : selectedNodes)
              {
                if (node->x * CELL_SIZE == x && node->y * CELL_SIZE == y)
                {
                  isNotDuplicate = false;
                }
              }
              if (isNotDuplicate)
              {
                nodesSelected++;

                selectedNodes.push_back(graph->getNode(x / CELL_SIZE, y / CELL_SIZE));
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
    SDL_SetRenderDrawColor(renderer, grid_background.r, grid_background.g,
                           grid_background.b, grid_background.a);
    SDL_RenderClear(renderer);

    // Draw grid lines.
    SDL_SetRenderDrawColor(renderer, grid_line_color.r, grid_line_color.g,
                           grid_line_color.b, grid_line_color.a);

    for (int x = 0; x < 1 + graph->xNodes * CELL_SIZE;
         x += CELL_SIZE)
    {
      SDL_RenderDrawLine(renderer, x + xPadding, 0 + yOffset + yPadding, x + xPadding, windowHeight + yOffset + yPadding);
    }

    for (int y = 0; y < 1 + graph->yNodes * CELL_SIZE;
         y += CELL_SIZE)
    {
      SDL_RenderDrawLine(renderer, 0 + xPadding, y + yOffset + yPadding, windowWidth + xPadding, y + yOffset + yPadding);
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

    for (Node *node : forbiddenNodes)
    {
      drawCell(renderer, node, forbiddenNodesColor);
    }

    if (showSnapshots)
    {
      SDL_Color pathNodesColor = {0, 255, 0, 255};
      SDL_Color pathNodesPastColor = {0, 0, 255, 255};
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

        SDL_Color starting = {255, 255, 0, 255};
        SDL_Color ending = {0, 255, 255, 255};
        drawCell(renderer, pathNodesSnapshots.back()->front(), ending);
        drawCell(renderer, pathNodesSnapshots.back()->back(), starting);
      }
    }
    else if (pathNodes.size() > 0)
    {
      SDL_Color pathNodesColor = {0, 255, 0, 255};
      // TODO - start, left, shows full path: feature or bug?
      // only of you can go past end snapshot into first snapshot
      for (Node *node : pathNodes)
      {
        drawCell(renderer, node, pathNodesColor);
      }

      // TODO - redeclaring every loop, move outside loop
      SDL_Color starting = {255, 255, 0, 255};
      SDL_Color ending = {0, 255, 255, 255};
      drawCell(renderer, pathNodes.front(), ending);
      drawCell(renderer, pathNodes.back(), starting);
    }

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  std::cout << (durationTotal / pathsFound) << std::endl;

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
      .x = (node->x * CELL_SIZE) + xPadding,
      .y = (node->y * CELL_SIZE) + yOffset + yPadding,
      .w = CELL_SIZE,
      .h = CELL_SIZE,
  };

  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect(renderer, &cell);
}
