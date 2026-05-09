#pragma once
#include "INode.h"

struct AStarPath {
    INode** nodes;   // path from start to goal (inclusive)
    int     length;  // number of nodes; 0 = unsolvable
};

// Takes ownership of start. All nodes are freed by astarFree().
AStarPath astarSolve(INode* start);
void      astarFree(AStarPath& path);
