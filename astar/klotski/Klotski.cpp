#include "Klotski.h"

KlotskiNode::KlotskiNode(const KPiece pieces[], int numPieces, int g)
    : numPieces_(numPieces), g_(g)
{
    for (int i = 0; i < numPieces; ++i) pieces_[i] = pieces[i];
    key_[20] = '\0';
}

int KlotskiNode::getG() const { return g_; }

int KlotskiNode::getH() const {
    // pieces_[0] is the 2x2 goal piece; exit is at (row=3, col=1).
    int dr = pieces_[0].row - 3;
    int dc = pieces_[0].col - 1;
    return (dr < 0 ? -dr : dr) + (dc < 0 ? -dc : dc);
}

void KlotskiNode::renderBoard(int board[5][4]) const {
    for (int r = 0; r < 5; ++r)
        for (int c = 0; c < 4; ++c)
            board[r][c] = 0;

    for (int i = 0; i < numPieces_; ++i) {
        const KPiece& p = pieces_[i];
        int id = i + 1;
        switch (p.type) {
            case PT_2x2:
                board[p.row    ][p.col    ] = id;
                board[p.row    ][p.col + 1] = id;
                board[p.row + 1][p.col    ] = id;
                board[p.row + 1][p.col + 1] = id;
                break;
            case PT_2x1:
                board[p.row    ][p.col] = id;
                board[p.row + 1][p.col] = id;
                break;
            case PT_1x2:
                board[p.row][p.col    ] = id;
                board[p.row][p.col + 1] = id;
                break;
            case PT_1x1:
                board[p.row][p.col] = id;
                break;
        }
    }
}

void KlotskiNode::getBoard(int board[5][4]) const {
    renderBoard(board);
}

bool KlotskiNode::canMove(int pi, int dr, int dc, const int board[5][4]) const {
    const KPiece& p = pieces_[pi];
    int nr = p.row + dr, nc = p.col + dc;
    int rows, cols;
    switch (p.type) {
        case PT_2x2: rows = 2; cols = 2; break;
        case PT_2x1: rows = 2; cols = 1; break;
        case PT_1x2: rows = 1; cols = 2; break;
        default:     rows = 1; cols = 1; break;
    }
    if (nr < 0 || nr + rows > 5 || nc < 0 || nc + cols > 4) return false;
    int id = pi + 1;
    for (int r = nr; r < nr + rows; ++r)
        for (int c = nc; c < nc + cols; ++c)
            if (board[r][c] != 0 && board[r][c] != id) return false;
    return true;
}

INode** KlotskiNode::getNeighbors(int& count) const {
    int board[5][4];
    renderBoard(board);

    const int dr[] = { -1, 1,  0, 0 };
    const int dc[] = {  0, 0, -1, 1 };

    KlotskiNode* tmp[KLOTSKI_MAX_PIECES * 4];
    count = 0;

    for (int pi = 0; pi < numPieces_; ++pi) {
        for (int d = 0; d < 4; ++d) {
            if (canMove(pi, dr[d], dc[d], board)) {
                KPiece next[KLOTSKI_MAX_PIECES];
                for (int i = 0; i < numPieces_; ++i) next[i] = pieces_[i];
                next[pi].row += dr[d];
                next[pi].col += dc[d];
                tmp[count++] = new KlotskiNode(next, numPieces_, g_ + 1);
            }
        }
    }

    INode** result = new INode*[count];
    for (int i = 0; i < count; ++i) result[i] = tmp[i];
    return result;
}

const char* KlotskiNode::getKey() const {
    // Canonical encoding: each cell is identified by piece shape + offset
    // within that shape, so visually identical boards always produce the same key.
    //   '0'       = empty
    //   '1'-'4'   = 2x2 cells (top-left, top-right, bottom-left, bottom-right)
    //   'a'/'b'   = top/bottom of a 2x1 vertical
    //   'c'/'d'   = left/right of a 1x2 horizontal
    //   'e'       = 1x1
    int board[5][4];
    renderBoard(board);

    int k = 0;
    for (int r = 0; r < 5; ++r) {
        for (int c = 0; c < 4; ++c) {
            int id = board[r][c];
            if (id == 0) {
                key_[k++] = '0';
            } else {
                const KPiece& p = pieces_[id - 1];
                int dr = r - p.row, dc = c - p.col;
                switch (p.type) {
                    case PT_2x2: key_[k++] = (char)('1' + dr * 2 + dc); break;
                    case PT_2x1: key_[k++] = (char)('a' + dr);          break;
                    case PT_1x2: key_[k++] = (char)('c' + dc);          break;
                    default:     key_[k++] = 'e';                        break;
                }
            }
        }
    }
    key_[20] = '\0';
    return key_;
}
