#ifndef CHESS_POSITIONAL_EVALUATE_H
#define CHESS_POSITIONAL_EVALUATE_H

#include "types.h"
#include "board.h"

namespace positional_evaluate {

    extern float midgame_value[64][MAX_FIGURE];
    extern float endgame_value[64][MAX_FIGURE];

    void midgame_init();
    void endgame_init();
    void init();

    float evaluate(const Board & board);
}

//#include "positional_evaluate.cpp"

#endif //CHESS_POSITIONAL_EVALUATE_H
