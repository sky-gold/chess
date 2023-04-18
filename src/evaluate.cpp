#include "evaluate.h"

namespace evaluate {
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

    float checkmate_evaluate(const Board & board, float material) {
        uint8_t turn = board.turn();
        Square win_king = lsb(board.figure_bitboard(KING | turn));
        Square op_king = lsb(board.figure_bitboard(KING | turn ^ 1));
        if (material < 0) {
            std::swap(win_king, op_king);
        }
        int dx = abs((win_king % 8) - (op_king % 8));
        int dy = abs((win_king / 8) - (op_king / 8));
        float king_distance = (float)sqrt(dx*dx + dy*dy) + dx + dy;
        int edge_distance = std::min((op_king % 8), 7 - (op_king % 8)) + std::min((op_king / 8), 7 - (op_king / 8));
        int min_edge_distance = std::min((op_king % 8), 7 - (op_king % 8)) + std::min((op_king / 8), 7 - (op_king / 8));
        float result = 0;
        result += 8.0f - (float)(edge_distance + 1);
        result += 8.0f - (float)(min_edge_distance + 1);
        result += 10.0f -  (king_distance);
        result *= 0.1f;
        if (material > 0) {
            return result;
        } else {
            return -result;
        }
    }

    EvalType evaluate(const Board & board) {
        if (board.get_legal_moves().empty()) {
            if (board.state() == Board::State::Draw) {
                return EVAL_ZERO;
            }
            return -EVAL_INF;
        }
        float material = material_evaluate::evaluate(board);
        float positional = positional_evaluate::evaluate(board);
        float result = material + positional;
        if (abs(result) >= 3 && (popcount(board.color_bitboard(WHITE)) < 5 || popcount(board.color_bitboard(BLACK)) < 5)) {
            result += checkmate_evaluate(board, material);
        }
        int eval = std::lround(10 * result) * 10;
        if (eval >= EVAL_MAX) return EVAL_MAX;
        if (eval <= -EVAL_MAX) return -EVAL_MAX;
        if (eval == EVAL_ZERO) return 1;
        return (EvalType)eval;
    }
}