#ifndef CHESS_MOVE_ORDERING_H
#define CHESS_MOVE_ORDERING_H

#include <algorithm>

#include "board.h"
#include "evaluate.h"

namespace move_ordering {
    MoveList get_ordered_legal_moves(const Board & board);
}

#endif //CHESS_MOVE_ORDERING_H
