#ifndef CHESS_SLIDING_FIGURE_H
#define CHESS_SLIDING_FIGURE_H

#include <vector>
#include <algorithm>

#include "types.h"


namespace sliding_figure {
    Bitboard get_mask(Square pos, const std::vector<std::pair<int, int>> &delta);

    Bitboard get_move_mask(Square pos, Bitboard occupied, const std::vector<std::pair<int, int>> &delta);

    void possible_moves_dfs(const std::vector<std::vector<Bitboard>> &dir_masks, std::vector<Bitboard> &result,
                            int i = 0, Bitboard mask = 0);

    std::vector<Bitboard> all_possible_moves(Square pos, const std::vector<std::pair<int, int>> &delta);

}

//#include "sliding_figure.cpp"

#endif //CHESS_SLIDING_FIGURE_H
