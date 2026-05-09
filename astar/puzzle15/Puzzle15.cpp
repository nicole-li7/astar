#include "Puzzle15.h"

const int Puzzle15Node::GOAL[16] = { 1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,0 };

Puzzle15Node::Puzzle15Node(const int state[16], int g) : g_(g) {
    for (int i = 0; i < 16; ++i) state_[i] = state[i];
    key_[16] = '\0';
}

int Puzzle15Node::getG() const { return g_; }
int Puzzle15Node::getH() const { return computeManhattan(); }

int Puzzle15Node::computeManhattan() const {
    int dist = 0;
    for (int i = 0; i < 16; ++i) {
        int tile = state_[i];
        if (tile == 0) continue;
        int goalIdx = tile - 1; // tile 1 belongs at index 0, etc.
        int dr = i / 4 - goalIdx / 4;
        int dc = i % 4 - goalIdx % 4;
        dist += (dr < 0 ? -dr : dr) + (dc < 0 ? -dc : dc);
    }
    return dist;
}

INode** Puzzle15Node::getNeighbors(int& count) const {
    int blank = 0;
    for (int i = 0; i < 16; ++i)
        if (state_[i] == 0) { blank = i; break; }

    int row = blank / 4, col = blank % 4;

    const int dr[] = { -1,  1,  0, 0 };
    const int dc[] = {  0,  0, -1, 1 };

    count = 0;
    for (int d = 0; d < 4; ++d) {
        int nr = row + dr[d], nc = col + dc[d];
        if (nr >= 0 && nr < 4 && nc >= 0 && nc < 4) ++count;
    }

    INode** result = new INode*[count];
    int ri = 0;
    for (int d = 0; d < 4; ++d) {
        int nr = row + dr[d], nc = col + dc[d];
        if (nr < 0 || nr >= 4 || nc < 0 || nc >= 4) continue;
        int next[16];
        for (int i = 0; i < 16; ++i) next[i] = state_[i];
        next[blank]       = next[nr * 4 + nc];
        next[nr * 4 + nc] = 0;
        result[ri++] = new Puzzle15Node(next, g_ + 1);
    }
    return result;
}

const char* Puzzle15Node::getKey() const {
    for (int i = 0; i < 16; ++i) {
        int t = state_[i];
        key_[i] = t < 10 ? (char)('0' + t) : (char)('a' + t - 10);
    }
    return key_;
}
