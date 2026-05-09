#pragma once
#include "../astar/INode.h"

// Piece shapes on a 4-wide x 5-tall board.
enum PieceType { PT_2x2 = 0, PT_2x1 = 1, PT_1x2 = 2, PT_1x1 = 3 };

struct KPiece {
    PieceType type;
    int       row, col; // top-left corner
};

#define KLOTSKI_MAX_PIECES 16

// Klotski ("Huarong Pass") sliding-block puzzle.
// pieces[0] MUST be the 2x2 goal piece.
// Goal: pieces[0] reaches row 3, col 1 (bottom-centre exit).
class KlotskiNode : public INode {
public:
    KlotskiNode(const KPiece pieces[], int numPieces, int g = 0);

    int getG() const;
    int getH() const; // Manhattan distance of 2x2 piece to exit

    INode**     getNeighbors(int& count) const;
    const char* getKey() const; // canonical: encodes shape+offset per cell

    // Fills board[row][col] with 1-based piece index (0 = empty).
    void getBoard(int board[5][4]) const;

    int           getNumPieces() const { return numPieces_; }
    const KPiece* getPieces()    const { return pieces_; }

private:
    KPiece       pieces_[KLOTSKI_MAX_PIECES];
    int          numPieces_;
    int          g_;
    mutable char key_[21]; // 4x5 = 20 cells + null

    void renderBoard(int board[5][4]) const;
    bool canMove(int pi, int dr, int dc, const int board[5][4]) const;
};
