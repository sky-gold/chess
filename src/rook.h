#ifndef CHESS_ROOK_H
#define CHESS_ROOK_H

#include <algorithm>

#include "types.h"
#include "magic_bitboard.h"
#include "sliding_figure.h"
#include "misc.h"

namespace rook_logic {
    extern Magic<Bitboard> move_mask[64];
    extern Magic<std::vector<Move>> move_list[64];

    extern std::vector<std::pair<int, int>> delta;

    void move_mask_precalc();

    void move_list_precalc();

    Bitboard get_moves_bitboard(Square pos, Bitboard turn_bitboard, Bitboard opposite_bitboard);

    const std::vector<Move> &get_moves(Square pos, Bitboard turn_bitboard, Bitboard opposite_bitboard);

    void init();
}

#include "rook.cpp"

#endif //CHESS_ROOK_H
