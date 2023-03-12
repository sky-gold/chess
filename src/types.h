#ifndef CHESS_TYPES_H
#define CHESS_TYPES_H

#include <cstdint>

using Move = uint16_t; // 0-5 = from 6-11 = to 12-15 = move flag

const Move MOVE_NULL = 0;

using Figure = uint8_t; // 0 = COL_or 1-3 = name

const uint8_t WHITE = 0;
const uint8_t BLACK = 1;

const Figure FIGURE_NULL = 0;
const Figure KING = (1 << 1);
const Figure QUEEN = (2 << 1);
const Figure BISHOP = (3 << 1);
const Figure KNIGHT = (4 << 1);
const Figure ROOK = (5 << 1);
const Figure PAWN = (6 << 1);

const size_t MAX_FIGURE = 14;

using Square = uint8_t;

const Square SQUARE_NULL = 64;

using Bitboard = uint64_t;

const Bitboard ROW_1 = ((1 << 8) - 1);
const Bitboard ROW_2 = ROW_1 << 8;
const Bitboard ROW_3 = ROW_2 << 8;
const Bitboard ROW_4 = ROW_3 << 8;
const Bitboard ROW_5 = ROW_4 << 8;
const Bitboard ROW_6 = ROW_5 << 8;
const Bitboard ROW_7 = ROW_6 << 8;
const Bitboard ROW_8 = ROW_7 << 8;

const Bitboard COL_A = 0x0101010101010101;
const Bitboard COL_B = COL_A >> 1;
const Bitboard COL_C = COL_B >> 1;
const Bitboard COL_D = COL_C >> 1;
const Bitboard COL_E = COL_D >> 1;
const Bitboard COL_F = COL_E >> 1;
const Bitboard COL_G = COL_F >> 1;
const Bitboard COL_H = COL_G >> 1;

using Hash = uint64_t;


#endif //CHESS_TYPES_H
