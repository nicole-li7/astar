#pragma once
#include "../astar/INode.h"

// Missionaries and cannibals river-crossing problem.
// 3 missionaries (M) and 3 cannibals (C) must cross from left to right.
// Boat holds 1-2 people. Missionaries must never be outnumbered on either bank.
class MissionariesNode : public INode {
public:
    MissionariesNode(int mLeft, int cLeft, int boatLeft, int g = 0);

    int getG() const;
    int getH() const;

    INode**     getNeighbors(int& count) const;
    const char* getKey() const;

    int getMLeft()    const { return mLeft_; }
    int getCLeft()    const { return cLeft_; }
    int getBoatLeft() const { return boatLeft_; }

private:
    int          mLeft_;
    int          cLeft_;
    int          boatLeft_; // 1 = boat on left bank, 0 = right bank
    int          g_;
    mutable char key_[4];   // "mcb\0"

    bool isValid() const;
};
