#ifndef CHESS_SEARCH_H
#define CHESS_SEARCH_H

#include <thread>
#include <unordered_map>
#include <mutex>
#include <future>

#include "board.h"
#include "evaluate.h"
#include "move_ordering.h"
#include "transposition_table.h"

namespace search {

    class Search {
    public:
        Search();

        ~Search();

        void set_board(const Board & new_board);

        void stop();

        Move get_move() const;

        EvalType get_eval() const;

        bool is_working() const;

        TranspositionTable transposition_table;
        TranspositionTable capture_only_table;

        Board board;
        std::atomic<bool> stop_flag = false;
        std::atomic<Move> best_move = MOVE_NULL;
        std::atomic<EvalType> eval = EVAL_ZERO;
//        std::mutex mtx;
    private:
        std::future<void> future_;
        bool is_working_ = false;
    };

    EvalType capture_only(Board &board, EvalType alpha, EvalType beta, Search *search_container);

    EvalType alpha_beta(Board &board, size_t depth, Search *search_container, EvalType alpha = -EVAL_INF, EvalType beta = EVAL_INF);

    std::pair<Move, EvalType> find_best_move(Board board, size_t depth, Search* search_container);

    void iterative_deeping(Board board, Search* search_container);
}

//#include "search.cpp"

#endif //CHESS_SEARCH_H
