#include <iostream>
#include "astar/AStar.h"
#include "puzzle8/Puzzle8.h"
#include "missionaries/Missionaries.h"
#include "queens8/Queens8.h"
#include "puzzle15/Puzzle15.h"
#include "klotski/Klotski.h"

// ---- 8-puzzle helpers ----

static void printPuzzleState(const int* s) {
    for (int i = 0; i < 9; ++i) {
        if (s[i] == 0) std::cout << '_';
        else           std::cout << s[i];
        std::cout << (i % 3 == 2 ? '\n' : ' ');
    }
}

static void runPuzzle() {
    std::cout << "=== 8-Puzzle ===\n\n";

    int start[9] = { 4, 1, 2,
                     0, 6, 3,
                     7, 5, 8 };

    AStarPath path = astarSolve(new Puzzle8Node(start));

    if (path.length == 0) {
        std::cout << "No solution found.\n";
        return;
    }

    std::cout << "Solved in " << path.length - 1 << " move(s):\n\n";
    for (int i = 0; i < path.length; ++i) {
        std::cout << "Step " << i << ":\n";
        printPuzzleState(static_cast<Puzzle8Node*>(path.nodes[i])->getState());
        std::cout << '\n';
    }

    astarFree(path);
}

// ---- Missionaries helpers ----

static void printBank(int m, int c) {
    for (int i = 0; i < m; ++i) std::cout << 'M';
    for (int i = 0; i < c; ++i) std::cout << 'C';
    if (m + c == 0) std::cout << "    ";
}

static void printMissionariesState(const MissionariesNode* n) {
    int mL = n->getMLeft(),  cL = n->getCLeft();
    int mR = 3 - mL,         cR = 3 - cL;

    printBank(mL, cL);
    if (n->getBoatLeft()) std::cout << " [B]|~~~~|     ";
    else                  std::cout << "     |~~~~|[B] ";
    printBank(mR, cR);
    std::cout << '\n';
}

static void runMissionaries() {
    std::cout << "=== Missionaries & Cannibals ===\n\n";
    std::cout << "M = missionary, C = cannibal, [B] = boat\n\n";

    // Start: all 3M + 3C on left bank, boat on left.
    // Goal:  all on right bank (mLeft=0, cLeft=0, boatLeft=0).
    AStarPath path = astarSolve(new MissionariesNode(3, 3, 1));

    if (path.length == 0) {
        std::cout << "No solution found.\n";
        return;
    }

    std::cout << "Solved in " << path.length - 1 << " move(s):\n\n";
    for (int i = 0; i < path.length; ++i) {
        std::cout << "Step " << i << ": ";
        printMissionariesState(static_cast<MissionariesNode*>(path.nodes[i]));
    }

    astarFree(path);
}

// ---- 8-queens helpers ----

static void printQueensBoard(const Queens8Node* n) {
    const int* cols = n->getCols();
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c)
            std::cout << (cols[r] == c ? "Q " : ". ");
        std::cout << '\n';
    }
}

static void runQueens8() {
    std::cout << "=== 8-Queens ===\n\n";

    int empty[8] = { 0,0,0,0,0,0,0,0 };
    AStarPath path = astarSolve(new Queens8Node(empty, 0));

    if (path.length == 0) {
        std::cout << "No solution found.\n";
        return;
    }

    // Only print the final solved board.
    Queens8Node* solution = static_cast<Queens8Node*>(path.nodes[path.length - 1]);
    std::cout << "Solution (placed " << solution->getPlaced() << " queens):\n\n";
    printQueensBoard(solution);

    astarFree(path);
}

// ---- 15-puzzle helpers ----

static void printPuzzle15State(const int* s) {
    for (int i = 0; i < 16; ++i) {
        if      (s[i] == 0)  std::cout << "  _";
        else if (s[i] < 10)  std::cout << "  " << s[i];
        else                 std::cout << " "  << s[i];
        if (i % 4 == 3) std::cout << '\n';
    }
}

static void runPuzzle15() {
    std::cout << "=== 15-Puzzle ===\n\n";

    // Derived by reversing 10 moves from the goal — guaranteed solvable.
    int start[16] = {  1,  6,  2,  4,
                       9,  0,  3,  8,
                      10,  5,  7, 12,
                      13, 14, 11, 15 };

    AStarPath path = astarSolve(new Puzzle15Node(start));

    if (path.length == 0) {
        std::cout << "No solution found.\n";
        return;
    }

    std::cout << "Solved in " << path.length - 1 << " move(s):\n\n";
    for (int i = 0; i < path.length; ++i) {
        std::cout << "Step " << i << ":\n";
        printPuzzle15State(static_cast<Puzzle15Node*>(path.nodes[i])->getState());
        std::cout << '\n';
    }

    astarFree(path);
}

// ---- Klotski helpers ----

static void printKlotskiBoard(const KlotskiNode* n) {
    int board[5][4];
    n->getBoard(board);
    const KPiece* pieces = n->getPieces();
    std::cout << "+--------+\n";
    for (int r = 0; r < 5; ++r) {
        std::cout << "| ";
        for (int c = 0; c < 4; ++c) {
            int id = board[r][c];
            char ch;
            if      (id == 0) ch = '.';
            else if (id == 1) ch = '#';                              // 2x2 goal
            else if (pieces[id-1].type == PT_1x1) ch = 's';         // soldier
            else if (pieces[id-1].type == PT_1x2) ch = 'D';         // Guan Yu
            else              ch = (char)('A' + id - 2);            // generals
            std::cout << ch << ' ';
        }
        std::cout << "|\n";
    }
    std::cout << "+--------+\n";
}

static void runKlotski() {
    std::cout << "=== Klotski (Huarong Pass) ===\n\n";
    std::cout << "# = 2x2 Cao Cao (goal: slide to rows 3-4, cols 1-2)\n";
    std::cout << "A/B = generals (2x1)  D = Guan Yu (1x2)  s = soldiers (1x1)\n\n";

    // Standard Huarong Pass (10 pieces, 2 empty at row 4 cols 1-2):
    //   A # # B
    //   A # # B
    //   C D D E
    //   C s s E
    //   s . . s
    KPiece pieces[10];
    pieces[0].type = PT_2x2; pieces[0].row = 0; pieces[0].col = 1; // Cao Cao (goal)
    pieces[1].type = PT_2x1; pieces[1].row = 0; pieces[1].col = 0; // Zhang Fei
    pieces[2].type = PT_2x1; pieces[2].row = 0; pieces[2].col = 3; // Ma Chao
    pieces[3].type = PT_2x1; pieces[3].row = 2; pieces[3].col = 0; // Zhao Yun
    pieces[4].type = PT_1x2; pieces[4].row = 2; pieces[4].col = 1; // Guan Yu
    pieces[5].type = PT_2x1; pieces[5].row = 2; pieces[5].col = 3; // Huang Zhong
    pieces[6].type = PT_1x1; pieces[6].row = 3; pieces[6].col = 1; // soldier
    pieces[7].type = PT_1x1; pieces[7].row = 3; pieces[7].col = 2; // soldier
    pieces[8].type = PT_1x1; pieces[8].row = 4; pieces[8].col = 0; // soldier
    pieces[9].type = PT_1x1; pieces[9].row = 4; pieces[9].col = 3; // soldier

    AStarPath path = astarSolve(new KlotskiNode(pieces, 10));

    if (path.length == 0) {
        std::cout << "No solution found.\n";
        return;
    }

    std::cout << "Solved in " << path.length - 1 << " move(s):\n\n";
    for (int i = 0; i < path.length; ++i) {
        std::cout << "Step " << i << ":\n";
        printKlotskiBoard(static_cast<KlotskiNode*>(path.nodes[i]));
        std::cout << '\n';
    }

    astarFree(path);
}

// ---- Main ----

int main() {
//    runPuzzle();
//    std::cout << '\n';
//    runMissionaries();
    //std::cout << '\n';
    //runQueens8();
    //std::cout << '\n';
    //runPuzzle15();
    //std::cout << '\n';
    runKlotski();
    return 0;
}
