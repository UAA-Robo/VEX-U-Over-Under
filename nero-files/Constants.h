#pragma once

// in inches
const double FIELD_SIZE = 138.73;
// length and width of nodes in inches, for real-life movement
// TODO - IMPORTANT - LOS algorithm assumes this is always 1, multiply this by cell size and take half that to get to center of node
const double NODE_SIZE = 1;
// length and width of nodes in pixels, for visualization
const int VISUAL_NODE_SIZE = 6;
// const int VISUAL_NODE_SIZE = 40;
// taking advantage of the map's symmetry. Not exact, but close to real values. Used for computing where to place obstacles
const double ZONE_SIZE = FIELD_SIZE / 6;
// number of nodes per row and column
const int X_NODES_COUNT = FIELD_SIZE / NODE_SIZE;
const int Y_NODES_COUNT = FIELD_SIZE / NODE_SIZE;
// const int X_NODES_COUNT = 10;
// const int Y_NODES_COUNT = 10;
