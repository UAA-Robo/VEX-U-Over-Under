#pragma once
#include <tuple>

class Rectangle
{
public:
  int points;
  std::tuple<int, int> topLeftPoint;
  std::tuple<int, int> topRightPoint;
  std::tuple<int, int> bottomLeftPoint;
  std::tuple<int, int> bottomRightPoint;

  Rectangle(std::tuple<int, int> topLeftPoint, std::tuple<int, int> topRightPoint, std::tuple<int, int> bottomLeftPoint, std::tuple<int, int> bottomRightPoint)
  {
    this->points = 4;
    this->topLeftPoint = topLeftPoint;
    this->topRightPoint = topRightPoint;
    this->bottomLeftPoint = bottomLeftPoint;
    this->bottomRightPoint = bottomRightPoint;
  }
};
