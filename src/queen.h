#ifndef CHESS_QUEEN_H
#define CHESS_QUEEN_H

#include <algorithm>

#include "types.h"
#include "magic_bitboard.h"
#include "sliding_figure.h"
#include "misc.h"

#include "rook.h"
#include "bishop.h"

namespace queen_logic {

    Bitboard get_moves_bitboard(Square pos, Bitboard turn_bitboard, Bitboard opposite_bitboard);

    std::pair<const std::vector<Move> &, const std::vector<Move> &>
    get_moves(Square pos, Bitboard turn_bitboard, Bitboard opposite_bitboard);
}

#include "queen.cpp"

#endif //CHESS_QUEEN_H
