#include "Missionaries.h"

MissionariesNode::MissionariesNode(int mLeft, int cLeft, int boatLeft, int g)
    : mLeft_(mLeft), cLeft_(cLeft), boatLeft_(boatLeft), g_(g)
{
    key_[3] = '\0';
}

int MissionariesNode::getG() const { return g_; }

int MissionariesNode::getH() const {
    // Lower bound: ceil(people_remaining / boat_capacity).
    // Add 1 if boat is already on the right and people still need to cross
    // (a return trip is required before any forward progress).
    int people = mLeft_ + cLeft_;
    if (people == 0) return 0;
    int base = (people + 1) / 2;
    return base + (boatLeft_ == 0 ? 1 : 0);
}

bool MissionariesNode::isValid() const {
    if (mLeft_ < 0 || mLeft_ > 3 || cLeft_ < 0 || cLeft_ > 3) return false;
    int mRight = 3 - mLeft_, cRight = 3 - cLeft_;
    // 0 missionaries on a side is safe; otherwise must not be outnumbered.
    if (mLeft_  > 0 && mLeft_  < cLeft_)  return false;
    if (mRight  > 0 && mRight  < cRight)  return false;
    return true;
}

INode** MissionariesNode::getNeighbors(int& count) const {
    // All ways to put 1 or 2 people in the boat.
    const int moves[5][2] = { {1,0},{2,0},{0,1},{0,2},{1,1} };

    MissionariesNode* tmp[5];
    count = 0;

    for (int i = 0; i < 5; ++i) {
        int dm = moves[i][0], dc = moves[i][1];
        int nm, nc, nb;
        if (boatLeft_ == 1) {
            nm = mLeft_ - dm; nc = cLeft_ - dc; nb = 0; // cross to right
        } else {
            nm = mLeft_ + dm; nc = cLeft_ + dc; nb = 1; // return to left
        }
        MissionariesNode* next = new MissionariesNode(nm, nc, nb, g_ + 1);
        if (next->isValid()) {
            tmp[count++] = next;
        } else {
            delete next;
        }
    }

    INode** result = new INode*[count];
    for (int i = 0; i < count; ++i) result[i] = tmp[i];
    return result;
}

const char* MissionariesNode::getKey() const {
    key_[0] = (char)('0' + mLeft_);
    key_[1] = (char)('0' + cLeft_);
    key_[2] = (char)('0' + boatLeft_);
    return key_;
}
