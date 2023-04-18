#include "repetition_history.h"

RepetitionHistory::RepetitionHistory() {
    data_.resize(100);
    len_.resize(100);
}

void RepetitionHistory::add_pos(Hash hash) {
    data_[version_][len_[version_]++] = hash;
}

void RepetitionHistory::undo() {
    assert(len_[version_] > 0);
    --len_[version_];
    if (len_[version_] == 0 && version_ > 0) {
        --version_;
    }
}

uint8_t RepetitionHistory::count(Hash hash) const {
    uint8_t result = 0;
    for (uint8_t i = 0; i < len_[version_]; ++i) {
        if (data_[version_][i] == hash) {
            ++result;
        }
    }
    return result;
}

void RepetitionHistory::clear() {
    if (version_ + 1 >= data_.size()) {
        data_.push_back({});
    }
    ++version_;
    len_[version_] = 0;
}