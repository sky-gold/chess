#include "material_evaluate.h"


namespace material_evaluate {
    float figure_material[MAX_FIGURE];

    void init() {
        figure_material[PAWN | WHITE] = 1;
        figure_material[PAWN | BLACK] = -1;
        figure_material[BISHOP | WHITE] = 3;
        figure_material[BISHOP | BLACK] = -3;
        figure_material[KNIGHT | WHITE] = 3;
        figure_material[KNIGHT | BLACK] = -3;
        figure_material[ROOK | WHITE] = 5;
        figure_material[ROOK | BLACK] = -5;
        figure_material[QUEEN | WHITE] = 9;
        figure_material[QUEEN | BLACK] = -9;
    }

    float evaluate(const Board & board) {
        float result = 0;
        uint8_t turn = board.turn();
        result += 1 * (popcount(board.figure_bitboard(PAWN | turn)) - popcount(board.figure_bitboard(PAWN | turn ^ 1)));
        result += 3 * (popcount(board.figure_bitboard(KNIGHT | turn)) - popcount(board.figure_bitboard(KNIGHT | turn ^ 1)));
        result += 3 * (popcount(board.figure_bitboard(BISHOP | turn)) - popcount(board.figure_bitboard(BISHOP | turn ^ 1)));
        result += 5 * (popcount(board.figure_bitboard(ROOK | turn)) - popcount(board.figure_bitboard(ROOK | turn ^ 1)));
        result += 9 * (popcount(board.figure_bitboard(QUEEN | turn)) - popcount(board.figure_bitboard(QUEEN | turn ^ 1)));
        if (popcount(board.figure_bitboard(BISHOP | turn)) >= 2) {
            result += 0.2;
        }
        if (popcount(board.figure_bitboard(BISHOP | turn ^ 1)) >= 2) {
            result -= 0.2;
        }
        return result;
    }
}