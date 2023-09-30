#include <iostream>
#include <time.h>
#include <random>
#include <stdexcept>
#include <cmath>
#include "Node.h"
#include "Graph.h"
#include "Constants.h"
#include "Enums.h"

Position Graph::get_node_position(int x, int y)
{
  if (y == 0 && x == 0)
  {
    return Position::TOP_LEFT;
  }
  else if (y == 0 && x == X_NODES_COUNT - 1)
  {
    return Position::TOP_RIGHT;
  }
  else if (y == Y_NODES_COUNT - 1 && x == X_NODES_COUNT - 1)
  {
    return Position::BOTTOM_RIGHT;
  }
  else if (y == Y_NODES_COUNT - 1 && x == 0)
  {
    return Position::BOTTOM_LEFT;
  }
  else if (y == 0)
  {
    return Position::TOP;
  }
  else if (x == X_NODES_COUNT - 1)
  {
    return Position::RIGHT;
  }
  else if (y == Y_NODES_COUNT - 1)
  {
    return Position::BOTTOM;
  }
  else if (x == 0)
  {
    return Position::LEFT;
  }
  else
  {
    return Position::CENTER;
  }
}

Graph::Graph(int X_NODES_COUNT, int Y_NODES_COUNT, double NODE_SIZE) : X_NODES_COUNT(X_NODES_COUNT),
                                                                       Y_NODES_COUNT(Y_NODES_COUNT),
                                                                       NODE_SIZE(NODE_SIZE),
                                                                       waypoints(new std::set<Node *>),
                                                                       forbidden_nodes(new std::vector<Node *>),
                                                                       nodes(new Node **[Y_NODES_COUNT])
{
  for (int x = 0; x < Y_NODES_COUNT; x++)
  {
    nodes[x] = new Node *[X_NODES_COUNT];
  }

  for (int y = 0; y < Y_NODES_COUNT; y++)
  {
    for (int x = 0; x < X_NODES_COUNT; x++)
    {
      nodes[y][x] = new Node(x, y, this);

      switch (get_node_position(x, y))
      {
      case Position::TOP_LEFT:
      {
        break;
      }
      case Position::TOP_RIGHT:
      {
        add_edge(nodes[y][x], nodes[y][x - 1]);
        break;
      }
      case Position::BOTTOM_RIGHT:
      {
        add_edge(nodes[y][x], nodes[y - 1][x]);
        add_edge(nodes[y][x], nodes[y - 1][x - 1]);
        add_edge(nodes[y][x], nodes[y][x - 1]);
        break;
      }
      case Position::BOTTOM_LEFT:
      {
        add_edge(nodes[y][x], nodes[y - 1][x + 1]);
        add_edge(nodes[y][x], nodes[y - 1][x]);
        break;
      }
      case Position::TOP:
      {
        add_edge(nodes[y][x], nodes[y][x - 1]);
        break;
      }
      case Position::RIGHT:
      {
        add_edge(nodes[y][x], nodes[y - 1][x]);
        add_edge(nodes[y][x], nodes[y - 1][x - 1]);
        add_edge(nodes[y][x], nodes[y][x - 1]);
        break;
      }
      case Position::BOTTOM:
      {
        add_edge(nodes[y][x], nodes[y - 1][x + 1]);
        add_edge(nodes[y][x], nodes[y - 1][x]);
        add_edge(nodes[y][x], nodes[y - 1][x - 1]);
        add_edge(nodes[y][x], nodes[y][x - 1]);
        break;
      }
      case Position::LEFT:
      {
        add_edge(nodes[y][x], nodes[y - 1][x + 1]);
        add_edge(nodes[y][x], nodes[y - 1][x]);
        break;
      }
      case Position::CENTER:
      {
        add_edge(nodes[y][x], nodes[y - 1][x + 1]);
        add_edge(nodes[y][x], nodes[y - 1][x]);
        add_edge(nodes[y][x], nodes[y - 1][x - 1]);
        add_edge(nodes[y][x], nodes[y][x - 1]);
        break;
      }
      default:
      {
        std::cout << "\n";
        throw std::runtime_error("ERROR 10237123");
        break;
      }
      }
    };
  };

  add_forbidden_zones(this);
  std::cout << "BBBBBBBBBB\n";
  find_waypoints();
  std::cout << "BBBBBBBBBB\n";
  add_neighbor_waypoints();
  std::cout << "BBBBBBBBBB\n";
};

Graph::~Graph()
{

  for (int y = 0; y < Y_NODES_COUNT; y++)
  {
    for (int x = 0; x < X_NODES_COUNT; x++)
    {
      delete nodes[y][x];
    }
  }

  for (int x = 0; x < Y_NODES_COUNT; x++)
  {
    delete[] nodes[x];
  }

  delete[] nodes;
}

Node *Graph::get_node(int x, int y)
{
  if (x >= this->X_NODES_COUNT || y >= this->Y_NODES_COUNT || x < 0 || y < 0)
  {
    throw std::runtime_error("ERROR 005");
  }
  return nodes[y][x];
};

std::vector<Node *> *Graph::get_forbidden_nodes()
{
  return forbidden_nodes;
}

void Graph::add_forbidden_node(Node *node)
{
  forbidden_nodes->push_back(node);
}

ForbiddenType Graph::get_forbidden_type(int loc, int start, int end, int robot_zones_count, int buffer_zones_count)
{
  if (start <= loc && loc <= end)
  {
    return ForbiddenType::CORE;
  }
  else if (end < loc && loc <= end + robot_zones_count)
  {
    return ForbiddenType::ROBOT;
  }
  else if (end + robot_zones_count < loc && loc <= end + robot_zones_count + buffer_zones_count)
  {
    return ForbiddenType::BUFFER;
  }

  throw std::runtime_error("ERROR 129312093123");
  return ForbiddenType::NONE;
}

ForbiddenType Graph::get_forbidden_type(int x, int y, int start, int end, int robot_zones_count, int buffer_zones_count)
{
}

bool Graph::is_valid_node(int x, int y)
{
  if ((x >= 0 && x < X_NODES_COUNT) && (y >= 0 && y < Y_NODES_COUNT))
  {
    return true;
  }

  return false;
}

Position Graph::get_triangle_position(Node *right_angle_node, Node *a, Node *b)
{
  Position position_a = get_relative_position(right_angle_node, a);
  Position position_b = get_relative_position(right_angle_node, b);

  switch (position_a)
  {
  case Position::TOP:
  {
    switch (position_b)
    {
    case Position::RIGHT:
    {
      return Position::BOTTOM_LEFT;
    }
    case Position::LEFT:
    {
      return Position::BOTTOM_RIGHT;
    }
    default:
    {
      throw std::runtime_error("ERROR 23819723");
    }
    }
    break;
  }
  case Position::RIGHT:
  {
    switch (position_b)
    {
    case Position::TOP:
    {
      return Position::BOTTOM_LEFT;
    }
    case Position::BOTTOM:
    {
      return Position::TOP_LEFT;
    }
    default:
    {
      throw std::runtime_error("ERROR 23819723");
    }
    }
  }
  case Position::BOTTOM:
  {
    switch (position_b)
    {
    case Position::RIGHT:
    {
      return Position::TOP_LEFT;
    }
    case Position::LEFT:
    {
      return Position::TOP_RIGHT;
    }
    default:
    {
      throw std::runtime_error("ERROR 23819723");
    }
    }
  }
  case Position::LEFT:
  {
    switch (position_b)
    {
    case Position::TOP:
    {
      return Position::BOTTOM_RIGHT;
    }
    case Position::BOTTOM:
    {
      return Position::TOP_RIGHT;
    }
    default:
    {
      throw std::runtime_error("ERROR 23819723");
    }
    }
  }
  default:
  {
    throw std::runtime_error("ERROR 59172983719028");
  }
  }

  // if ((position_a == Position::TOP || position_a == Position::RIGHT || position_a == Position::BOTTOM || position_a == Position::LEFT) && (position_b == Position::TOP || position_b == Position::RIGHT || position_b == Position::BOTTOM || position_b == Position::LEFT))
  // {
  // }

  // throw std::runtime_error("ERROR 000001");

  throw std::runtime_error("ERROR 129381298318198237123");
}

// given right angle node, which triangle orientation is it
// given 3 nodes and triangle orientation, which is a (same x as right angle node) and which is b
// get slope, and if isoceles, readjust slope to 1/1

// different side lengths, if equal can go by one/one
void Graph::forbid_triangle(Node *right_angle_node, Node *a, Node *b, int robot_nodes_count, int buffer_nodes_count)
{
  if (right_angle_node->x == b->x)
  {
    Node *temp = a;
    a = b;
    b = temp;
  }

  int x_start = a->x;
  int y_start = a->y;
  int x_end = b->x;
  int y_end = b->y;
  double x = x_start;
  double y = y_start;

  double dx = x_end - x_start;
  double dy = y_end - y_start;
  double x_slope;
  double y_slope;

  if (dx < 0)
  {
    dx = dx * (-1);
  }

  if (dy < 0)
  {
    dy = dy * (-1);
  }

  if (dx == dy)
  {
    x_slope = 1;
    y_slope = 1;
  }
  else
  {
    throw std::runtime_error("ERROR 1892313");
    x_slope = (dx / dy) / 140;
    y_slope = (dy / dx) / 140;
  }

  if (x_slope < 0)
  {
    x_slope = x_slope * (-1);
  }

  if (y_slope < 0)
  {
    y_slope = y_slope * (-1);
  }

  Position position = get_triangle_position(right_angle_node, a, b);

  switch (position)
  {
  case Position::TOP_LEFT:
  {
    y_slope = y_slope * (-1);

    while (x <= x_end && y >= y_end)
    {
      for (int sub_x = x_start; sub_x <= x; sub_x++)
      {
        if (is_valid_node(y, sub_x) && nodes[int(y)][sub_x]->forbidden_type == ForbiddenType::NONE)
        // if (is_valid_node(y, sub_x) && get_node(sub_x, y)->forbidden_type == ForbiddenType::NONE)
        {
          // nodes[int(y)][sub_x]->forbidden_type = ForbiddenType::CORE;
          nodes[int(y)][sub_x]->forbid(ForbiddenType::CORE);
        }
      }

      x += x_slope;
      y += y_slope;
    }

    x_start = a->x - robot_nodes_count;
    y_start = a->y + robot_nodes_count * 2;
    x_end = b->x + robot_nodes_count * 2;
    y_end = b->y - robot_nodes_count;
    x = x_start;
    y = y_start;

    while (x <= x_end && y >= y_end)
    {
      for (int sub_x = x_start; sub_x <= x; sub_x++)
      {
        if (is_valid_node(y, sub_x) && nodes[int(y)][sub_x]->forbidden_type == ForbiddenType::NONE)
        {
          // nodes[int(y)][sub_x]->forbidden_type = ForbiddenType::ROBOT;
          nodes[int(y)][sub_x]->forbid(ForbiddenType::ROBOT);
        }
      }

      x += x_slope;
      y += y_slope;
    }

    x_start = a->x - robot_nodes_count - buffer_nodes_count;
    y_start = a->y + robot_nodes_count * 2 + buffer_nodes_count * 2;
    x_end = b->x + robot_nodes_count * 2 + buffer_nodes_count * 2;
    y_end = b->y - robot_nodes_count - buffer_nodes_count;
    x = x_start;
    y = y_start;

    while (x <= x_end && y >= y_end)
    {
      for (int sub_x = x_start; sub_x <= x; sub_x++)
      {
        if (is_valid_node(y, sub_x) && nodes[int(y)][sub_x]->forbidden_type == ForbiddenType::NONE)
        {
          // nodes[int(y)][sub_x]->forbidden_type = ForbiddenType::BUFFER;
          nodes[int(y)][sub_x]->forbid(ForbiddenType::BUFFER);
        }
      }

      x += x_slope;
      y += y_slope;
    }
    break;
  }
  case Position::TOP_RIGHT:
  {
    x_slope = x_slope * (-1);
    y_slope = y_slope * (-1);

    while (x >= x_end && y >= y_end)
    {
      for (int sub_x = x_start; sub_x >= x; sub_x--)
      {
        if (is_valid_node(y, sub_x) && nodes[int(y)][sub_x]->forbidden_type == ForbiddenType::NONE)
        {
          // nodes[int(y)][sub_x]->forbidden_type = ForbiddenType::CORE;
          nodes[int(y)][sub_x]->forbid(ForbiddenType::CORE);
        }
      }

      x += x_slope;
      y += y_slope;
    }

    x_start = a->x + robot_nodes_count;
    y_start = a->y + robot_nodes_count * 2;
    x_end = b->x - robot_nodes_count * 2;
    y_end = b->y - robot_nodes_count;
    x = x_start;
    y = y_start;

    while (x >= x_end && y >= y_end)
    {
      for (int sub_x = x_start; sub_x >= x; sub_x--)
      {
        if (is_valid_node(y, sub_x) && nodes[int(y)][sub_x]->forbidden_type == ForbiddenType::NONE)
        {
          // nodes[int(y)][sub_x]->forbidden_type = ForbiddenType::ROBOT;
          nodes[int(y)][sub_x]->forbid(ForbiddenType::ROBOT);
        }
      }

      x += x_slope;
      y += y_slope;
    }

    x_start = a->x + robot_nodes_count + buffer_nodes_count;
    y_start = a->y + robot_nodes_count * 2 + buffer_nodes_count * 2;
    x_end = b->x - robot_nodes_count * 2 - buffer_nodes_count * 2;
    y_end = b->y - robot_nodes_count - buffer_nodes_count;
    x = x_start;
    y = y_start;

    while (x >= x_end && y >= y_end)
    {
      for (int sub_x = x_start; sub_x >= x; sub_x--)
      {
        if (is_valid_node(y, sub_x) && nodes[int(y)][sub_x]->forbidden_type == ForbiddenType::NONE)
        {
          // nodes[int(y)][sub_x]->forbidden_type = ForbiddenType::BUFFER;
          nodes[int(y)][sub_x]->forbid(ForbiddenType::BUFFER);
        }
      }

      x += x_slope;
      y += y_slope;
    }
    break;
  }
  case Position::BOTTOM_RIGHT:
  {
    x_slope = x_slope * (-1);

    while (x >= x_end && y <= y_end)
    {
      for (int sub_x = x_start; sub_x >= x; sub_x--)
      {
        if (is_valid_node(y, sub_x) && nodes[int(y)][sub_x]->forbidden_type == ForbiddenType::NONE)
        {
          // nodes[int(y)][sub_x]->forbidden_type = ForbiddenType::CORE;
          nodes[int(y)][sub_x]->forbid(ForbiddenType::CORE);
        }
      }

      x += x_slope;
      y += y_slope;
    }

    x_start = a->x + robot_nodes_count;
    y_start = a->y - robot_nodes_count * 2;
    x_end = b->x - robot_nodes_count * 2;
    y_end = b->y + robot_nodes_count;
    x = x_start;
    y = y_start;

    while (x >= x_end && y <= y_end)
    {
      for (int sub_x = x_start; sub_x >= x; sub_x--)
      {
        if (is_valid_node(y, sub_x) && nodes[int(y)][sub_x]->forbidden_type == ForbiddenType::NONE)
        {
          // nodes[int(y)][sub_x]->forbidden_type = ForbiddenType::ROBOT;
          nodes[int(y)][sub_x]->forbid(ForbiddenType::ROBOT);
        }
      }

      x += x_slope;
      y += y_slope;
    }

    x_start = a->x + robot_nodes_count + buffer_nodes_count;
    y_start = a->y - robot_nodes_count * 2 - buffer_nodes_count * 2;
    x_end = b->x - robot_nodes_count * 2 - buffer_nodes_count * 2;
    y_end = b->y + robot_nodes_count + buffer_nodes_count;
    x = x_start;
    y = y_start;

    while (x >= x_end && y <= y_end)
    {
      for (int sub_x = x_start; sub_x >= x; sub_x--)
      {
        if (is_valid_node(y, sub_x) && nodes[int(y)][sub_x]->forbidden_type == ForbiddenType::NONE)
        {
          // nodes[int(y)][sub_x]->forbidden_type = ForbiddenType::BUFFER;
          nodes[int(y)][sub_x]->forbid(ForbiddenType::BUFFER);
        }
      }

      x += x_slope;
      y += y_slope;
    }
    break;
  }
  case Position::BOTTOM_LEFT:
  {
    while (x <= x_end && y <= y_end)
    {
      for (int sub_x = x_start; sub_x <= x; sub_x++)
      {
        if (is_valid_node(y, sub_x) && nodes[int(y)][sub_x]->forbidden_type == ForbiddenType::NONE)
        {
          // nodes[int(y)][sub_x]->forbidden_type = ForbiddenType::CORE;
          nodes[int(y)][sub_x]->forbid(ForbiddenType::CORE);
        }
      }

      x += x_slope;
      y += y_slope;
    }

    x_start = a->x - robot_nodes_count;
    y_start = a->y - robot_nodes_count * 2;
    x_end = b->x + robot_nodes_count * 2;
    y_end = b->y + robot_nodes_count;
    x = x_start;
    y = y_start;

    while (x <= x_end && y <= y_end)
    {
      for (int sub_x = x_start; sub_x <= x; sub_x++)
      {
        if (is_valid_node(y, sub_x) && nodes[int(y)][sub_x]->forbidden_type == ForbiddenType::NONE)
        {
          // nodes[int(y)][sub_x]->forbidden_type = ForbiddenType::ROBOT;
          nodes[int(y)][sub_x]->forbid(ForbiddenType::ROBOT);
        }
      }

      x += x_slope;
      y += y_slope;
    }

    x_start = a->x - robot_nodes_count - buffer_nodes_count;
    y_start = a->y - robot_nodes_count * 2 - buffer_nodes_count * 2;
    x_end = b->x + robot_nodes_count * 2 + buffer_nodes_count * 2;
    y_end = b->y + robot_nodes_count + buffer_nodes_count;
    x = x_start;
    y = y_start;

    while (x <= x_end && y <= y_end)
    {
      for (int sub_x = x_start; sub_x <= x; sub_x++)
      {
        if (is_valid_node(y, sub_x) && nodes[int(y)][sub_x]->forbidden_type == ForbiddenType::NONE)
        {
          // nodes[int(y)][sub_x]->forbidden_type = ForbiddenType::BUFFER;
          nodes[int(y)][sub_x]->forbid(ForbiddenType::BUFFER);
        }
      }

      x += x_slope;
      y += y_slope;
    }
    break;
  }
  default:
  {
    throw std::runtime_error("ERROR 12837129");
  }
  }
}

// void Graph::forbid_triangle(Node *a, Node *b, int robot_zones_count, int buffer_zones_count)
// {
//   int x_start = a->x;
//   int y_start = a->y;
//   int x_end = b->x;
//   int y_end = b->y;
//   int x = x_start;
//   int y = y_start;
//   Position position = get_relative_position(b, a);

//   switch (position)
//   {
//   case Position::TOP_LEFT:
//   {
//     while (x <= x_end && y <= y_end)
//     {
//       for (int sub_x = x; sub_x <= x_end; sub_x++)
//       {
//         nodes[int(y)][sub_x]->forbid(ForbiddenType::CORE);
//       }

//       x++;
//       y++;
//     }

//     x_start = a->x - robot_zones_count;
//     y_start = a->y;
//     x_end = b->x;
//     y_end = b->y + robot_zones_count;
//     x = x_start;
//     y = y_start;

//     while (x <= x_end && y <= y_end)
//     {
//       for (int sub_x = x; sub_x <= x_end; sub_x++)
//       {
//         if (nodes[int(y)][sub_x]->forbidden_type != ForbiddenType::NONE)
//         {
//           break;
//         }
//         nodes[int(y)][sub_x]->forbid(ForbiddenType::ROBOT);
//       }

//       x++;
//       y++;
//     }

//     x_start = a->x - robot_zones_count - buffer_zones_count;
//     y_start = a->y;
//     x_end = b->x;
//     y_end = b->y + robot_zones_count + buffer_zones_count;
//     x = x_start;
//     y = y_start;

//     while (x <= x_end && y <= y_end)
//     {
//       for (int sub_x = x; sub_x <= x_end; sub_x++)
//       {
//         if (nodes[int(y)][sub_x]->forbidden_type != ForbiddenType::NONE)
//         {
//           break;
//         }
//         nodes[int(y)][sub_x]->forbid(ForbiddenType::BUFFER);
//       }

//       x++;
//       y++;
//     }
//     break;
//   }
//   case Position::TOP_RIGHT:
//   {
//     while (x <= x_end && y <= y_end)
//     {
//       for (int sub_x = x; sub_x <= x_end; sub_x++)
//       {
//         nodes[int(y)][sub_x]->forbid(ForbiddenType::CORE);
//       }

//       x--;
//       y++;
//     }

//     x_start = a->x - robot_zones_count;
//     y_start = a->y;
//     x_end = b->x;
//     y_end = b->y + robot_zones_count;
//     x = x_start;
//     y = y_start;

//     while (x <= x_end && y <= y_end)
//     {
//       for (int sub_x = x; sub_x <= x_end; sub_x++)
//       {
//         if (nodes[int(y)][sub_x]->forbidden_type != ForbiddenType::NONE)
//         {
//           break;
//         }
//         nodes[int(y)][sub_x]->forbid(ForbiddenType::ROBOT);
//       }

//       x--;
//       y++;
//     }

//     x_start = a->x - robot_zones_count - buffer_zones_count;
//     y_start = a->y;
//     x_end = b->x;
//     y_end = b->y + robot_zones_count + buffer_zones_count;
//     x = x_start;
//     y = y_start;

//     while (x <= x_end && y <= y_end)
//     {
//       for (int sub_x = x; sub_x <= x_end; sub_x++)
//       {
//         if (nodes[int(y)][sub_x]->forbidden_type != ForbiddenType::NONE)
//         {
//           break;
//         }
//         nodes[int(y)][sub_x]->forbid(ForbiddenType::BUFFER);
//       }

//       x--;
//       y++;
//     }
//     break;
//   }
//   case Position::BOTTOM_RIGHT:
//   {
//     while (x <= x_end && y <= y_end)
//     {
//       for (int sub_x = x; sub_x <= x_end; sub_x++)
//       {
//         nodes[int(y)][sub_x]->forbid(ForbiddenType::CORE);
//       }

//       x++;
//       y++;
//     }

//     x_start = a->x - robot_zones_count;
//     y_start = a->y;
//     x_end = b->x;
//     y_end = b->y + robot_zones_count;
//     x = x_start;
//     y = y_start;

//     while (x <= x_end && y <= y_end)
//     {
//       for (int sub_x = x; sub_x <= x_end; sub_x++)
//       {
//         if (nodes[int(y)][sub_x]->forbidden_type != ForbiddenType::NONE)
//         {
//           break;
//         }
//         nodes[int(y)][sub_x]->forbid(ForbiddenType::ROBOT);
//       }

//       x++;
//       y++;
//     }

//     x_start = a->x - robot_zones_count - buffer_zones_count;
//     y_start = a->y;
//     x_end = b->x;
//     y_end = b->y + robot_zones_count + buffer_zones_count;
//     x = x_start;
//     y = y_start;

//     while (x <= x_end && y <= y_end)
//     {
//       for (int sub_x = x; sub_x <= x_end; sub_x++)
//       {
//         if (nodes[int(y)][sub_x]->forbidden_type != ForbiddenType::NONE)
//         {
//           break;
//         }
//         nodes[int(y)][sub_x]->forbid(ForbiddenType::BUFFER);
//       }

//       x++;
//       y++;
//     }
//     break;
//   }
//   case Position::BOTTOM_LEFT:
//   {
//     while (x <= x_end && y <= y_end)
//     {
//       for (int sub_x = x; sub_x <= x_end; sub_x++)
//       {
//         nodes[int(y)][sub_x]->forbid(ForbiddenType::CORE);
//       }

//       x++;
//       y++;
//     }

//     x_start = a->x - robot_zones_count;
//     y_start = a->y;
//     x_end = b->x;
//     y_end = b->y + robot_zones_count;
//     x = x_start;
//     y = y_start;

//     while (x <= x_end && y <= y_end)
//     {
//       for (int sub_x = x; sub_x <= x_end; sub_x++)
//       {
//         if (nodes[int(y)][sub_x]->forbidden_type != ForbiddenType::NONE)
//         {
//           break;
//         }
//         nodes[int(y)][sub_x]->forbid(ForbiddenType::ROBOT);
//       }

//       x++;
//       y++;
//     }

//     x_start = a->x - robot_zones_count - buffer_zones_count;
//     y_start = a->y;
//     x_end = b->x;
//     y_end = b->y + robot_zones_count + buffer_zones_count;
//     x = x_start;
//     y = y_start;

//     while (x <= x_end && y <= y_end)
//     {
//       for (int sub_x = x; sub_x <= x_end; sub_x++)
//       {
//         if (nodes[int(y)][sub_x]->forbidden_type != ForbiddenType::NONE)
//         {
//           break;
//         }
//         nodes[int(y)][sub_x]->forbid(ForbiddenType::BUFFER);
//       }

//       x++;
//       y++;
//     }
//     break;
//   }
//   default:
//   {
//     throw std::runtime_error("ERROR 00000");
//     break;
//   }
//   }
// }

// void Graph::forbid_triangle(Node *a, Node *b, int robot_zones_count, int buffer_zones_count)
// {
//   int bx = b->x;
//   int by = b->y;
//   int x = a->x;
//   int y = a->y;
//   int xInitial = x;
//   Position relativePosition = get_relative_position(a, b);

//   // CORE FORBIDDEN NODES
//   switch (relativePosition)
//   {
//   case Position::TOP:
//   case Position::RIGHT:
//   case Position::BOTTOM:
//   case Position::LEFT:
//   {
//     throw std::runtime_error("ERROR 600");
//     break;
//   }
//   // start at a
//   // go top left one step at a time
//   // each time, from that point to the right most point (xInitial, which is a->x), go through those nodes and flag them as forbidden
//   case Position::TOP_LEFT:
//   {
//     while (x >= bx && y >= by)
//     {
//       for (int xx = xInitial; xx >= x; xx--)
//       {
//         nodes[y][xx]->forbid(ForbiddenType::CORE);
//       }

//       x--;
//       y--;
//     }

//     x = a->x break;
//   }
//   case Position::TOP_RIGHT:
//   {
//     while (x <= bx && y >= by)
//     {
//       for (int xx = xInitial; xx <= x; xx++)
//       {
//         nodes[y][xx]->forbid(ForbiddenType::CORE);
//       }

//       x++;
//       y--;
//     }
//     break;
//   }
//   case Position::BOTTOM_RIGHT:
//   {
//     while (x <= bx && y <= by)
//     {
//       for (int xx = xInitial; xx <= x; xx++)
//       {
//         nodes[y][xx]->forbid(ForbiddenType::CORE);
//       }

//       x++;
//       y++;
//     }
//     break;
//   }
//   case Position::BOTTOM_LEFT:
//   {
//     while (x >= bx && y <= by)
//     {
//       for (int xx = xInitial; xx >= x; xx--)
//       {
//         nodes[y][xx]->forbid(ForbiddenType::CORE);
//       }

//       x--;
//       y++;
//     }
//     break;
//   }
//   default:
//     throw std::runtime_error("ERROR 500");
//     break;
//   }

//   x =
//       // ROBOT FORBIDDEN NODES
//       switch (relativePosition)
//   {
//   case Position::TOP:
//   case Position::RIGHT:
//   case Position::BOTTOM:
//   case Position::LEFT:
//   {
//     throw std::runtime_error("ERROR 600");
//     break;
//   }
//   // start at a
//   // go top left one step at a time
//   // each time, from that point to the right most point (xInitial, which is a->x), go through those nodes and flag them as forbidden
//   case Position::TOP_LEFT:
//   {
//     while (x >= bx && y >= by)
//     {
//       for (int xx = xInitial; xx >= x; xx--)
//       {
//         nodes[y][xx]->forbid(ForbiddenType::CORE);
//       }

//       x--;
//       y--;
//     }

//     x = a->x break;
//   }
//   case Position::TOP_RIGHT:
//   {
//     while (x <= bx && y >= by)
//     {
//       for (int xx = xInitial; xx <= x; xx++)
//       {
//         nodes[y][xx]->forbid(ForbiddenType::CORE);
//       }

//       x++;
//       y--;
//     }
//     break;
//   }
//   case Position::BOTTOM_RIGHT:
//   {
//     while (x <= bx && y <= by)
//     {
//       for (int xx = xInitial; xx <= x; xx++)
//       {
//         nodes[y][xx]->forbid(ForbiddenType::CORE);
//       }

//       x++;
//       y++;
//     }
//     break;
//   }
//   case Position::BOTTOM_LEFT:
//   {
//     while (x >= bx && y <= by)
//     {
//       for (int xx = xInitial; xx >= x; xx--)
//       {
//         nodes[y][xx]->forbid(ForbiddenType::CORE);
//       }

//       x--;
//       y++;
//     }
//     break;
//   }
//   default:
//     throw std::runtime_error("ERROR 500");
//     break;
//   }

//   // BUFFER FORBIDDEN NODES
//   switch (relativePosition)
//   {
//   case Position::TOP:
//   case Position::RIGHT:
//   case Position::BOTTOM:
//   case Position::LEFT:
//   {
//     throw std::runtime_error("ERROR 600");
//     break;
//   }
//   // start at a
//   // go top left one step at a time
//   // each time, from that point to the right most point (xInitial, which is a->x), go through those nodes and flag them as forbidden
//   case Position::TOP_LEFT:
//   {
//     while (x >= bx && y >= by)
//     {
//       for (int xx = xInitial; xx >= x; xx--)
//       {
//         nodes[y][xx]->forbid(ForbiddenType::CORE);
//       }

//       x--;
//       y--;
//     }

//     x = a->x break;
//   }
//   case Position::TOP_RIGHT:
//   {
//     while (x <= bx && y >= by)
//     {
//       for (int xx = xInitial; xx <= x; xx++)
//       {
//         nodes[y][xx]->forbid(ForbiddenType::CORE);
//       }

//       x++;
//       y--;
//     }
//     break;
//   }
//   case Position::BOTTOM_RIGHT:
//   {
//     while (x <= bx && y <= by)
//     {
//       for (int xx = xInitial; xx <= x; xx++)
//       {
//         nodes[y][xx]->forbid(ForbiddenType::CORE);
//       }

//       x++;
//       y++;
//     }
//     break;
//   }
//   case Position::BOTTOM_LEFT:
//   {
//     while (x >= bx && y <= by)
//     {
//       for (int xx = xInitial; xx >= x; xx--)
//       {
//         nodes[y][xx]->forbid(ForbiddenType::CORE);
//       }

//       x--;
//       y++;
//     }
//     break;
//   }
//   default:
//     throw std::runtime_error("ERROR 500");
//     break;
//   }
// }

void Graph::forbid_rectangle(Node *topLeftPoint, Node *topRightPoint, Node *bottomLeftPoint, int robot_zones_count, int buffer_zones_count)
{
  for (int y = topLeftPoint->y; y <= bottomLeftPoint->y; y++)
  {
    for (int x = topLeftPoint->x; x <= topRightPoint->x; x++)
    {
      nodes[y][x]->forbid(ForbiddenType::CORE);
    }
  }
}

int Graph::get_edge_cost(Node *a, Node *b)
{
  // return 10;
  // if nodes are cardinal neighbors
  if ((b->x == a->x + 1 && b->y == a->y) || (b->x == a->x - 1 && b->y == a->y) || (b->x == a->x && b->y == a->y + 1) || (b->x == a->x && b->y == a->y - 1))
  {
    return 10;
  }
  // if nodes are diagonal neighbors
  else if ((b->x == a->x + 1 && b->y == a->y + 1) || (b->x == a->x + 1 && b->y == a->y - 1) || (b->x == a->x - 1 && b->y == a->y + 1) || (b->x == a->x - 1 && b->y == a->y - 1))
  {
    return 14;
  }
  else
  {
    // Euclidean distance
    int x1 = a->x;
    int y1 = a->y;
    int x2 = b->x;
    int y2 = b->y;
    // return std::sqrt(std::pow(x2 - x1, 2) * 10 + std::pow(y2 - y1, 2) * 10);
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));

    // throw std::runtime_error("ERROR 001");
  }
}

std::vector<Node *> *Graph::reconstruct_path(Node *currentNode, std::map<Node *, Node *> cameFrom)
{
  path_nodes = new std::vector<Node *>;

  while (currentNode != cameFrom[currentNode])
  {
    path_nodes->push_back(currentNode);
    currentNode = cameFrom[currentNode];
  }

  path_nodes->push_back(currentNode);

  return path_nodes;
}

// Returns the position of b relative to a
Position Graph::get_relative_position(Node *a, Node *b)
{
  int ax = a->x;
  int ay = a->y;
  int bx = b->x;
  int by = b->y;

  if (bx == ax && by < ay)
  {
    return Position::TOP;
  }
  else if (bx > ax && by == ay)
  {
    return Position::RIGHT;
  }
  else if (bx == ax && by > ay)
  {
    return Position::BOTTOM;
  }
  else if (bx < ax && by == ay)
  {
    return Position::LEFT;
  }
  else if (bx < ax && by < ay)
  {
    return Position::TOP_LEFT;
  }
  else if (bx > ax && by < ay)
  {
    return Position::TOP_RIGHT;
  }
  else if (bx > ax && by > ay)
  {
    return Position::BOTTOM_RIGHT;
  }
  else if (bx < ax && by > ay)
  {
    return Position::BOTTOM_LEFT;
  }
  else
  {
    throw std::runtime_error("ERROR 004");
  }
}

void Graph::add_edge(Node *a, Node *b)
{
  a->add_neighbor(b);
  b->add_neighbor(a);
}

void Graph::remove_edge(Node *a, Node *b)
{
  a->remove_neighbor(b);
  b->remove_neighbor(a);
}

void Graph::add_forbidden_zones(Graph *graph)
{
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

  forbid_triangle(
      graph->get_node(0, 0),
      graph->get_node(0, ROLLER_SIZE),
      graph->get_node(ROLLER_SIZE, 0),
      ROBOT_ZONES_COUNT,
      BUFFER_ZONES_COUNT);
  forbid_triangle(
      graph->get_node(FIELD_SIZE - 1, 0),
      graph->get_node(FIELD_SIZE - 1, ROLLER_SIZE),
      graph->get_node(FIELD_SIZE - 1 - ROLLER_SIZE, 0),
      ROBOT_ZONES_COUNT,
      BUFFER_ZONES_COUNT);
  forbid_triangle(
      graph->get_node(0, FIELD_SIZE - 1),
      graph->get_node(0, FIELD_SIZE - 1 - ROLLER_SIZE),
      graph->get_node(ROLLER_SIZE, FIELD_SIZE - 1),
      ROBOT_ZONES_COUNT,
      BUFFER_ZONES_COUNT);
  forbid_triangle(
      graph->get_node(FIELD_SIZE - 1, FIELD_SIZE - 1),
      graph->get_node(FIELD_SIZE - 1, FIELD_SIZE - 1 - ROLLER_SIZE),
      graph->get_node(FIELD_SIZE - 1 - ROLLER_SIZE, FIELD_SIZE - 1),
      ROBOT_ZONES_COUNT,
      BUFFER_ZONES_COUNT);

  forbid_rectangle(graph->get_node(GOAL_START_X, GOAL_START_Y),
                   graph->get_node(GOAL_START_X + GOAL_SIZE_X, GOAL_START_Y),
                   graph->get_node(GOAL_START_X, GOAL_START_Y + GOAL_SIZE_Y),
                   ROBOT_ZONES_COUNT,
                   BUFFER_ZONES_COUNT);
  forbid_rectangle(graph->get_node(GOAL_START_X, FIELD_SIZE - 1 - GOAL_SIZE_Y),
                   graph->get_node(GOAL_START_X + GOAL_SIZE_X, FIELD_SIZE - 1 - GOAL_SIZE_Y),
                   graph->get_node(GOAL_START_X, FIELD_SIZE - 1),
                   ROBOT_ZONES_COUNT,
                   BUFFER_ZONES_COUNT);
  forbid_rectangle(graph->get_node(BAR_LEFT_START_X, BAR_LEFT_START_Y),
                   graph->get_node(BAR_LEFT_START_X + BAR_LEFT_SIZE_X, BAR_LEFT_START_Y),
                   graph->get_node(BAR_LEFT_START_X, BAR_LEFT_START_Y + BAR_LEFT_SIZE_Y),
                   ROBOT_ZONES_COUNT,
                   BUFFER_ZONES_COUNT);
  forbid_rectangle(graph->get_node(BAR_RIGHT_START_X, BAR_RIGHT_START_Y),
                   graph->get_node(BAR_RIGHT_START_X + BAR_RIGHT_SIZE_X, BAR_RIGHT_START_Y),
                   graph->get_node(BAR_RIGHT_START_X, BAR_RIGHT_START_Y + BAR_RIGHT_SIZE_Y),
                   ROBOT_ZONES_COUNT,
                   BUFFER_ZONES_COUNT);
  forbid_rectangle(graph->get_node(BAR_MAIN_START_X, BAR_MAIN_START_Y),
                   graph->get_node(BAR_MAIN_START_X + BAR_MAIN_SIZE_X, BAR_MAIN_START_Y),
                   graph->get_node(BAR_MAIN_START_X, BAR_MAIN_START_Y + BAR_MAIN_SIZE_Y),
                   ROBOT_ZONES_COUNT,
                   BUFFER_ZONES_COUNT);
}
//

//
//
//
//
// Graph::Graph(int X_NODES_COUNT, int Y_NODES_COUNT, double NODE_SIZE)
//     : Graph::Graph(X_NODES_COUNT, Y_NODES_COUNT, NODE_SIZE, GraphType::VG),
//       waypoints(new std::set<Node *>)
// {
//   find_waypoints();
//   add_neighbor_waypoints();
// }

void Graph::insert_waypoint(Node *node)
{
  for (Node *waypoint : (*waypoints))
  {
    bool cond = check_LOS(node, waypoint);

    if (cond)
    {
      node->add_waypoint_neighbor(waypoint);
      waypoint->add_waypoint_neighbor(node);
    }
  }

  waypoints->insert(node);
}

// TODO - IMPORTANT - editing list/set as you're iterating over it, look for it elsewhere
void Graph::remove_waypoint(Node *node)
{
  std::set<Node *> toBeRemoved;

  for (Node *waypoint_neighbor : (*node->get_waypoint_neighbors()))
  {
    toBeRemoved.insert(waypoint_neighbor);
    waypoint_neighbor->remove_waypoint_neighbor(node);
  }

  for (Node *waypoint_neighbor : toBeRemoved)
  {
    node->remove_waypoint_neighbor(waypoint_neighbor);
  }

  waypoints->erase(node);
}

// Octile distance
int Graph::get_h_cost(Node *currentNode, Node *destination)
{
  int x1 = currentNode->x;
  int y1 = currentNode->y;
  int x2 = destination->x;
  int y2 = destination->y;
  // TODO - which one?
  // return std::sqrt(std::pow(x2 - x1, 2) * 10 + std::pow(y2 - y1, 2) * 10);
  return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

std::vector<Node *> *Graph::get_path(Node *origin, Node *destination)
{
  std::set<Node *> frontier;
  std::set<Node *> closed;
  std::map<Node *, Node *> cameFrom;
  std::map<Node *, int> gScores;
  std::map<Node *, int> fScores;

  frontier.insert(origin);
  cameFrom[origin] = origin;
  gScores[origin] = 0;
  fScores[origin] = get_h_cost(origin, destination);

  insert_waypoint(origin);
  insert_waypoint(destination);

  while (frontier.size() > 0)
  {
    int lowestFScore = MAX_INT;
    int lowestHScore = MAX_INT;
    int lowestGScore = MAX_INT;
    int highestGScore = 0;
    Node *currentNode;

    for (Node *node : frontier)
    {
      if (fScores[node] < lowestFScore)
      {
        lowestFScore = fScores[node];
        currentNode = node;
      }
      else if (fScores[node] == lowestFScore && get_h_cost(node, destination) < get_h_cost(currentNode, destination))
      {
        currentNode = node;
      }
    }

    frontier.erase(currentNode);

    if (currentNode == destination)
    {
      remove_waypoint(origin);
      remove_waypoint(destination);
      return reconstruct_path(currentNode, cameFrom);
    }

    for (Node *neighbor : (*currentNode->get_waypoint_neighbors()))
    {
      if (closed.find(neighbor) == closed.end())
      {
        int neighborGScore = gScores[currentNode] + get_edge_cost(currentNode, neighbor);

        if (gScores.find(neighbor) == gScores.end() || neighborGScore < gScores[neighbor])
        {
          cameFrom[neighbor] = currentNode;
          gScores[neighbor] = neighborGScore;
          fScores[neighbor] = neighborGScore + get_edge_cost(neighbor, destination);

          if (closed.find(neighbor) == closed.end() && frontier.find(neighbor) == frontier.end())
          {
            frontier.insert(neighbor);
          }
        }
      }
    }

    // Remove this?
    closed.insert(currentNode);
  }
  throw std::runtime_error("ERROR 002");
}

std::vector<std::vector<Node *> *> *Graph::get_path_snapshots(Node *origin, Node *destination)
{
  std::set<Node *> frontier;
  std::set<Node *> closed;
  std::map<Node *, Node *> cameFrom;
  std::map<Node *, int> gScores;
  std::map<Node *, int> fScores;
  path_nodes_snapshots = new std::vector<std::vector<Node *> *>;

  frontier.insert(origin);
  cameFrom[origin] = origin;
  gScores[origin] = 0;
  fScores[origin] = get_edge_cost(origin, destination);

  insert_waypoint(origin);
  insert_waypoint(destination);

  while (frontier.size() > 0)
  {
    path_nodes_snapshots->push_back(new std::vector<Node *>);
    int lowestFScore = MAX_INT;
    int lowestHScore = MAX_INT;
    int lowestGScore = MAX_INT;
    int highestGScore = 0;
    Node *currentNode;

    for (Node *node : frontier)
    {
      if (get_h_cost(node, destination) < lowestHScore)
      {
        lowestHScore = get_h_cost(node, destination);
        currentNode = node;
      }
      else if (get_h_cost(node, destination) == lowestHScore && gScores[node] < lowestGScore)
      {
        lowestGScore = gScores[node];
        currentNode = node;
      }
    }

    Node *temp = currentNode;
    path_nodes_snapshots->back()->push_back(temp);
    while (temp != cameFrom[temp])
    {
      path_nodes_snapshots->back()->push_back(cameFrom[temp]);
      temp = cameFrom[temp];
    }

    frontier.erase(currentNode);

    if (currentNode == destination)
    {
      remove_waypoint(origin);
      remove_waypoint(destination);
      return path_nodes_snapshots;
    }

    for (Node *neighbor : (*currentNode->get_waypoint_neighbors()))
    {
      if (closed.find(neighbor) == closed.end())
      {
        int neighborGScore = gScores[currentNode] + get_edge_cost(currentNode, neighbor);

        if (gScores.find(neighbor) == gScores.end() || neighborGScore < gScores[neighbor])
        {
          cameFrom[neighbor] = currentNode;
          gScores[neighbor] = neighborGScore;
          fScores[neighbor] = neighborGScore + get_edge_cost(neighbor, destination);

          if (closed.find(neighbor) == closed.end() && frontier.find(neighbor) == frontier.end())
          {
            frontier.insert(neighbor);
          }
        }
      }
    }

    // Remove this?
    closed.insert(currentNode);
  }
  throw std::runtime_error("ERROR 100");
}

std::vector<Node *> *Graph::get_random_path()
{
  srand(time(0));

  std::mt19937 rng(rand());
  std::uniform_int_distribution<int> randX(0, X_NODES_COUNT - 1);
  std::uniform_int_distribution<int> randY(0, Y_NODES_COUNT - 1);

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
  } while (nodes[originY][originX]->get_is_forbidden() || nodes[destinationY][destinationX]->get_is_forbidden() || (originX == destinationX && originY == destinationY));

  if (originX == destinationX && originY == destinationY)
  {
    throw std::runtime_error("ERROR SAME ORIGIN AND DESTINATION");
  }

  return get_path(nodes[originY][originX], nodes[destinationY][destinationX]);
}

std::vector<std::vector<Node *> *> *Graph::get_random_path_snapshots()
{
  srand(time(0));

  // std::random_device rd;
  std::mt19937 rng(rand());
  std::uniform_int_distribution<int> randX(0, X_NODES_COUNT - 1);
  std::uniform_int_distribution<int> randY(0, Y_NODES_COUNT - 1);

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
  } while (nodes[originY][originX]->get_is_forbidden() || nodes[destinationY][destinationX]->get_is_forbidden() || (originX == destinationX && originY == destinationY));

  if (originX == destinationX && originY == destinationY)
  {
    throw std::runtime_error("ERROR SAME ORIGIN AND DESTINATION");
  }

  return get_path_snapshots(nodes[originY][originX], nodes[destinationY][destinationX]);
}

std::set<Node *> *Graph::get_waypoints()
{

  return waypoints;
}

// can store this data on the node itself
// is a waypoint if:
// * is not forbidden
// * top, left, bottom, right are not forbidden
// * topleft topright, topright bottomright, bottomright bottomleft, bottomleft topleft are not forbidden
// * topleft, topright, bottomright, or bottomleft is forbidden
// FIX THIS, idk how
Position Graph::get_is_waypoint(Node *node)
{
  int x = node->x;
  int y = node->y;

  bool hasTopNeighbor = y - 1 >= 0;
  bool hasBottomNeighbor = y + 1 <= Y_NODES_COUNT - 1;
  bool hasLeftNeighbor = x - 1 >= 0;
  bool hasRightNeighbor = x + 1 <= X_NODES_COUNT - 1;

  bool hasTopLeftNeighbor = hasTopNeighbor && hasLeftNeighbor;
  bool hasTopRightNeighbor = hasTopNeighbor && hasRightNeighbor;
  bool hasBottomRightNeighbor = hasBottomNeighbor && hasRightNeighbor;
  bool hasBottomLeftNeighbor = hasBottomNeighbor && hasLeftNeighbor;

  bool topNeighborForbidden = hasTopNeighbor && nodes[y - 1][x]->get_is_forbidden();
  bool rightNeighborForbidden = hasRightNeighbor && nodes[y][x + 1]->get_is_forbidden();
  bool bottomNeighborForbidden = hasBottomNeighbor && nodes[y + 1][x]->get_is_forbidden();
  bool leftNeighborForbidden = hasLeftNeighbor && nodes[y][x - 1]->get_is_forbidden();

  bool topLeftNeighborForbidden = hasTopLeftNeighbor && nodes[y - 1][x - 1]->get_is_forbidden();
  bool topRightNeighborForbidden = hasTopRightNeighbor && nodes[y - 1][x + 1]->get_is_forbidden();
  bool bottomRightNeighborForbidden = hasBottomRightNeighbor && nodes[y + 1][x + 1]->get_is_forbidden();
  bool bottomLeftNeighborForbidden = hasBottomLeftNeighbor && nodes[y + 1][x - 1]->get_is_forbidden();

  // if ((!node->get_is_forbidden()) &&
  //     (!topNeighborForbidden && !rightNeighborForbidden && !bottomNeighborForbidden && !leftNeighborForbidden) &&
  //     ((!(topLeftNeighborForbidden && topRightNeighborForbidden) && !(topRightNeighborForbidden && bottomRightNeighborForbidden) && !(bottomRightNeighborForbidden && bottomLeftNeighborForbidden) && !(bottomLeftNeighborForbidden && topLeftNeighborForbidden))))
  // {
  if ((!node->get_is_forbidden()) &&
      (!topNeighborForbidden && !rightNeighborForbidden && !bottomNeighborForbidden && !leftNeighborForbidden) &&
      ((!(topLeftNeighborForbidden && topRightNeighborForbidden) && !(topRightNeighborForbidden && bottomRightNeighborForbidden) && !(bottomRightNeighborForbidden && bottomLeftNeighborForbidden) && !(bottomLeftNeighborForbidden && topLeftNeighborForbidden))))
  {
    if (topLeftNeighborForbidden)
    {
      return Position::BOTTOM_RIGHT;
    }
    else if (topRightNeighborForbidden)
    {
      return Position::BOTTOM_LEFT;
    }
    else if (bottomRightNeighborForbidden)
    {
      return Position::TOP_LEFT;
    }
    else if (bottomLeftNeighborForbidden)
    {
      return Position::TOP_RIGHT;
    }
  }
  else
  {
    return Position::NONE;
    throw std::runtime_error("ERROR 110");
  }

  if ((topLeftNeighborForbidden && topRightNeighborForbidden) ||
      (topRightNeighborForbidden && bottomRightNeighborForbidden) ||
      (bottomRightNeighborForbidden && bottomLeftNeighborForbidden) ||
      (bottomLeftNeighborForbidden && topLeftNeighborForbidden))
  {
    if (!topNeighborForbidden && !rightNeighborForbidden && !bottomNeighborForbidden && !leftNeighborForbidden)
    {
      throw std::runtime_error("ERROR 200");
    }
  }

  if (topNeighborForbidden ||
      bottomNeighborForbidden ||
      leftNeighborForbidden ||
      rightNeighborForbidden)
  {
    return Position::NONE;
  }
  else if (bottomRightNeighborForbidden)
  {
    return Position::TOP_LEFT;
  }
  else if (bottomLeftNeighborForbidden)
  {
    return Position::TOP_RIGHT;
  }
  if (topLeftNeighborForbidden)
  {
    return Position::BOTTOM_RIGHT;
  }
  else if (topRightNeighborForbidden)
  {
    return Position::BOTTOM_LEFT;
  }

  return Position::NONE;
}

// Checks if b is taut to a
bool Graph::get_is_taut(Node *a, Node *b)
{
  switch (a->get_waypoint())
  {
  case Position::TOP_LEFT:
  {
    switch (get_relative_position(a, b))
    {
    case Position::TOP:
    case Position::RIGHT:
    case Position::BOTTOM:
    case Position::LEFT:
    case Position::TOP_RIGHT:
    case Position::BOTTOM_LEFT:
    {
      return true;
      break;
    }
    }
  }
  case Position::TOP_RIGHT:
  {
    switch (get_relative_position(a, b))
    {
    case Position::TOP:
    case Position::RIGHT:
    case Position::BOTTOM:
    case Position::LEFT:
    case Position::TOP_LEFT:
    case Position::BOTTOM_RIGHT:
    {
      return true;
      break;
    }
    }
    break;
  }
  case Position::BOTTOM_RIGHT:
  {
    switch (get_relative_position(a, b))
    {
    case Position::TOP:
    case Position::RIGHT:
    case Position::BOTTOM:
    case Position::LEFT:
    case Position::TOP_RIGHT:
    case Position::BOTTOM_LEFT:
    {
      return true;
      break;
    }
    }
    break;
  }
  case Position::BOTTOM_LEFT:
  {
    switch (get_relative_position(a, b))
    {
    case Position::TOP:
    case Position::RIGHT:
    case Position::BOTTOM:
    case Position::LEFT:
    case Position::TOP_LEFT:
    case Position::BOTTOM_RIGHT:
    {
      return true;
      break;
    }
    }
    break;
  }
  }

  return false;
}

// Checks if both a and b are taut waypoints to each other
bool Graph::get_are_taut_waypoints(Node *a, Node *b)
{
  if (get_is_taut(a, b) && get_is_taut(b, a))
  {
    return true;
  }
  return false;
}

// TODO - IMPORTANT - make this function check from center of a to center of b
// TODO - top, right, bottom, left trivial, no slope need, go one node at a time
bool Graph::check_LOS(Node *a, Node *b)
{

  if (a == b)
  {
    return true;
  }

  double x = a->x + (1 / 2);
  double y = a->y + (1 / 2);
  // double dx = (b->x + (NODE_SIZE / 2)) - a->x;
  // double dy = (b->y + (NODE_SIZE / 2)) - a->y;
  // TODO - dx or dy can never be zero because adding half. Add half before taking distance? Can result in rounding error. Take difference first then check if zero otherwise add half?
  double dx = ((b->x - a->x) + (1 / 2));
  double dy = ((b->y - a->y) + (1 / 2));
  double xSlope;
  double ySlope;

  if (dy == 0)
  {
    xSlope = dx / (100 * dx);
  }
  else
  {
    xSlope = (dx / dy) / 100;
  }

  if (dx == 0)
  {
    ySlope = dy / (100 * dy);
  }
  else
  {
    ySlope = (dy / dx) / 100;
  }

  ySlope = dy / 140;
  xSlope = dx / 140;

  if (xSlope < 0)
  {
    xSlope *= (-1.0);
  }
  if (ySlope < 0)
  {
    ySlope *= (-1.0);
  }

  switch (get_relative_position(a, b))
  {
  case Position::TOP_LEFT:
  {
    xSlope *= (-1.0);
    ySlope *= (-1.0);
    while (y + ySlope >= b->y && x + xSlope >= b->x)
    {
      y += ySlope;
      x += xSlope;

      if (nodes[int(y)][int(x)]->get_is_forbidden())
      {
        // nodes[int(y)][int(x)]->hit = true;
        return false;
      }
    }
    break;
  }
  case Position::TOP_RIGHT:
  case Position::TOP:
  {
    ySlope *= (-1);
    while (y + ySlope >= b->y && x + xSlope <= b->x)
    {
      y += ySlope;
      x += xSlope;

      if (nodes[int(y)][int(x)]->get_is_forbidden())
      {
        // nodes[int(y)][int(x)]->hit = true;
        return false;
      }
    }
    break;
  }
  case Position::BOTTOM_RIGHT:
  case Position::BOTTOM:
  case Position::RIGHT:
    // FIX: same y, different x, xSlope = dx / (10 * dx), dx = number of nodes (+1 for inclusive?)
    {
      while (y + ySlope <= b->y && x + xSlope <= b->x)
      {
        y += ySlope;
        x += xSlope;

        if (nodes[int(y)][int(x)]->get_is_forbidden())
        {
          // nodes[int(y)][int(x)]->hit = true;
          return false;
        }
      }
      break;
    }
  case Position::BOTTOM_LEFT:
  case Position::LEFT:
  {
    xSlope *= (-1);
    while (y + ySlope <= b->y && x + xSlope >= b->x)
    {
      y += ySlope;
      x += xSlope;

      if (nodes[int(y)][int(x)]->get_is_forbidden())
      {
        // nodes[int(y)][int(x)]->hit = true;
        return false;
      }
    }
    break;
  }
  default:
    throw std::runtime_error("9000");
    break;
  }

  return true;

  // while ((y + ySlope >= 0 && y + ySlope <= Y_NODES_COUNT - 1) && (x + xSlope >= 0 && x + xSlope <= X_NODES_COUNT - 1))
  while ((y + ySlope >= 0 && y + ySlope <= Y_NODES_COUNT - 1) && (x + xSlope >= 0 && x + xSlope <= X_NODES_COUNT - 1))
  {
    y += ySlope;
    x += xSlope;

    if (nodes[int(y)][int(x)]->get_is_forbidden())
    {
      return false;
    }
  }

  return true;
}

// Before doing a line of check, make sure they are taut waypoints
void Graph::find_waypoints()
{
  for (int y = 0; y < Y_NODES_COUNT; y++)
  {
    for (int x = 0; x < X_NODES_COUNT; x++)
    {
      Node *node = nodes[y][x];

      if (node->get_is_forbidden())
      {
        continue;
      }

      Position corner = get_is_waypoint(node);
      node->set_waypoint(corner);

      switch (corner)
      {
      case Position::NONE:
      {
        break;
      }
      case Position::TOP_LEFT:
      case Position::TOP_RIGHT:
      case Position::BOTTOM_RIGHT:
      case Position::BOTTOM_LEFT:
      {
        waypoints->insert(node);
        break;
      }
      default:
      {
        throw std::runtime_error("ERROR 700");
        break;
      }
      }
    }
  }
}

void Graph::add_neighbor_waypoints()
{
  for (Node *nodeA : (*waypoints))
  {
    for (Node *nodeB : (*waypoints))
    {
      if (nodeA != nodeB)
      {
        if (get_are_taut_waypoints(nodeA, nodeB) && check_LOS(nodeA, nodeB))
        {
          nodeA->add_waypoint_neighbor(nodeB);
          nodeB->add_waypoint_neighbor(nodeA);
          // nodeA->get_waypoint_neighbors().insert(nodeB);
          // nodeB->get_waypoint_neighbors().insert(nodeA);
        }
      }
    }
  }
}
