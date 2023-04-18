#include "move_ordering.h"

namespace move_ordering {
    MoveList get_ordered_legal_moves(const Board & board) {
        MoveList result;
        MoveList moves = board.get_legal_moves();
        std::random_shuffle(moves.begin(), moves.end());
        std::vector<bool> used(moves.size(), false);
        for (int i = 0; i < moves.size(); ++i) {
            uint8_t pos1 = moves[i] & ((1ULL << 6) - 1);
            uint8_t pos2 = (moves[i] >> 6) & ((1ULL << 6) - 1);
            if (!used[i] && abs(evaluate::figure_material[board.get_figure(pos2)]) > abs(evaluate::figure_material[board.get_figure(pos1)])) {
                result.push_back(moves[i]);
                used[i] = true;
            }
        }
        for (int i = 0; i < moves.size(); ++i) {
            uint8_t pos1 = moves[i] & ((1ULL << 6) - 1);
            uint8_t pos2 = (moves[i] >> 6) & ((1ULL << 6) - 1);
            if (!used[i] && abs(evaluate::figure_material[board.get_figure(pos2)]) == abs(evaluate::figure_material[board.get_figure(pos1)])) {
                result.push_back(moves[i]);
                used[i] = true;
            }
        }
        for (int i = 0; i < moves.size(); ++i) {
            if (!used[i]) {
                result.push_back(moves[i]);
                used[i] = true;
            }
        }
        return result;
    }
}