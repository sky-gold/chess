//
// Created by nikita on 12.03.2023.
//

#ifndef CHESS_REPETITION_HISTORY_H
#define CHESS_REPETITION_HISTORY_H

#include <vector>
#include <array>
#include <cassert>

#include "types.h"
#include "constant.h"

class RepetitionHistory {
public:
    RepetitionHistory();

    void add_pos(Hash hash);

    void undo();

    uint8_t count(Hash hash) const;

    void clear();

private:
    std::vector<std::array<Hash, MAX_QUIET_MOVES>> data_;
    std::vector<uint8_t> len_;
    size_t version_ = 0;

};


#endif //CHESS_REPETITION_HISTORY_H
