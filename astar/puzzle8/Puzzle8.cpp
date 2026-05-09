#include "Puzzle8.h"

const int Puzzle8Node::GOAL[9] = { 1,2,3,4,5,6,7,8,0 };

Puzzle8Node::Puzzle8Node(const int state[9], int g) : g_(g) {
    for (int i = 0; i < 9; ++i) state_[i] = state[i];
    key_[9] = '\0';
}

int Puzzle8Node::getG() const { return g_; }
int Puzzle8Node::getH() const { return computeManhattan(); }

int Puzzle8Node::computeManhattan() const {
    int dist = 0;
    for (int i = 0; i < 9; ++i) {
        int tile = state_[i];
        if (tile == 0) continue;
        int goalIdx = tile - 1; // tile 1 belongs at index 0, etc.
        int dr = i / 3 - goalIdx / 3;
        int dc = i % 3 - goalIdx % 3;
        dist += (dr < 0 ? -dr : dr) + (dc < 0 ? -dc : dc);
    }
    return dist;
}

INode** Puzzle8Node::getNeighbors(int& count) const {
    int blank = 0;
    for (int i = 0; i < 9; ++i)
        if (state_[i] == 0) { blank = i; break; }

    int row = blank / 3, col = blank % 3;

    const int dr[] = { -1,  1,  0, 0 };
    const int dc[] = {  0,  0, -1, 1 };

    count = 0;
    for (int d = 0; d < 4; ++d) {
        int nr = row + dr[d], nc = col + dc[d];
        if (nr >= 0 && nr < 3 && nc >= 0 && nc < 3) ++count;
    }

    INode** result = new INode*[count];
    int ri = 0;
    for (int d = 0; d < 4; ++d) {
        int nr = row + dr[d], nc = col + dc[d];
        if (nr < 0 || nr >= 3 || nc < 0 || nc >= 3) continue;
        int next[9];
        for (int i = 0; i < 9; ++i) next[i] = state_[i];
        next[blank]       = next[nr * 3 + nc];
        next[nr * 3 + nc] = 0;
        result[ri++] = new Puzzle8Node(next, g_ + 1);
    }
    return result;
}

const char* Puzzle8Node::getKey() const {
    for (int i = 0; i < 9; ++i)
        key_[i] = (char)('0' + state_[i]);
    return key_;
}
