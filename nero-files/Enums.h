#pragma once

// TODO - merge Position and Waypoint

enum class Position
{
  NONE,
  CENTER,
  TOP,
  RIGHT,
  BOTTOM,
  LEFT,
  TOP_LEFT,
  TOP_RIGHT,
  BOTTOM_RIGHT,
  BOTTOM_LEFT,
};

// enum class Waypoint
// {
//   NONE,
//   TOP_LEFT,
//   TOP_RIGHT,
//   BOTTOM_RIGHT,
//   BOTTOM_LEFT
// };

enum class GraphType
{
  NORMAL,
  VG
};

enum class GUIMode
{
  SIMPLE,
  SNAPSHOTS,
  LOS
};
