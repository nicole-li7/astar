#pragma once
#include "../astar/INode.h"

class Puzzle15Node : public INode {
public:
    static const int GOAL[16]; // {1,2,...,15,0}

    Puzzle15Node(const int state[16], int g = 0);

    int getG() const;
    int getH() const; // Manhattan distance

    INode**     getNeighbors(int& count) const;
    const char* getKey() const;

    const int* getState() const { return state_; }

private:
    int          state_[16];
    int          g_;
    mutable char key_[17]; // one hex char per tile + null

    int computeManhattan() const;
};
