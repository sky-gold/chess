#ifndef CHESS_MISC_H
#define CHESS_MISC_H

#include <vector>
#include <string>
#include <map>

#include "types.h"

char from_figure_to_char(Figure figure);

Figure from_char_to_figure(char ch);

std::vector<Bitboard> get_submasks(Bitboard mask);

uint8_t lsb(Bitboard bb);

uint8_t pop_lsb(Bitboard &bb);

std::string bitboard_string(Bitboard b);

uint16_t create_move(Square pos1, Square pos2, uint8_t move_flag);

std::string get_string_move(Move move);

uint8_t popcount(Bitboard b);

#include "misc.cpp"

#endif //CHESS_MISC_H
