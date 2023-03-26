#ifndef CHESS_JUMPING_FIGURE_H
#define CHESS_JUMPING_FIGURE_H

#include <vector>
#include <algorithm>

#include "types.h"


namespace jumping_figure {
    Bitboard get_mask(Square pos, const std::vector<std::pair<int, int>> &delta);

}

#include "jumping_figure.cpp"

#endif //CHESS_JUMPING_FIGURE_H
