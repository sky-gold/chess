#include "jumping_figure.h"

namespace jumping_figure {
    Bitboard get_mask(Square pos, const std::vector<std::pair<int, int>> &delta) {
        Bitboard mask = 0;
        int i = pos / 8;
        int j = pos % 8;
        for (auto [di, dj]: delta) {
            int new_i = i + di;
            int new_j = j + dj;
            if (new_i >= 0 && new_i < 8 && new_j >= 0 && new_j < 8) {
                mask |= (1ULL << (new_i * 8 + new_j));
            }
        }
        return mask;
    }

}