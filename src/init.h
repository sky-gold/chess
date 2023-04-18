#ifndef CHESS_INIT_H
#define CHESS_INIT_H

#include "rook.h"
#include "bishop.h"
#include "king.h"
#include "knight.h"
#include "pawn.h"
#include "zobrist_hash.h"

#include "evaluate.h"
#include "material_evaluate.h"
#include "positional_evaluate.h"


namespace init {

    void init_evaluate() {
        positional_evaluate::init();
        material_evaluate::init();
        evaluate::init_figure_material();

    }

    void init_figures() {
        rook_logic::init();
        bishop_logic::init();
        king_logic::init();
        knight_logic::init();
        pawn_logic::init();
    }

    void init_hash() {
        zobrist_hashing::init();

    }

    void init() {
        std::cout << "Evaluate Init..." << std::endl;
        init_evaluate();
        std::cout << "Figures Init..." << std::endl;
        init_figures();
        std::cout << "Hash Init..." << std::endl;
        init_hash();
        std::cout << "Init end" << std::endl;
    }
}

#endif //CHESS_INIT_H
