#ifndef CHESS_MATERIAL_EVALUATE_H
#define CHESS_MATERIAL_EVALUATE_H

#include "board.h"

namespace material_evaluate {
    extern float figure_material[MAX_FIGURE];

    void init();

    float evaluate(const Board & board);
}

//#include "material_evaluate.cpp"

#endif //CHESS_MATERIAL_EVALUATE_H
