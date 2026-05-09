#pragma once
#include "../astar/INode.h"

class Puzzle8Node : public INode {
public:
    static const int GOAL[9]; // {1,2,3,4,5,6,7,8,0}

    Puzzle8Node(const int state[9], int g = 0);

    int getG() const;
    int getH() const;

    INode**     getNeighbors(int& count) const;
    const char* getKey() const;

    const int* getState() const { return state_; }

private:
    int          state_[9];
    int          g_;
    mutable char key_[10]; // 9 digits + null

    int computeManhattan() const;
};
