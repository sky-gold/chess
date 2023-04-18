#ifndef CHESS_PAWN_H
#define CHESS_PAWN_H


#include <cstdint>
#include <vector>

#include "types.h"
#include "magic_bitboard.h"
#include "misc.h"

namespace pawn_logic {

    extern Bitboard pawn_attack_bitboard[64][2];

    extern Magic<std::vector<Move>> passant_move_list[64];

    void passant_precalc();

    const std::vector<Move> &get_passant(Square passant_flag, Bitboard pawns);

    void calc_attack_mask();

    std::vector<Move> get_moves(Bitboard pawns, Bitboard turn_bitboard, Bitboard opposite_bitboard, uint8_t turn);

    void init();
}

//#include "pawn.cpp"

#endif //CHESS_PAWN_H
