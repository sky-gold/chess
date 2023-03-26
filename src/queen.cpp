#include "queen.h"

namespace queen_logic {
    Bitboard get_moves_bitboard(Square pos, Bitboard turn_bitboard, Bitboard opposite_bitboard) {
        return rook_logic::get_moves_bitboard(pos, turn_bitboard, opposite_bitboard) |
               bishop_logic::get_moves_bitboard(pos, turn_bitboard, opposite_bitboard);
    }

    std::pair<const std::vector<Move> &, const std::vector<Move> &>
    get_moves(Square pos, Bitboard turn_bitboard, Bitboard opposite_bitboard) {
        return {rook_logic::get_moves(pos, turn_bitboard, opposite_bitboard),
                bishop_logic::get_moves(pos, turn_bitboard, opposite_bitboard)};
    }
}