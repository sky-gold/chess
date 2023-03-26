#ifndef CHESS_EVALUATE_H
#define CHESS_EVALUATE_H

#include "board.h"


constexpr int INF = 1e9;

int figure_material[MAX_FIGURE];

void init_figure_material() {
    figure_material[PAWN | WHITE] = 100;
    figure_material[PAWN | BLACK] = -100;
    figure_material[BISHOP | WHITE] = 300;
    figure_material[BISHOP | BLACK] = -300;
    figure_material[KNIGHT | WHITE] = 300;
    figure_material[KNIGHT | BLACK] = -300;
    figure_material[ROOK | WHITE] = 500;
    figure_material[ROOK | BLACK] = -500;
    figure_material[QUEEN | WHITE] = 900;
    figure_material[QUEEN | BLACK] = -900;

}


int material_evaluate(const Board & board) {
    int result = 0;
    uint8_t turn = board.turn();
    result += 100 * (popcount(board.figure_bitboard(PAWN | turn)) - popcount(board.figure_bitboard(PAWN | turn ^ 1)));
    result += 300 * (popcount(board.figure_bitboard(KNIGHT | turn)) - popcount(board.figure_bitboard(KNIGHT | turn ^ 1)));
    result += 300 * (popcount(board.figure_bitboard(BISHOP | turn)) - popcount(board.figure_bitboard(BISHOP | turn ^ 1)));
    result += 500 * (popcount(board.figure_bitboard(ROOK | turn)) - popcount(board.figure_bitboard(ROOK | turn ^ 1)));
    result += 900 * (popcount(board.figure_bitboard(QUEEN | turn)) - popcount(board.figure_bitboard(QUEEN | turn ^ 1)));
    return result;
}

int positional_evaluate(const Board & board) {
    int result = 0;
    Bitboard white_pawn = board.figure_bitboard(PAWN | WHITE);
    Bitboard black_pawn = board.figure_bitboard(PAWN | BLACK);
    result += 70 * (popcount(white_pawn & BOARD_CORE) - popcount(black_pawn & BOARD_CORE));
    result += 100 * (popcount(white_pawn & ROW_6) - popcount(black_pawn & ROW_3));
    result += 250 * (popcount(white_pawn & ROW_7) - popcount(black_pawn & ROW_2));
    result += 7 * (popcount(board.figure_bitboard(KNIGHT | WHITE) & BOARD_CORE) -  popcount(board.figure_bitboard(KNIGHT | BLACK) & BOARD_CORE));
    result += 20 * (popcount(board.figure_bitboard(KNIGHT | WHITE) & BOARD_CENTER) -  popcount(board.figure_bitboard(KNIGHT | BLACK) & BOARD_CENTER));
    result += 8 * (popcount(board.figure_bitboard(BISHOP | WHITE) & BOARD_CORE) -  popcount(board.figure_bitboard(BISHOP | BLACK) & BOARD_CORE));
    result += 15 * (popcount(board.figure_bitboard(BISHOP | WHITE) & BOARD_CENTER) -  popcount(board.figure_bitboard(BISHOP | BLACK) & BOARD_CENTER));

    result += -13 * (popcount(white_pawn & (white_pawn >> 8)) - popcount(black_pawn & (black_pawn >> 8)));
    result += 8 * (popcount(white_pawn & ((white_pawn & (~COL_H)) << 9)) - popcount(black_pawn & ((black_pawn & (~COL_H)) >> 7)));
    result += 8 * (popcount(white_pawn & ((white_pawn & (~COL_A)) << 7)) - popcount(black_pawn & ((black_pawn & (~COL_A)) >> 9)));

    int k = board.step_count() / 5;
    if (k == 0) k = 1;
    result /= k;

    if (board.turn()) {
        return -result;
    } else {
        return result;
    }
}

int checkmate_evaluate(const Board & board, int material) {
//    std::cout << "AAAA" << std::endl;
    uint8_t turn = board.turn();
    Square turn_king = lsb(board.figure_bitboard(KING | turn));
    Square op_king = lsb(board.figure_bitboard(KING | turn ^ 1));
    int king_distance = abs((turn_king % 8) - (op_king % 8)) + abs((turn_king / 8) - (op_king / 8));
    int edge_distance = std::min((op_king % 8), 7 - (op_king % 8)) + std::min((op_king / 8), 7 - (op_king / 8));
    int min_edge_distance = std::min((op_king % 8), 7 - (op_king % 8)) + std::min((op_king / 8), 7 - (op_king / 8));
    int result = 0;
    result += 20 / (edge_distance + 1);
    result += 10 / (min_edge_distance + 1);
//    if (min_edge_distance <= 2) {
        result += 200 / (king_distance + 1 + min_edge_distance);
//    }
    if (material > 0) {
        return result;
    } else {
        return -result;
    }
}

int evaluate(const Board & board) {
    if (board.get_legal_moves().empty()) {
        if (board.state() == Board::State::Draw) {
            return 0;
        }
        return -INF + (int)board.step_count();
    }
    int result = material_evaluate(board);
    if (abs(result) >= 300 && (popcount(board.color_bitboard(WHITE)) < 5 || popcount(board.color_bitboard(BLACK)) < 5)) {
        result += checkmate_evaluate(board, result);
    }
    result += positional_evaluate(board);
    return result;
}

#endif //CHESS_EVALUATE_H
