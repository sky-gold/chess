#ifndef CHESS_MOVE_LIST_H
#define CHESS_MOVE_LIST_H

#include <cstdint>
#include <array>

#include "types.h"

const size_t MOVE_LIST_MAX = 218;

class MoveList {
public:
    MoveList();

    void push_back(Move move);

    Move &operator[](uint8_t index);

    Move operator[](uint8_t index) const;

    Move *begin();

    [[nodiscard]] const Move *begin() const;

    Move *end();

    [[nodiscard]] const Move *end() const;

    [[nodiscard]] size_t size() const;

    [[nodiscard]] bool empty() const;

    void clear();

private:
    std::array<Move, MOVE_LIST_MAX> data_;
    size_t len_ = 0;
};


#endif //CHESS_MOVE_LIST_H
