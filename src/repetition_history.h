//
// Created by nikita on 12.03.2023.
//

#ifndef CHESS_REPETITION_HISTORY_H
#define CHESS_REPETITION_HISTORY_H

#include <vector>
#include <array>
#include <assert.h>

#include "types.h"
#include "constant.h"

class RepetitionHistory {
public:
    RepetitionHistory() {
        data_.resize(100);
        len_.resize(100);
    };

    void add_pos(Hash hash) {
        data_[version_][len_[version_]++] = hash;
    }

    uint8_t count(Hash hash) {
        uint8_t result = 0;
        for (uint8_t i = 0; i < len_[version_]; ++i) {
            if (data_[version_][i] == hash) {
                ++result;
            }
        }
        return result;
    }

    void clear() {
        if (version_ + 1 >= data_.size()) {
            data_.push_back({});
        }
        ++version_;
        len_[version_] = 0;
    }

    void unclear() {
        assert(version_ > 0);
        --version_;
    }

private:
    std::vector<std::array<Hash, MAX_QUIET_MOVES>> data_;
    std::vector<uint8_t> len_;
    size_t version_ = 0;

};


#endif //CHESS_REPETITION_HISTORY_H
