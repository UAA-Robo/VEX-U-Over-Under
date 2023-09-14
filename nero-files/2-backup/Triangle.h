#pragma once

class Triangle
{
public:
  int points;
  Node *rightAnglePoint;
  Node *sidePointA;
  Node *sidePointB;

  Triangle(Node *rightAnglePoint, Node *sidePointA, Node *sidePointB)
  {
    this->points = 3;
    this->rightAnglePoint = rightAnglePoint;
    this->sidePointA = sidePointA;
    this->sidePointB = sidePointB;
  }
};