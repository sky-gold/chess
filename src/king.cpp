#include "king.h"

namespace king_logic {
    Bitboard move_mask[64];
    Magic<std::vector<Move>> move_list[64];

    std::vector<std::pair<int, int>> delta = {{1,  0},
                                              {-1, 0},
                                              {0,  1},
                                              {0,  -1},
                                              {1,  1},
                                              {1,  -1},
                                              {-1, 1},
                                              {-1, -1}};

    void move_mask_precalc() {
        for (Square pos = 0; pos < 64; ++pos) {
            move_mask[pos] = jumping_figure::get_mask(pos, delta);
        }
    }

    void move_list_precalc() {
        for (Square pos = 0; pos < 64; ++pos) {
            std::vector<Bitboard> possible_moves = get_submasks(move_mask[pos]);
            move_list[pos] = get_magic<std::vector<Move>>(55, possible_moves);
            for (auto possible_mask: possible_moves) {
                Bitboard check_mask = possible_mask;
                std::vector<Move> moves;
                while (check_mask > 0) {
                    Square to = pop_lsb(check_mask);
                    moves.push_back(pos | (to << 6));
                }
                move_list[pos][possible_mask] = moves;
            }
        }
    }

    Bitboard get_moves_bitboard(Square pos, Bitboard turn_bitboard, Bitboard opposite_bitboard) {
        return move_mask[pos] & (~turn_bitboard);
    }

    const std::vector<Move> &get_moves(Square pos, Bitboard turn_bitboard, Bitboard opposite_bitboard) {
        return move_list[pos][move_mask[pos] & (~turn_bitboard)];
    }

    void init() {
        move_mask_precalc();
        move_list_precalc();
    }
}