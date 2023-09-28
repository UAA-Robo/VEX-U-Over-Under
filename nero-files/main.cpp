// No premature optimizations

// TODO - make sure all pathfindings are updated to the latest iteration
// TODO - update naming convention and style guide
// TODO - visualize the slope to make sure it's correct (positive, negative, both slopes)
// TODO - IMPORTANT - remove hard coded values to remove assumptions
// TODO - store edge costs to save computation time?
// TODO - occasional crashing on initial bootup (VG, non-snapshot)
// TODO - declare algorithms in one location and import them elsewhere, so you don't have to debug and update fixes from one version to others
// TODO - pass data structures around using pointers
// TODO - user smart pointers

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Think in terms of:
// * Data
// * Calculations
// * Actions
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// TODO - IMPORTANT - factor out shared code in conditional code blocks
// TODO - benchmark creation and pathfinding of grid for both grid types

#include "Enums.h"
#include "Node.h"
#include "Graph.h"
#include "GUI.h"
#include <iostream>
#include "Constants.h"

int main(int argv, char **args)
{
  Graph *graph = new Graph(X_NODES_COUNT, Y_NODES_COUNT, VISUAL_NODE_SIZE);
  GUI gui = GUI(graph, VISUAL_NODE_SIZE);
}
