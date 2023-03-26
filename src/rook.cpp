#include "rook.h"

namespace rook_logic {
    Magic<Bitboard> move_mask[64];
    Magic<std::vector<Move>> move_list[64];

    std::vector<std::pair<int, int>> delta = {{1,  0},
                                              {-1, 0},
                                              {0,  1},
                                              {0,  -1}};


    void move_mask_precalc() {
        uint8_t added_space = 2;
        for (Square pos = 0; pos < 64; ++pos) {
            uint64_t mask = sliding_figure::get_mask(pos, delta);
            move_mask[pos] = get_magic<Bitboard>(mask, added_space);
            for (Bitboard occupied: get_submasks(mask)) {
                Bitboard res = sliding_figure::get_move_mask(pos, occupied, delta);
                move_mask[pos][occupied] = res;
            }
        }
    }

    void move_list_precalc() {
        uint8_t added_space = 2;
        for (Square pos = 0; pos < 64; ++pos) {
            std::vector<Bitboard> possible_moves = sliding_figure::all_possible_moves(pos, delta);
            std::sort(possible_moves.begin(), possible_moves.end());
            possible_moves.resize(std::unique(possible_moves.begin(), possible_moves.end()) - possible_moves.begin());
            move_list[pos] = get_magic<std::vector<Move>>(50, possible_moves);
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
        return move_mask[pos][turn_bitboard | opposite_bitboard] & (~turn_bitboard);
    }

    const std::vector<Move> &get_moves(Square pos, Bitboard turn_bitboard, Bitboard opposite_bitboard) {
        return move_list[pos][move_mask[pos][turn_bitboard | opposite_bitboard] & (~turn_bitboard)];
    }

    void init() {
        move_mask_precalc();
        move_list_precalc();
        return;
    }

}