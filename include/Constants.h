#pragma once

// const double FIELD_SIZE = 138.73;
const double FIELD_SIZE = 10;
// length and width of nodes in inches, for real-life movement
// TODO - IMPORTANT - LOS algorithm assumes this is always 1, multiply this by cell size and take half that to get to center of node
const double NODE_SIZE = 1;
// length and width of nodes in pixels, for visualization
const int VISUAL_NODE_SIZE = 6;
const double ZONE_SIZE = FIELD_SIZE / 6;
const int X_NODES_COUNT = FIELD_SIZE / NODE_SIZE;
const int Y_NODES_COUNT = FIELD_SIZE / NODE_SIZE;
const int MAX_INT = 2147483647;
// TODO - when this is 8, error occur because of the taut waypoints rules
const int ROBOT_ZONES_COUNT = 8;
const int BUFFER_ZONES_COUNT = 0;