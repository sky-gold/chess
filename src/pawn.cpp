#include "pawn.h"

namespace pawn_logic {

    Bitboard pawn_attack_bitboard[64][2];

    Magic<std::vector<Move>> passant_move_list[64];

    void passant_precalc() {
        for (Square pos = 40; pos < 48; ++pos) {
            Bitboard mask = 0;
            if ((pos % 8) != 7) {
                mask |= (1ULL << (pos - 7));
            }
            if ((pos % 8) != 0) {
                mask |= (1ULL << (pos - 9));
            }
            passant_move_list[pos] = get_magic<std::vector<Move>>(mask, 2);
            for (Bitboard pawns: get_submasks(mask)) {

                std::vector<Move> result;
                if ((pos % 8) != 7 && ((pawns >> (pos - 7)) & 1)) {
                    result.push_back(create_move(pos - 7, pos, 6));
                }
                if ((pos % 8) != 0 && ((pawns >> (pos - 9)) & 1)) {
                    result.push_back(create_move(pos - 9, pos, 6));
                }
                passant_move_list[pos][pawns] = result;
            }
        }
        for (Square pos = 16; pos < 24; ++pos) {
            Bitboard mask = 0;
            if ((pos % 8) != 7) {
                mask |= (1ULL << (pos + 9));
            }
            if ((pos % 8) != 0) {
                mask |= (1ULL << (pos + 7));
            }
            passant_move_list[pos] = get_magic<std::vector<Move>>(mask, 2);
            for (Bitboard pawns: get_submasks(mask)) {
                std::vector<Move> result;
                if ((pos % 8) != 7 && ((pawns >> (pos + 9)) & 1)) {
                    result.push_back(create_move(pos + 9, pos, 7));
                }
                if ((pos % 8) != 0 && ((pawns >> (pos + 7)) & 1)) {
                    result.push_back(create_move(pos + 7, pos, 7));
                }
                passant_move_list[pos][pawns] = result;
            }
        }

    }

    const std::vector<Move> &get_passant(Square passant_flag, Bitboard pawns) {
        return passant_move_list[passant_flag][pawns];
    }

    void calc_attack_mask() {
        for (Square pos = 0; pos < 64; ++pos) {
            uint8_t i = (pos >> 3);
            uint8_t j = (pos & 7);
            if (j != 0) {
                if (i != 7) {
                    pawn_attack_bitboard[pos][WHITE] |= (1ULL << (pos + 7));
                }
                if (i != 0) {
                    pawn_attack_bitboard[pos][BLACK] |= (1ULL << (pos - 9));
                }
            }
            if (j != 7) {
                if (i != 7) {
                    pawn_attack_bitboard[pos][WHITE] |= (1ULL << (pos + 9));
                }
                if (i != 0) {
                    pawn_attack_bitboard[pos][BLACK] |= (1ULL << (pos - 7));
                }
            }
        }
    }

    std::vector<Move> get_moves(Bitboard pawns, Bitboard turn_bitboard, Bitboard opposite_bitboard, uint8_t turn) {
        std::vector<Move> result;
        Bitboard occupied = turn_bitboard | opposite_bitboard;
        Bitboard check_bitboard = pawns;
        if (turn == WHITE) {
            while (check_bitboard > 0) {
                Square pos = pop_lsb(check_bitboard);
                uint8_t i = (pos >> 3);
                uint8_t j = (pos & 7);
                if (i != 7 && ((occupied >> (pos + 8)) & 1) == 0) {
                    if (i == 6) {
                        result.push_back(pos + ((pos + 8) << 6) + (10 << 12));
                        result.push_back(pos + ((pos + 8) << 6) + (11 << 12));
                        result.push_back(pos + ((pos + 8) << 6) + (12 << 12));
                        result.push_back(pos + ((pos + 8) << 6) + (13 << 12));
                    } else {
                        result.push_back(pos + ((pos + 8) << 6));
                    }
                    if (i == 1 && ((occupied >> (pos + 16)) & 1) == 0) {
                        result.push_back(pos + ((pos + 16) << 6));
                    }
                }
                if (j != 0 && ((opposite_bitboard >> (pos + 7)) & 1)) {
                    if (i == 6) {
                        result.push_back(pos + ((pos + 7) << 6) + (10 << 12));
                        result.push_back(pos + ((pos + 7) << 6) + (11 << 12));
                        result.push_back(pos + ((pos + 7) << 6) + (12 << 12));
                        result.push_back(pos + ((pos + 7) << 6) + (13 << 12));
                    } else {
                        result.push_back(pos + ((pos + 7) << 6));
                    }
                }
                if (j != 7 && ((opposite_bitboard >> (pos + 9)) & 1)) {
                    if (i == 6) {
                        result.push_back(pos + ((pos + 9) << 6) + (10 << 12));
                        result.push_back(pos + ((pos + 9) << 6) + (11 << 12));
                        result.push_back(pos + ((pos + 9) << 6) + (12 << 12));
                        result.push_back(pos + ((pos + 9) << 6) + (13 << 12));
                    } else {
                        result.push_back(pos + ((pos + 9) << 6));
                    }
                }

            }
        } else {
            while (check_bitboard > 0) {
                Square pos = pop_lsb(check_bitboard);
                uint8_t i = (pos >> 3);
                uint8_t j = (pos & 7);
                if (i != 0 && ((occupied >> (pos - 8)) & 1) == 0) {
                    if (i == 1) {
                        result.push_back(pos + ((pos - 8) << 6) + (10 << 12));
                        result.push_back(pos + ((pos - 8) << 6) + (11 << 12));
                        result.push_back(pos + ((pos - 8) << 6) + (12 << 12));
                        result.push_back(pos + ((pos - 8) << 6) + (13 << 12));
                    } else {
                        result.push_back(pos + ((pos - 8) << 6));
                    }
                    if (i == 6 && ((occupied >> (pos - 16)) & 1) == 0) {
                        result.push_back(pos + ((pos - 16) << 6));
                    }
                }
                if (j != 0 && ((opposite_bitboard >> (pos - 9)) & 1)) {
                    if (i == 1) {
                        result.push_back(pos + ((pos - 9) << 6) + (10 << 12));
                        result.push_back(pos + ((pos - 9) << 6) + (11 << 12));
                        result.push_back(pos + ((pos - 9) << 6) + (12 << 12));
                        result.push_back(pos + ((pos - 9) << 6) + (13 << 12));
                    } else {
                        result.push_back(pos + ((pos - 9) << 6));
                    }
                }
                if (j != 7 && ((opposite_bitboard >> (pos - 7)) & 1)) {
                    if (i == 1) {
                        result.push_back(pos + ((pos - 7) << 6) + (10 << 12));
                        result.push_back(pos + ((pos - 7) << 6) + (11 << 12));
                        result.push_back(pos + ((pos - 7) << 6) + (12 << 12));
                        result.push_back(pos + ((pos - 7) << 6) + (13 << 12));
                    } else {
                        result.push_back(pos + ((pos - 7) << 6));
                    }
                }
            }
        }
        return result;
    }

    void init() {
        calc_attack_mask();
        passant_precalc();
    }
}