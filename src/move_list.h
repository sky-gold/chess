#ifndef CHESS_MOVE_LIST_H
#define CHESS_MOVE_LIST_H

#include <cstdint>
#include <array>

#include "types.h"

const size_t MOVE_LIST_MAX = 218;

class MoveList {
public:
    MoveList() = default;

    void push_back(Move move) {
        data_[len_++] = move;
    }

    Move &operator[](uint8_t index) {
        return data_[index];
    }

    Move operator[](uint8_t index) const {
        return data_[index];
    }

    auto begin() {
        return data_.begin();
    }

    auto begin() const {
        return data_.begin();
    }

    auto end() {
        return data_.begin() + len_;
    }

    auto end() const {
        return data_.begin() + len_;
    }

    size_t size() const {
        return len_;
    }

    bool empty() const {
        return (len_ == 0);
    }

    void clear() {
        len_ = 0;
    }

private:
    std::array<Move, MOVE_LIST_MAX> data_;
    size_t len_ = 0;
};


#endif //CHESS_MOVE_LIST_H
