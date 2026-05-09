#pragma once
#include "../astar/INode.h"

// 8-queens problem: place one queen per row such that no two queens share
// a column or diagonal. We fill the board row by row; each neighbor places
// the next queen in a non-attacking column.
class Queens8Node : public INode {
public:
    Queens8Node(const int cols[8], int placed, int g = 0);

    int getG() const;
    int getH() const; // = 8 - placed (remaining rows)

    INode**     getNeighbors(int& count) const;
    const char* getKey() const;

    int        getPlaced() const { return placed_; }
    const int* getCols()   const { return cols_; }

private:
    int          cols_[8];  // cols_[r] = column of queen in row r
    int          placed_;   // how many rows have been filled (0-8)
    int          g_;
    mutable char key_[9];   // one digit per row + null

    bool attacks(int row, int col) const;
};
