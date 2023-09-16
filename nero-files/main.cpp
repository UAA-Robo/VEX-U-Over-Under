#include <map>
#include <vector>
#include <cmath>
#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include <stdexcept>

#include "Node.h"
#include "Graph.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Button.h"
#include "GUI.h"

// in inches
double const FIELD_SIZE = 138.73;
// length and width of nodes in inches, for real-life movement
double const NODE_SIZE = 1;
// length and width of nodes in pixels, for visualization
int const CELL_SIZE = 6;
// taking advantage of the map's symmetry. Not exact, but close to real values. Used for computing where to place obstacles
double const ZONE_SIZE = FIELD_SIZE / 6;
// number of nodes per row and column
int const X_NODES = FIELD_SIZE / NODE_SIZE;
int const Y_NODES = FIELD_SIZE / NODE_SIZE;

int main(int argv, char **args)
{
  Graph *graph = new Graph(X_NODES, Y_NODES);

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

  std::vector<Triangle> forbiddenZonesTriangles;
  std::vector<Rectangle> forbiddenZonesRectangles;

  forbiddenZonesTriangles.push_back(Triangle(graph->getNode(0, 0),
                                             graph->getNode(0, ROLLER_SIZE),
                                             graph->getNode(ROLLER_SIZE, 0)));
  forbiddenZonesTriangles.push_back(Triangle(graph->getNode(FIELD_SIZE - 1, 0),
                                             graph->getNode(FIELD_SIZE - 1, ROLLER_SIZE),
                                             graph->getNode(FIELD_SIZE - 1 - ROLLER_SIZE, 0)));
  forbiddenZonesTriangles.push_back(Triangle(graph->getNode(0, FIELD_SIZE - 1),
                                             graph->getNode(0, FIELD_SIZE - 1 - ROLLER_SIZE),
                                             graph->getNode(ROLLER_SIZE, FIELD_SIZE - 1)));
  forbiddenZonesTriangles.push_back(Triangle(graph->getNode(FIELD_SIZE - 1, FIELD_SIZE - 1),
                                             graph->getNode(FIELD_SIZE - 1, FIELD_SIZE - 1 - ROLLER_SIZE),
                                             graph->getNode(FIELD_SIZE - 1 - ROLLER_SIZE, FIELD_SIZE - 1)));

  forbiddenZonesRectangles.push_back(Rectangle(graph->getNode(GOAL_START_X, GOAL_START_Y),
                                               graph->getNode(GOAL_START_X + GOAL_SIZE_X, GOAL_START_Y),
                                               graph->getNode(GOAL_START_X, GOAL_START_Y + GOAL_SIZE_Y),
                                               graph->getNode(GOAL_START_X + GOAL_SIZE_X, GOAL_START_Y + GOAL_SIZE_Y)));
  forbiddenZonesRectangles.push_back(Rectangle(graph->getNode(GOAL_START_X, FIELD_SIZE - 1 - GOAL_SIZE_Y),
                                               graph->getNode(GOAL_START_X + GOAL_SIZE_X, FIELD_SIZE - 1 - GOAL_SIZE_Y),
                                               graph->getNode(GOAL_START_X, FIELD_SIZE - 1),
                                               graph->getNode(GOAL_START_X + GOAL_SIZE_X, FIELD_SIZE - 1)));
  forbiddenZonesRectangles.push_back(Rectangle(graph->getNode(BAR_LEFT_START_X, BAR_LEFT_START_Y),
                                               graph->getNode(BAR_LEFT_START_X + BAR_LEFT_SIZE_X, BAR_LEFT_START_Y),
                                               graph->getNode(BAR_LEFT_START_X, BAR_LEFT_START_Y + BAR_LEFT_SIZE_Y),
                                               graph->getNode(BAR_LEFT_START_X + BAR_LEFT_SIZE_X, BAR_LEFT_START_Y + BAR_LEFT_SIZE_Y)));
  forbiddenZonesRectangles.push_back(Rectangle(graph->getNode(BAR_RIGHT_START_X, BAR_RIGHT_START_Y),
                                               graph->getNode(BAR_RIGHT_START_X + BAR_RIGHT_SIZE_X, BAR_RIGHT_START_Y),
                                               graph->getNode(BAR_RIGHT_START_X, BAR_RIGHT_START_Y + BAR_RIGHT_SIZE_Y),
                                               graph->getNode(BAR_RIGHT_START_X + BAR_RIGHT_SIZE_X, BAR_RIGHT_START_Y + BAR_RIGHT_SIZE_Y)));
  forbiddenZonesRectangles.push_back(Rectangle(graph->getNode(BAR_MAIN_START_X, BAR_MAIN_START_Y),
                                               graph->getNode(BAR_MAIN_START_X + BAR_MAIN_SIZE_X, BAR_MAIN_START_Y),
                                               graph->getNode(BAR_MAIN_START_X, BAR_MAIN_START_Y + BAR_MAIN_SIZE_Y),
                                               graph->getNode(BAR_MAIN_START_X + BAR_MAIN_SIZE_X, BAR_MAIN_START_Y + BAR_MAIN_SIZE_Y)));

  for (Triangle triangle : forbiddenZonesTriangles)
  {
    graph->forbidTriangle(triangle.sidePointA, triangle.sidePointB);
  }

  for (Rectangle rectangle : forbiddenZonesRectangles)
  {
    graph->forbidRectangle(rectangle.topLeftPoint, rectangle.topRightPoint, rectangle.bottomLeftPoint);
  }

  GUI gui = GUI(graph, CELL_SIZE);
  gui.run(false);
}
