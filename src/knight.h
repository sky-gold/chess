#ifndef CHESS_KNIGHT_H
#define CHESS_KNIGHT_H

#include "types.h"
#include "jumping_figure.h"
#include "misc.h"

namespace knight_logic {
    extern Bitboard move_mask[64];
    extern Magic<std::vector<Move>> move_list[64];

    extern std::vector<std::pair<int, int>> delta;

    void move_mask_precalc();

    void move_list_precalc();

    Bitboard get_moves_bitboard(Square pos, Bitboard turn_bitboard, Bitboard opposite_bitboard);

    const std::vector<Move> &get_moves(Square pos, Bitboard turn_bitboard, Bitboard opposite_bitboard);

    void init();
}

#include "knight.cpp"

#endif //CHESS_KNIGHT_H
