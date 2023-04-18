#include "positional_evaluate.h"

#include "misc.h"

#include <vector>

namespace positional_evaluate {
    float midgame_value[64][MAX_FIGURE];
    float endgame_value[64][MAX_FIGURE];

    /* values from https://www.chessprogramming.org/PeSTO%27s_Evaluation_Function */

    void midgame_init() {
        std::vector<float> pawn = {
            0, 0, 0, 0, 0, 0, 0, 0,
            98, 134, 61, 95, 68, 126, 34, -11,
            -6, 7, 26, 31, 65, 56, 25, -20,
            -14, 13, 6, 21, 23, 12, 17, -23,
            -27, -2, -5, 12, 17, 6, 10, -25,
            -26, -4, -4, -10, 3, 3, 33, -12,
            -35, -1, -20, -23, -15, 24, 38, -22,
            0, 0, 0, 0, 0, 0, 0, 0
        };
        for (Square sq = 0; sq < 64; ++sq) {
            midgame_value[sq][PAWN | WHITE] = pawn[63 - sq] / 100;
            midgame_value[sq][PAWN | BLACK] = pawn[sq] / 100;
        }
        std::vector<float> knight = {
            -167, -89, -34, -49,  61, -97, -15, -107,
            -73, -41,  72,  36,  23,  62,   7,  -17,
            -47,  60,  37,  65,  84, 129,  73,   44,
            -9,  17,  19,  53,  37,  69,  18,   22,
            -13,   4,  16,  13,  28,  19,  21,   -8,
            -23,  -9,  12,  10,  19,  17,  25,  -16,
            -29, -53, -12,  -3,  -1,  18, -14,  -19,
            -105, -21, -58, -33, -17, -28, -19,  -23,
        };
        for (Square sq = 0; sq < 64; ++sq) {
            midgame_value[sq][KNIGHT | WHITE] = knight[63 - sq] / 100;
            midgame_value[sq][KNIGHT | BLACK] = knight[sq] / 100;
        }
        std::vector<float> bishop = {
            -29,   4, -82, -37, -25, -42,   7,  -8,
            -26,  16, -18, -13,  30,  59,  18, -47,
            -16,  37,  43,  40,  35,  50,  37,  -2,
            -4,   5,  19,  50,  37,  37,   7,  -2,
            -6,  13,  13,  26,  34,  12,  10,   4,
            0,  15,  15,  15,  14,  27,  18,  10,
            4,  15,  16,   0,   7,  21,  33,   1,
            -33,  -3, -14, -21, -13, -12, -39, -21
        };
        for (Square sq = 0; sq < 64; ++sq) {
            midgame_value[sq][BISHOP | WHITE] = bishop[63 - sq] / 100;
            midgame_value[sq][BISHOP | BLACK] = bishop[sq] / 100;
        }
        std::vector<float> rook = {
            32,  42,  32,  51, 63,  9,  31,  43,
            27,  32,  58,  62, 80, 67,  26,  44,
            -5,  19,  26,  36, 17, 45,  61,  16,
            -24, -11,   7,  26, 24, 35,  -8, -20,
            -36, -26, -12,  -1,  9, -7,   6, -23,
            -45, -25, -16, -17,  3,  0,  -5, -33,
            -44, -16, -20,  -9, -1, 11,  -6, -71,
            -19, -13,   1,  17, 16,  7, -37, -26,
        };
        for (Square sq = 0; sq < 64; ++sq) {
            midgame_value[sq][ROOK | WHITE] = rook[63 - sq] / 100;
            midgame_value[sq][ROOK | BLACK] = rook[sq] / 100;
        }
        std::vector<float> queen = {
            -28,   0,  29,  12,  59,  44,  43,  45,
            -24, -39,  -5,   1, -16,  57,  28,  54,
            -13, -17,   7,   8,  29,  56,  47,  57,
            -27, -27, -16, -16,  -1,  17,  -2,   1,
            -9, -26,  -9, -10,  -2,  -4,   3,  -3,
            -14,   2, -11,  -2,  -5,   2,  14,   5,
            -35,  -8,  11,   2,   8,  15,  -3,   1,
            -1, -18,  -9,  10, -15, -25, -31, -50,
        };
        for (Square sq = 0; sq < 64; ++sq) {
            midgame_value[sq][QUEEN | WHITE] = queen[63 - sq] / 100;
            midgame_value[sq][QUEEN | BLACK] = queen[sq] / 100;
        }

        std::vector<float> king = {
                -65,  23,  16, -15, -56, -34,   2,  13,
                29,  -1, -20,  -7,  -8,  -4, -38, -29,
                -9,  24,   2, -16, -20,   6,  22, -22,
                -17, -20, -12, -27, -30, -25, -14, -36,
                -49,  -1, -27, -39, -46, -44, -33, -51,
                -14, -14, -22, -46, -44, -30, -15, -27,
                1,   7,  -8, -64, -43, -16,   9,   8,
                -15,  36,  12, -54,   8, -28,  24,  14,
        };
        for (Square sq = 0; sq < 64; ++sq) {
            midgame_value[sq][KING | WHITE] = king[63 - sq] / 100;
            midgame_value[sq][KING | BLACK] = king[sq] / 100;
        }
    }

    void endgame_init() {
        std::vector<float> pawn = {
                0,   0,   0,   0,   0,   0,   0,   0,
                178, 173, 158, 134, 147, 132, 165, 187,
                94, 100,  85,  67,  56,  53,  82,  84,
                32,  24,  13,   5,  -2,   4,  17,  17,
                13,   9,  -3,  -7,  -7,  -8,   3,  -1,
                4,   7,  -6,   1,   0,  -5,  -1,  -8,
                13,   8,   8,  10,  13,   0,   2,  -7,
                0,   0,   0,   0,   0,   0,   0,   0,
        };
        for (Square sq = 0; sq < 64; ++sq) {
            endgame_value[sq][PAWN | WHITE] = pawn[63 - sq] / 100;
            endgame_value[sq][PAWN | BLACK] = pawn[sq] / 100;
        }
        std::vector<float> knight = {
                -14, -21, -11,  -8, -7,  -9, -17, -24,
                -8,  -4,   7, -12, -3, -13,  -4, -14,
                2,  -8,   0,  -1, -2,   6,   0,   4,
                -3,   9,  12,   9, 14,  10,   3,   2,
                -6,   3,  13,  19,  7,  10,  -3,  -9,
                -12,  -3,   8,  10, 13,   3,  -7, -15,
                -14, -18,  -7,  -1,  4,  -9, -15, -27,
                -23,  -9, -23,  -5, -9, -16,  -5, -17,
        };
        for (Square sq = 0; sq < 64; ++sq) {
            endgame_value[sq][KNIGHT | WHITE] = knight[63 - sq] / 100;
            endgame_value[sq][KNIGHT | BLACK] = knight[sq] / 100;
        }
        std::vector<float> bishop = {
                -14, -21, -11,  -8, -7,  -9, -17, -24,
                -8,  -4,   7, -12, -3, -13,  -4, -14,
                2,  -8,   0,  -1, -2,   6,   0,   4,
                -3,   9,  12,   9, 14,  10,   3,   2,
                -6,   3,  13,  19,  7,  10,  -3,  -9,
                -12,  -3,   8,  10, 13,   3,  -7, -15,
                -14, -18,  -7,  -1,  4,  -9, -15, -27,
                -23,  -9, -23,  -5, -9, -16,  -5, -17,
        };
        for (Square sq = 0; sq < 64; ++sq) {
            endgame_value[sq][BISHOP | WHITE] = bishop[63 - sq] / 100;
            endgame_value[sq][BISHOP | BLACK] = bishop[sq] / 100;
        }
        std::vector<float> rook = {
            13, 10, 18, 15, 12,  12,   8,   5,
            11, 13, 13, 11, -3,   3,   8,   3,
            7,  7,  7,  5,  4,  -3,  -5,  -3,
            4,  3, 13,  1,  2,   1,  -1,   2,
            3,  5,  8,  4, -5,  -6,  -8, -11,
            -4,  0, -5, -1, -7, -12,  -8, -16,
            -6, -6,  0,  2, -9,  -9, -11,  -3,
            -9,  2,  3, -1, -5, -13,   4, -20,
        };
        for (Square sq = 0; sq < 64; ++sq) {
            endgame_value[sq][ROOK | WHITE] = rook[63 - sq] / 100;
            endgame_value[sq][ROOK | BLACK] = rook[sq] / 100;
        }
        std::vector<float> queen = {
            -9,  22,  22,  27,  27,  19,  10,  20,
            -17,  20,  32,  41,  58,  25,  30,   0,
            -20,   6,   9,  49,  47,  35,  19,   9,
            3,  22,  24,  45,  57,  40,  57,  36,
            -18,  28,  19,  47,  31,  34,  39,  23,
            -16, -27,  15,   6,   9,  17,  10,   5,
            -22, -23, -30, -16, -16, -23, -36, -32,
            -33, -28, -22, -43,  -5, -32, -20, -41,
        };
        for (Square sq = 0; sq < 64; ++sq) {
            endgame_value[sq][QUEEN | WHITE] = queen[63 - sq] / 100;
            endgame_value[sq][QUEEN | BLACK] = queen[sq] / 100;
        }

        std::vector<float> king = {
            -74, -35, -18, -18, -11,  15,   4, -17,
            -12,  17,  14,  17,  17,  38,  23,  11,
            10,  17,  23,  15,  20,  45,  44,  13,
            -8,  22,  24,  27,  26,  33,  26,   3,
            -18,  -4,  21,  24,  27,  23,   9, -11,
            -19,  -3,  11,  21,  23,  16,   7,  -9,
            -27, -11,   4,  13,  14,   4,  -5, -17,
            -53, -34, -21, -11, -28, -14, -24, -43
        };

        for (Square sq = 0; sq < 64; ++sq) {
            endgame_value[sq][KING | WHITE] = 0;//king[63 - sq] / 100;
            endgame_value[sq][KING | BLACK] = 0;//king[sq] / 100;
        }
    }

    void init() {
        midgame_init();
        endgame_init();
    }

    float evaluate(const Board & board) {
        float mg = 0;
        float eg = 0;
        for (Square sq = 0; sq < 64; ++sq) {
            Figure figure = board.get_figure(sq);
            if (figure != FIGURE_NULL) {
                if ((figure & 1) == board.turn()) {
                    mg += midgame_value[sq][figure];
                    eg += endgame_value[sq][figure];
                } else {
                    mg -= midgame_value[sq][figure];
                    eg -= endgame_value[sq][figure];
                }
            }
        }

        float pawn_value = 0;

        Bitboard turn_pawn = board.figure_bitboard(PAWN | WHITE);
        Bitboard opposite_pawn = board.figure_bitboard(PAWN | BLACK);

        pawn_value += -0.13f * (float)(popcount(turn_pawn & (turn_pawn >> 8)) - popcount(opposite_pawn & (opposite_pawn >> 8)));
        pawn_value += 0.08f * (float)(popcount(turn_pawn & ((turn_pawn & (~COL_H)) << 9)) - popcount(opposite_pawn & ((opposite_pawn & (~COL_H)) >> 7)));
        pawn_value += 0.08f * (float)(popcount(turn_pawn & ((turn_pawn & (~COL_A)) << 7)) - popcount(opposite_pawn & ((opposite_pawn & (~COL_A)) >> 9)));

        uint8_t non_pawn_white = popcount(board.color_bitboard(WHITE) ^ board.get_figure(PAWN | WHITE));
        uint8_t non_pawn_black = popcount(board.color_bitboard(BLACK) ^ board.get_figure(PAWN | BLACK));
        float eg_factor = std::min(1.0f, 2.0f / (float)std::min(non_pawn_black, non_pawn_white));
        return mg * (1 - eg_factor) + eg * eg_factor + pawn_value;
    }
}
