#pragma once

class Rectangle
{
public:
  int points;
  Node *topLeftPoint;
  Node *topRightPoint;
  Node *bottomLeftPoint;
  Node *bottomRightPoint;

  Rectangle(Node *topLeftPoint, Node *topRightPoint, Node *bottomLeftPoint, Node *bottomRightPoint)
  {
    this->points = 4;
    this->topLeftPoint = topLeftPoint;
    this->topRightPoint = topRightPoint;
    this->bottomLeftPoint = bottomLeftPoint;
    this->bottomRightPoint = bottomRightPoint;
  }
};
