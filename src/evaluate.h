#ifndef CHESS_EVALUATE_H
#define CHESS_EVALUATE_H

#include <cmath>

#include "board.h"


#include "material_evaluate.h"
#include "positional_evaluate.h"

namespace evaluate {
    extern int figure_material[MAX_FIGURE];

    void init_figure_material();

    float checkmate_evaluate(const Board &board, float material);

    EvalType evaluate(const Board &board);

}


#endif //CHESS_EVALUATE_H
