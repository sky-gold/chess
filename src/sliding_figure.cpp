#include "sliding_figure.h"

namespace sliding_figure {
    Bitboard get_mask(Square pos, const std::vector<std::pair<int, int>> &delta) {
        Bitboard mask = 0;
        int i = pos / 8;
        int j = pos % 8;
        for (auto [di, dj]: delta) {
            int new_i = i + di;
            int new_j = j + dj;
            while (new_i >= 0 && new_i < 8 && new_j >= 0 && new_j < 8) {
                mask |= (1ULL << ((new_i - di) * 8 + (new_j - dj)));
                new_i += di;
                new_j += dj;
            }
        }
        mask &= ~(1ULL << pos);
        return mask;
    }

    Bitboard get_move_mask(Square pos, Bitboard occupied, const std::vector<std::pair<int, int>> &delta) {
        Bitboard ans = 0;
        int i = pos / 8;
        int j = pos % 8;
        for (auto [di, dj]: delta) {
            int new_i = i + di;
            int new_j = j + dj;
            while (new_i >= 0 && new_i < 8 && new_j >= 0 && new_j < 8) {
                ans |= ((uint64_t) 1 << (new_i * 8 + new_j));
                if ((occupied >> (new_i * 8 + new_j)) & 1) {
                    break;
                }
                new_i += di;
                new_j += dj;
            }
        }
        return ans;
    }

    void possible_moves_dfs(const std::vector<std::vector<Bitboard>> &dir_masks, std::vector<Bitboard> &result,
                            int i, Bitboard mask) {
        if (i == dir_masks.size()) {
            result.push_back(mask);
            return;
        }
        for (auto tmp_mask: dir_masks[i]) {
            possible_moves_dfs(dir_masks, result, i + 1, mask | tmp_mask);
        }
        return;
    }

    std::vector<Bitboard> all_possible_moves(Square pos, const std::vector<std::pair<int, int>> &delta) {
        std::vector<std::vector<Bitboard>> dir_masks;
        int i = pos / 8;
        int j = pos % 8;
        for (auto [di, dj]: delta) {
            dir_masks.push_back({0});
            Bitboard mask = 0;
            int new_i = i + di;
            int new_j = j + dj;
            while (new_i >= 0 && new_i < 8 && new_j >= 0 && new_j < 8) {
                mask |= ((uint64_t) 1 << (new_i * 8 + new_j));
                dir_masks.back().push_back(mask);
                new_i += di;
                new_j += dj;
            }
        }
        std::vector<Bitboard> result;
        possible_moves_dfs(dir_masks, result);
        std::sort(result.begin(), result.end());
        result.resize(std::unique(result.begin(), result.end()) - result.begin());
        return result;
    }

}
