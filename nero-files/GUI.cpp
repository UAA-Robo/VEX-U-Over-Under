#pragma once
#include <iostream>
#include <chrono>
#include "GUI.h"

GUI::GUI(Graph *graph, int visual_node_size) : GRAPH(graph),
                                               VISUAL_NODE_SIZE(visual_node_size),
                                               X_NODES_COUNT(GRAPH->xNodes),
                                               Y_NODES_COUNT(GRAPH->yNodes),
                                               GRID_WIDTH((X_NODES_COUNT * VISUAL_NODE_SIZE) + 1),
                                               GRID_HEIGHT((Y_NODES_COUNT * VISUAL_NODE_SIZE) + 1),
                                               X_PADDING(100),
                                               Y_PADDING(100),
                                               SCROLL_SPEED(100),
                                               BACKGROUND_COLOR({233, 233, 233, 255}),
                                               LINE_COLOR({200, 200, 200, 255}),
                                               CURSOR_COLOR({200, 200, 200, 255}),
                                               ORIGIN_NODE_COLOR({255, 255, 0, 255}),
                                               DESTINATION_NODE_COLOR({0, 0, 0, 255}),
                                               FORBIDDEN_NODE_COLOR({255, 0, 0, 255}),
                                               PATH_NODE_COLOR({0, 255, 0, 255}),
                                               PAST_PATH_NODE_COLOR({0, 0, 255, 255})
{
  y_offset = 0;
  selected_nodes_count = 0;
  snapshot_number = 0;
  paths_found_count = 0;
  total_computation_duration = 0;

  mode = SIMPLE;
  is_selecting_nodes_allowed = false;
  is_auto_mode = false;
  quit = SDL_FALSE;
  mouse_active = SDL_FALSE;
  mouse_hover = SDL_FALSE;

  forbidden_nodes = GRAPH->getForbiddenNodes();
  waypoints = GRAPH->waypoints;

  cursor = {0, 0, VISUAL_NODE_SIZE, VISUAL_NODE_SIZE};

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Initialize SDL: %s", SDL_GetError());
    // return EXIT_FAILURE;
  }

  if (SDL_CreateWindowAndRenderer(GRID_WIDTH, GRID_HEIGHT, 0, &window, &renderer) < 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Create window and renderer: %s", SDL_GetError());
    // return EXIT_FAILURE;
  }

  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
  SDL_SetWindowTitle(window, "Pathfinding");

  run();
}

int GUI::run()
{
  switch (mode)
  {
  case SIMPLE:
  {
    get_random_path();
    break;
  }
  case SNAPSHOTS:
  {
    get_random_path_snapshots();
    break;
  }
  case LOS:
  {
    break;
  }
  }

  event_loop();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}

void GUI::event_loop()
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
          is_auto_mode = true;
          break;
        case SDLK_s:
        case SDLK_DOWN:
          is_auto_mode = false;
          break;
        case SDLK_a:
        case SDLK_LEFT:
          if (snapshot_number - 1 >= 0)
          {
            snapshot_number--;
          }
          break;
        case SDLK_d:
        case SDLK_RIGHT:
          if (snapshot_number + 1 < path_nodes_snapshots.size())
          {
            snapshot_number++;
          }
          break;
        case SDLK_ESCAPE:
          quit = SDL_TRUE;
          break;
        case SDLK_SPACE:
          generate_path();
          break;
        case SDLK_TAB:
          change_modes();
          break;
        }
      case SDLK_m:
      {
        break;
      }
      break;
      case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_RIGHT)
        {
          reset_grid();
        }
        else if (event.button.button == SDL_BUTTON_LEFT && is_selecting_nodes_allowed)
        {
          select_node(event);
        }
        break;
      case SDL_MOUSEMOTION:
        move_cursor(event);
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

    if (is_auto_mode && snapshot_number + 1 < path_nodes_snapshots.size())
    {
      snapshot_number++;
    }

    draw_background();
    draw_grid();
    draw_forbidden_nodes();
    draw_main();
    draw_cursor();

    // drawing mode -> GRAPH mode

    // draw normal path
    // draw normal snapshots
    // draw VG path
    // draw VG snapshots
    // draw LOS

    // if (a != nullptr && b != nullptr)
    // {
    //   draw_LOS(a, b);
    // }

    // if (hitNode != nullptr)
    // {
    //   if (hitNode->hit)
    //   {
    //     draw_cell(renderer, hitNode, {0, 0, 0, 255});
    //   }
    //   // else
    //   // {
    //   //   draw_cell(renderer, hitNode, FORBIDDEN_NODE_COLOR);
    //   // }
    // }

    SDL_RenderPresent(renderer);
  }
}

void GUI::clear_data()
{
  is_auto_mode = false;
  is_selecting_nodes_allowed = false;

  switch (mode)
  {
  case SIMPLE:
  {
    path_nodes.clear();
    break;
  }
  case SNAPSHOTS:
  {
    for (std::vector<Node *> *vec : path_nodes_snapshots)
    {
      delete vec;
    }
    path_nodes_snapshots.clear();
    snapshot_number = 0;
    break;
  }
  case LOS:
  {
    break;
  }
  }
}

void GUI::get_path(Node *a, Node *b)
{
  clear_data();
  auto start = std::chrono::high_resolution_clock::now();
  path_nodes = GRAPH->getPath(selected_nodes[0], selected_nodes[1]);
  auto stop = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  paths_found_count++;
  total_computation_duration += duration.count();
  std::cout << "Time for path " << paths_found_count << ": " << duration.count() << std::endl;
}

void GUI::get_random_path()
{
  clear_data();

  auto start = std::chrono::high_resolution_clock::now();
  path_nodes = GRAPH->getRandomPath();
  auto stop = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  paths_found_count++;
  total_computation_duration += duration.count();
  std::cout << "Time for path " << paths_found_count << ": " << duration.count() << std::endl;
}

void GUI::get_path_snapshots(Node *a, Node *b)
{
  clear_data();
  path_nodes_snapshots = GRAPH->getPathSnapshots(selected_nodes[0], selected_nodes[1]);
}

void GUI::get_random_path_snapshots()
{
  clear_data();
  path_nodes_snapshots = GRAPH->getRandomPathSnapshots();
}

void GUI::generate_path()
{
  switch (mode)
  {
  case SIMPLE:
  {
    get_random_path();
    break;
  }
  case SNAPSHOTS:
  {
    get_random_path_snapshots();
    break;
  }
  default:
  {
    throw std::runtime_error("2000");
  }
  }
}

void GUI::change_modes()
{
  switch (mode)
  {
  case SIMPLE:
  {
    clear_data();
    mode = SNAPSHOTS;
    break;
  }
  case SNAPSHOTS:
  {
    clear_data();
    // mode = SIMPLE;
    mode = LOS;
    break;
  }
  case LOS:
  {
    mode = SIMPLE;
    break;
  }
  }
}

void GUI::reset_grid()
{
  clear_data();
  is_selecting_nodes_allowed = true;
  selected_nodes_count = 0;
  selected_nodes.clear();
  // switch (mode)
  // {
  // case SIMPLE:
  // {
  //   clear_data();
  //   break;
  // }
  // case SNAPSHOTS:
  // {
  //   clear_data();
  //   break;
  // }
  // default:
  // {
  //   throw std::runtime_error("ERROR 11000");
  // }
  // }
}

// TODO - refactor out shared lines
// selected_nodes and selected_nodes_count? really?
void GUI::select_node(SDL_Event &event)
{
  is_auto_mode = false;
  bool nodeAllowed = true;
  int x = (double(event.motion.x - X_PADDING) / VISUAL_NODE_SIZE) * VISUAL_NODE_SIZE;
  int y = (double(event.motion.y - Y_PADDING - y_offset) / VISUAL_NODE_SIZE) * VISUAL_NODE_SIZE;

  for (Node *forbiddenNode : forbidden_nodes)
  {
    if (forbiddenNode->x * VISUAL_NODE_SIZE == x && forbiddenNode->y * VISUAL_NODE_SIZE == y)
    {
      nodeAllowed = false;
    }
  }
  if (nodeAllowed)
  {
    bool isNotDuplicate = true;
    for (Node *node : selected_nodes)
    {
      if (is_inside_node(node, event.motion.x, event.motion.y))
      {
        isNotDuplicate = false;
      }
    }
    if (isNotDuplicate)
    {
      selected_nodes_count++;
      selected_nodes.push_back(GRAPH->getNode(x / VISUAL_NODE_SIZE, y / VISUAL_NODE_SIZE));
    }
  }

  switch (mode)
  {
  case SIMPLE:
  {
    if (selected_nodes_count == 2)
    {
      is_selecting_nodes_allowed = false;
      auto start = std::chrono::high_resolution_clock::now();
      path_nodes = GRAPH->getPath(selected_nodes[0], selected_nodes[1]);
      auto stop = std::chrono::high_resolution_clock::now();

      auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
      paths_found_count++;
      total_computation_duration += duration.count();
      std::cout << "Time for path " << paths_found_count << ": " << duration.count() << std::endl;
    }
    break;
  }
  case SNAPSHOTS:
  {
    if (selected_nodes_count == 2)
    {
      is_selecting_nodes_allowed = false;
      snapshot_number = 0;
      path_nodes_snapshots = GRAPH->getPathSnapshots(selected_nodes[0], selected_nodes[1]);
    }
    break;
  }
  default:
  {
    throw std::runtime_error("ERROR 12000");
  }
  }
}

// TODO - refactor out shared lines
// void GUI::select_node(SDL_Event &event)
// {
//   switch (mode)
//   {
//   case SIMPLE:
//   {
//     is_auto_mode = false;
//     bool nodeAllowed = true;
//     // int x = ((event.motion.x - X_PADDING) / VISUAL_NODE_SIZE) * VISUAL_NODE_SIZE;
//     // int y = ((event.motion.y - Y_PADDING - y_offset) / VISUAL_NODE_SIZE) * VISUAL_NODE_SIZE;

//     int x = (double(event.motion.x - X_PADDING) / VISUAL_NODE_SIZE) * VISUAL_NODE_SIZE;
//     int y = (double(event.motion.y - Y_PADDING - y_offset) / VISUAL_NODE_SIZE) * VISUAL_NODE_SIZE;

//     for (Node *forbiddenNode : forbidden_nodes)
//     {
//       if (forbiddenNode->x * VISUAL_NODE_SIZE == x && forbiddenNode->y * VISUAL_NODE_SIZE == y)
//       {
//         nodeAllowed = false;
//       }
//     }
//     if (nodeAllowed)
//     {
//       bool isNotDuplicate = true;
//       for (Node *node : selected_nodes)
//       {

//         // if (node->x * VISUAL_NODE_SIZE == x && node->y * VISUAL_NODE_SIZE == y)
//         // if (node->x * VISUAL_NODE_SIZE >= x && node->x * VISUAL_NODE_SIZE <= x + VISUAL_NODE_SIZE && node->y * VISUAL_NODE_SIZE >= y && node->y * VISUAL_NODE_SIZE <= y + VISUAL_NODE_SIZE)
//         // if (node->x * VISUAL_NODE_SIZE <= x && node->x * VISUAL_NODE_SIZE > x + VISUAL_NODE_SIZE && node->y * VISUAL_NODE_SIZE < y && node->y * VISUAL_NODE_SIZE > y + VISUAL_NODE_SIZE)
//         if (is_inside_node(node, event.motion.x, event.motion.y))
//         {
//           isNotDuplicate = false;
//           // break;
//         }
//       }
//       if (isNotDuplicate)
//       {
//         selected_nodes_count++;

//         selected_nodes.push_back(GRAPH->getNode(x / VISUAL_NODE_SIZE, y / VISUAL_NODE_SIZE));
//         if (selected_nodes_count == 2)
//         {
//           is_selecting_nodes_allowed = false;
//           auto start = std::chrono::high_resolution_clock::now();
//           // path_nodes = GRAPH->getVGPath(selected_nodes[0], selected_nodes[1]);
//           path_nodes = GRAPH->get_path(selected_nodes[0], selected_nodes[1]);

//           auto stop = std::chrono::high_resolution_clock::now();

//           auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
//           paths_found_count++;
//           total_computation_duration += duration.count();
//         }
//         else if (selected_nodes_count > 2 || selected_nodes_count < 0)
//         {
//           throw "Z";
//         }
//       }
//     }
//     // break;
//     break;
//   }
//   case SNAPSHOTS:
//   {
//     is_auto_mode = false;
//     snapshot_number = 0;
//     bool nodeAllowed = true;
//     int x = ((event.motion.x - X_PADDING) / VISUAL_NODE_SIZE) * VISUAL_NODE_SIZE;
//     int y = ((event.motion.y - Y_PADDING - y_offset) / VISUAL_NODE_SIZE) * VISUAL_NODE_SIZE;

//     for (Node *forbiddenNode : forbidden_nodes)
//     {
//       if (forbiddenNode->x * VISUAL_NODE_SIZE == x && forbiddenNode->y * VISUAL_NODE_SIZE == y)
//       {
//         nodeAllowed = false;
//         break;
//       }
//     }
//     if (nodeAllowed)
//     {
//       bool isNotDuplicate = true;
//       for (Node *node : selected_nodes)
//       {
//         if (node->x * VISUAL_NODE_SIZE == x && node->y * VISUAL_NODE_SIZE == y)
//         {
//           isNotDuplicate = false;
//           break;
//         }
//       }
//       if (isNotDuplicate)
//       {
//         selected_nodes_count++;

//         // selected_nodes.push_back(GRAPH->getNode((x * VISUAL_NODE_SIZE) / VISUAL_NODE_SIZE,
//         //                                        (y * VISUAL_NODE_SIZE) / VISUAL_NODE_SIZE));
//         selected_nodes.push_back(GRAPH->getNode(x / VISUAL_NODE_SIZE, y / VISUAL_NODE_SIZE));
//         if (selected_nodes_count == 2)
//         {
//           is_selecting_nodes_allowed = false;

//           auto start = std::chrono::high_resolution_clock::now();
//           // path_nodes_snapshots = GRAPH->getVGPathSnapshots(selected_nodes[0], selected_nodes[1]);
//           path_nodes_snapshots = GRAPH->get_path_snapshots(selected_nodes[0], selected_nodes[1]);
//           auto stop = std::chrono::high_resolution_clock::now();

//           auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
//           // paths_found_count++;
//           // total_computation_duration += duration.count();
//         }
//         else if (selected_nodes_count > 2 || selected_nodes_count < 0)
//         {
//           throw "Z";
//         }
//       }
//     }
//     // break;
//     break;
//   }
//   default:
//   {
//     throw std::runtime_error("ERROR 12000");
//   }
//   }
// }

void GUI::move_cursor(SDL_Event &event)
{

  cursor.x = ((double(event.motion.x) / VISUAL_NODE_SIZE) * VISUAL_NODE_SIZE);
  cursor.y = ((double(event.motion.y) / VISUAL_NODE_SIZE) * VISUAL_NODE_SIZE);
  if ((event.motion.x >= X_PADDING && event.motion.x <= X_PADDING + GRID_WIDTH) &&
      (event.motion.y >= Y_PADDING + y_offset && event.motion.y <= Y_PADDING + y_offset + GRID_HEIGHT))
  {

    cursor.x = (((event.motion.x - X_PADDING) / VISUAL_NODE_SIZE) * VISUAL_NODE_SIZE) + X_PADDING;
    cursor.y = (((event.motion.y - Y_PADDING - y_offset) / VISUAL_NODE_SIZE) * VISUAL_NODE_SIZE) + Y_PADDING + y_offset;
  }
  else
  {
    cursor.x = -1000;
    cursor.y = -1000;
  }

  if (!mouse_active)
  {
    mouse_active = SDL_TRUE;
  }
}

void GUI::scroll(SDL_Event &event)
{

  if (event.wheel.y > 0)
  {
    y_offset = y_offset + (event.wheel.y * SCROLL_SPEED);
  }
  // scroll down
  else if (event.wheel.y < 0)
  {
    y_offset = y_offset + (event.wheel.y * SCROLL_SPEED);
  }
  else
  {
    // y_offset = 0;
  }
}

void GUI::draw_background()
{
  // Draw grid background.
  SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g,
                         BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
  SDL_RenderClear(renderer);
}

void GUI::draw_grid()
{

  // Draw grid lines.
  SDL_SetRenderDrawColor(renderer, LINE_COLOR.r, LINE_COLOR.g,
                         LINE_COLOR.b, LINE_COLOR.a);

  for (int x = 0; x < 1 + GRAPH->xNodes * VISUAL_NODE_SIZE;
       x += VISUAL_NODE_SIZE)
  {
    SDL_RenderDrawLine(renderer, x + X_PADDING, 0 + y_offset + Y_PADDING, x + X_PADDING, GRID_HEIGHT + y_offset + Y_PADDING);
  }

  for (int y = 0; y < 1 + GRAPH->yNodes * VISUAL_NODE_SIZE;
       y += VISUAL_NODE_SIZE)
  {
    SDL_RenderDrawLine(renderer, 0 + X_PADDING, y + y_offset + Y_PADDING, GRID_WIDTH + X_PADDING, y + y_offset + Y_PADDING);
  }
}

void GUI::draw_forbidden_nodes()
{

  for (Node *node : forbidden_nodes)
  {
    draw_cell(renderer, node, FORBIDDEN_NODE_COLOR);
  }
}

void GUI::draw_main()
{
  switch (mode)
  {
  case SIMPLE:
  {
    draw_path();
    break;
  }
  case SNAPSHOTS:
  {
    draw_snapshots();
    break;
  }
  case LOS:
  {
    draw_LOS();
    break;
  }
  }
}

void GUI::draw_path()
{
  switch (GRAPH->type)
  {
  case NORMAL:
    draw_normal_path();
    break;
  case VG:
    draw_vg_path();
    break;
  }
}

void GUI::draw_normal_path()
{
  switch (mode)
  {
  case SIMPLE:
  {
    draw_vector_nodes(path_nodes, PATH_NODE_COLOR, ORIGIN_NODE_COLOR, DESTINATION_NODE_COLOR);
    break;
  }
  default:
  {
    throw std::runtime_error("ERROR 8000");
  }
  }
}

void GUI::draw_vg_path()
{
  draw_lines_between_nodes(path_nodes, PATH_NODE_COLOR);
  draw_vector_nodes(path_nodes, PATH_NODE_COLOR, ORIGIN_NODE_COLOR, DESTINATION_NODE_COLOR);
}

void GUI::draw_snapshots()
{
  switch (GRAPH->type)
  {
  case NORMAL:
    draw_normal_snapshots();
    break;
  case VG:
    draw_vg_snapshots();
    break;
  }
}

void GUI::draw_normal_snapshots()
{
  // TODO - handle snapshot_number and its errors, after generating snapshot path should not have to move forward to show initial state
  for (int s = 0; s <= snapshot_number; s++)
  {
    if (s == snapshot_number)
    {
      draw_vector_nodes((*path_nodes_snapshots[s]), PATH_NODE_COLOR, ORIGIN_NODE_COLOR, DESTINATION_NODE_COLOR);
    }
    else
    {
      draw_vector_nodes((*path_nodes_snapshots[s]), PAST_PATH_NODE_COLOR, ORIGIN_NODE_COLOR, DESTINATION_NODE_COLOR);
    }
  }
}

void GUI::draw_vg_snapshots()
{
  if (path_nodes_snapshots.size() == 0)
  {
    return;
  }

  for (int s = 0; s <= snapshot_number; s++)
  {
    if (s == snapshot_number)
    {
      draw_lines_between_nodes((*path_nodes_snapshots[s]), PATH_NODE_COLOR);
      draw_vector_nodes((*path_nodes_snapshots[s]), PATH_NODE_COLOR, PATH_NODE_COLOR, PATH_NODE_COLOR);
    }
    else
    {
      draw_lines_between_nodes((*path_nodes_snapshots[s]), PAST_PATH_NODE_COLOR);
      draw_vector_nodes((*path_nodes_snapshots[s]), PAST_PATH_NODE_COLOR, PAST_PATH_NODE_COLOR, PAST_PATH_NODE_COLOR);
    }
  }

  // for (int s = 0; s <= snapshot_number; s++)
  // {
  //   if (s == snapshot_number)
  //   {
  //     SDL_SetRenderDrawColor(renderer, PATH_NODE_COLOR.r, PATH_NODE_COLOR.g, PATH_NODE_COLOR.b, PATH_NODE_COLOR.a);
  //   }
  //   else
  //   {
  //     SDL_SetRenderDrawColor(renderer, PAST_PATH_NODE_COLOR.r, PAST_PATH_NODE_COLOR.g, PAST_PATH_NODE_COLOR.b, PAST_PATH_NODE_COLOR.a);
  //   }

  //   for (int n = 0; n < path_nodes_snapshots.at(s)->size() - 1; n++)
  //   {
  //     int x1 = (path_nodes_snapshots.at(s)->at(n)->x * VISUAL_NODE_SIZE) + X_PADDING + (VISUAL_NODE_SIZE / 2);
  //     int y1 = (path_nodes_snapshots.at(s)->at(n)->y * VISUAL_NODE_SIZE) + y_offset + Y_PADDING + (VISUAL_NODE_SIZE / 2);
  //     int x2 = (path_nodes_snapshots.at(s)->at(n + 1)->x * VISUAL_NODE_SIZE) + X_PADDING + (VISUAL_NODE_SIZE / 2);
  //     int y2 = (path_nodes_snapshots.at(s)->at(n + 1)->y * VISUAL_NODE_SIZE) + y_offset + Y_PADDING + (VISUAL_NODE_SIZE / 2);

  //     SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
  //   }
  // }

  // // SDL_Color ORIGIN_NODE_COLOR = {255, 255, 0, 255};
  // // SDL_Color DESTINATION_NODE_COLOR = {0, 255, 255, 255};
  // draw_cell(renderer, path_nodes_snapshots.back()->front(), DESTINATION_NODE_COLOR);
  // draw_cell(renderer, path_nodes_snapshots.back()->back(), ORIGIN_NODE_COLOR);

  // // SDL_SetRenderDrawColor(renderer, PATH_NODE_COLOR.r, PATH_NODE_COLOR.g, PATH_NODE_COLOR.b, PATH_NODE_COLOR.a);
  // // for (int i = 0; i < path_nodes.size() - 1; i++)
  // // {
  // //   int x1 = (path_nodes[i]->x * VISUAL_NODE_SIZE) + X_PADDING + (VISUAL_NODE_SIZE / 2);
  // //   int y1 = (path_nodes[i]->y * VISUAL_NODE_SIZE) + y_offset + Y_PADDING + (VISUAL_NODE_SIZE / 2);
  // //   int x2 = (path_nodes[i + 1]->x * VISUAL_NODE_SIZE) + X_PADDING + (VISUAL_NODE_SIZE / 2);
  // //   int y2 = (path_nodes[i + 1]->y * VISUAL_NODE_SIZE) + y_offset + Y_PADDING + (VISUAL_NODE_SIZE / 2);

  // //   SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
  // // }

  draw_cell(renderer, path_nodes_snapshots.back()->front(), DESTINATION_NODE_COLOR);
  draw_cell(renderer, path_nodes_snapshots.back()->back(), ORIGIN_NODE_COLOR);
}

void GUI::draw_LOS()
{
}

void GUI::draw_cursor()
{
  // Draw grid ghost cursor.
  if (mouse_active && mouse_hover)
  {
    SDL_SetRenderDrawColor(renderer, CURSOR_COLOR.r,
                           CURSOR_COLOR.g,
                           CURSOR_COLOR.b,
                           CURSOR_COLOR.a);
    SDL_RenderFillRect(renderer, &cursor);
  }
}

void GUI::draw_vector_nodes(std::vector<Node *> nodes, SDL_Color pathColor, SDL_Color ORIGIN_NODE_COLOR, SDL_Color DESTINATION_NODE_COLOR)
{
  for (int i = 0; i < nodes.size(); i++)
  {
    if (i == 0)
    {
      draw_cell(renderer, nodes[i], ORIGIN_NODE_COLOR);
    }
    else if (i == nodes.size() - 1)
    {
      draw_cell(renderer, nodes[i], DESTINATION_NODE_COLOR);
    }
    else
    {
      draw_cell(renderer, nodes[i], pathColor);
    }
  }
}

void GUI::draw_line(SDL_Renderer *renderer, Node *a, Node *b, SDL_Color color)
{
  int x1 = (a->x * VISUAL_NODE_SIZE) + X_PADDING + (VISUAL_NODE_SIZE / 2);
  int y1 = (a->y * VISUAL_NODE_SIZE) + y_offset + Y_PADDING + (VISUAL_NODE_SIZE / 2);
  int x2 = (b->x * VISUAL_NODE_SIZE) + X_PADDING + (VISUAL_NODE_SIZE / 2);
  int y2 = (b->y * VISUAL_NODE_SIZE) + y_offset + Y_PADDING + (VISUAL_NODE_SIZE / 2);

  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void GUI::draw_line(SDL_Renderer *renderer, Node *a, Node *b)
{

  int x1 = (a->x * VISUAL_NODE_SIZE) + X_PADDING + (VISUAL_NODE_SIZE / 2);
  int y1 = (a->y * VISUAL_NODE_SIZE) + y_offset + Y_PADDING + (VISUAL_NODE_SIZE / 2);
  int x2 = (b->x * VISUAL_NODE_SIZE) + X_PADDING + (VISUAL_NODE_SIZE / 2);
  int y2 = (b->y * VISUAL_NODE_SIZE) + y_offset + Y_PADDING + (VISUAL_NODE_SIZE / 2);

  SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void GUI::draw_lines_between_nodes(std::vector<Node *> nodes, SDL_Color pathColor)
{
  for (int i = 1; i < nodes.size(); i++)
  {
    draw_line(renderer, nodes[i - 1], nodes[i], pathColor);
  }
}

bool GUI::is_inside_node(Node *node, int eventX, int eventY)
{
  double nodeStartX = (node->x * VISUAL_NODE_SIZE) + X_PADDING;
  double nodeStartY = (node->y * VISUAL_NODE_SIZE) + y_offset + Y_PADDING;
  double nodeEndtX = ((node->x * VISUAL_NODE_SIZE) + X_PADDING) + VISUAL_NODE_SIZE;
  double nodeEndtY = ((node->y * VISUAL_NODE_SIZE) + y_offset + Y_PADDING) + VISUAL_NODE_SIZE;

  if (eventX >= nodeStartX && eventX <= nodeEndtX && eventY >= nodeStartY && eventY <= nodeEndtY)
  {
    return true;
  }

  return false;
}

void GUI::draw_waypoints()
{
  SDL_Color waypointsColor = {0, 0, 0, 255};
  // SDL_SetRenderDrawColor(waypointsColor.r, waypointsColor.g, waypointsColor.b, waypointsColor.a);
  for (Node *waypoint : waypoints)
  {
    draw_cell(renderer, waypoint, waypointsColor);
  }
}

//=============================================================================
//=============================================================================
// PRIVATE METHODS
//=============================================================================
//=============================================================================

void GUI::draw_cell(SDL_Renderer *renderer, Node *node, SDL_Color color)
{
  SDL_Rect cell{
      .x = (node->x * VISUAL_NODE_SIZE) + X_PADDING,
      .y = (node->y * VISUAL_NODE_SIZE) + y_offset + Y_PADDING,
      .w = VISUAL_NODE_SIZE,
      .h = VISUAL_NODE_SIZE,
  };

  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect(renderer, &cell);
}

void GUI::draw_cell(SDL_Renderer *renderer, Node *node)
{
  SDL_Rect cell{
      .x = (node->x * VISUAL_NODE_SIZE) + X_PADDING,
      .y = (node->y * VISUAL_NODE_SIZE) + y_offset + Y_PADDING,
      .w = VISUAL_NODE_SIZE,
      .h = VISUAL_NODE_SIZE,
  };

  SDL_RenderFillRect(renderer, &cell);
}

// void GUI::draw_cell_second(SDL_Renderer *renderer, Node *node, SDL_Color color, int xDifference)
// {
//   SDL_Rect cell{
//       .x = (node->x * VISUAL_NODE_SIZE) + (X_PADDING * 2),
//       .y = (node->y * VISUAL_NODE_SIZE) + y_offset + Y_PADDING,
//       .w = VISUAL_NODE_SIZE,
//       .h = VISUAL_NODE_SIZE,
//   };

//   SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
//   SDL_RenderFillRect(renderer, &cell);
// }

// Propogate those fixes to other parts of the code