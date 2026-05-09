#include "Queens8.h"

Queens8Node::Queens8Node(const int cols[8], int placed, int g)
    : placed_(placed), g_(g)
{
    for (int i = 0; i < 8; ++i) cols_[i] = cols[i];
    key_[8] = '\0';
}

int Queens8Node::getG() const { return g_; }
int Queens8Node::getH() const { return 8 - placed_; }

// True if a queen at (row, col) would be attacked by any already-placed queen.
bool Queens8Node::attacks(int row, int col) const {
    for (int r = 0; r < placed_; ++r) {
        if (cols_[r] == col) return true;
        int dr = row - r;
        int dc = cols_[r] - col;
        if ((dc < 0 ? -dc : dc) == dr) return true; // same diagonal
    }
    return false;
}

INode** Queens8Node::getNeighbors(int& count) const {
    if (placed_ >= 8) { count = 0; return new INode*[0]; }

    Queens8Node* tmp[8];
    count = 0;

    for (int col = 0; col < 8; ++col) {
        if (!attacks(placed_, col)) {
            int next[8];
            for (int i = 0; i < 8; ++i) next[i] = cols_[i];
            next[placed_] = col;
            tmp[count++] = new Queens8Node(next, placed_ + 1, g_ + 1);
        }
    }

    INode** result = new INode*[count];
    for (int i = 0; i < count; ++i) result[i] = tmp[i];
    return result;
}

const char* Queens8Node::getKey() const {
    for (int i = 0; i < placed_; ++i) key_[i] = (char)('0' + cols_[i]);
    for (int i = placed_; i < 8;    ++i) key_[i] = '.';
    return key_;
}
