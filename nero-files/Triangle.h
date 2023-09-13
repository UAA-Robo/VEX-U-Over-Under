#pragma once
#include <tuple>

class Triangle
{
public:
  int points;
  std::tuple<int, int> rightAnglePoint;
  std::tuple<int, int> sidePointA;
  std::tuple<int, int> sidePointB;

  Triangle(std::tuple<int, int> rightAnglePoint, std::tuple<int, int> sidePointA, std::tuple<int, int> sidePointB)
  {
    this->points = 3;
    this->rightAnglePoint = rightAnglePoint;
    this->sidePointA = sidePointA;
    this->sidePointB = sidePointB;
  }
};