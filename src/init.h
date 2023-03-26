#ifndef CHESS_INIT_H
#define CHESS_INIT_H

#include "rook.h"
#include "bishop.h"
#include "king.h"
#include "knight.h"
#include "pawn.h"
#include "zobrist_hash.h"

#include "evaluate.h"

namespace init {
    void init() {
        init_figure_material();
        std::cout << "ROOK" << std::endl;
        rook_logic::init();
        std::cout << "BISHOP" << std::endl;
        bishop_logic::init();
        std::cout << "KING" << std::endl;
        king_logic::init();
        std::cout << "KNIGHT" << std::endl;
        knight_logic::init();
        std::cout << "PAWN" << std::endl;
        pawn_logic::init();
        std::cout << "ZOBRIST" << std::endl;
        zobrist_hashing::init();
        std::cout << "END" << std::endl;
    }
}

#endif //CHESS_INIT_H
