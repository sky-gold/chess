#include "search.h"

namespace search {

    EvalType capture_only(Board &board, EvalType alpha, EvalType beta, Search *search_container) {
        if (search_container->stop_flag.load()) {
            return EVAL_ZERO;
        }
        EvalType eval = evaluate::evaluate(board);
        if (eval >= beta) {
            return beta;
        }
        Hash hash_val = board.hash();
        TranspositionTable::Element element = search_container->capture_only_table.get(hash_val);
        if (element.hash == hash_val && element.final) {
            return element.evaluation;
        }
        Move refutation_move = MOVE_NULL;
        if (element.hash == hash_val) {
            refutation_move = element.best_move;
        }
        MoveList moves = move_ordering::get_ordered_legal_moves(board);
        if (refutation_move != MOVE_NULL) {
            for (int i = 0; i < moves.size(); ++i) {
                if (moves[i] == refutation_move) {
                    std::swap(moves[0], moves[i]);
                    break;
                }
            }
        }
        alpha = std::max(alpha, eval);
        Move best_move = MOVE_NULL;
        for (Move move: moves) {
            if (board.get_figure((move >> 6) & ((1ULL << 6) - 1)) == FIGURE_NULL) continue;
            board.do_move(move);
            eval = -capture_only(board, -beta, -alpha, search_container);
            if (eval >= EVAL_MAX) {
                --eval;
            }
            board.undo_last_move();
            if (alpha < eval) {
                best_move = move;
                alpha = eval;
            }
            if (eval >= beta) {
                search_container->capture_only_table.update(TranspositionTable::Element(hash_val, 1, eval, best_move, false));
                return beta;
            }
        }
        search_container->capture_only_table.update(TranspositionTable::Element(hash_val, 1, alpha, best_move, true));
        return alpha;
    }

    EvalType alpha_beta(Board &board, size_t depth, Search *search_container, EvalType alpha, EvalType beta) {
//        std::lock_guard lock(search_container->mtx);
//        std::cout << "alpha_beta depth=" << depth << " alpha=" << alpha << " beta=" << beta << std::endl;
//        std::cout << board.get_string() << std::endl;
//        std::cout << get_string_move(board.last_move()) << std::endl;
//        if (get_string_move(board.last_move()) == "a4h4") {
//            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
//        }
        if (search_container->stop_flag.load()) {
//            std::cout << "!!! stop !!!" << std::endl;
//            std::cout << "A " << EVAL_ZERO << std::endl;
            return EVAL_ZERO;
        }
        if (depth == 0) {
            EvalType res = capture_only(board, alpha, beta, search_container);
//            std::cout << "B " << res << std::endl;
            return res;
        }
        Hash hash_val = board.hash();
        TranspositionTable::Element element = search_container->transposition_table.get(hash_val);
        Move refutation_move = MOVE_NULL;
        if (element.hash == hash_val) {
            if (element.final && element.deep >= depth) {
                return element.evaluation;
            }
            refutation_move = element.best_move;
        }
        MoveList moves = move_ordering::get_ordered_legal_moves(board);
        if (moves.empty()) {
//            std::cout << "empty_moves" << std::endl;
            return evaluate::evaluate(board);
        }
        if (refutation_move != MOVE_NULL) {
            for (int i = 0; i < moves.size(); ++i) {
                if (moves[i] == refutation_move) {
                    std::swap(moves[0], moves[i]);
                    break;
                }
            }

        }
        Move best_move = MOVE_NULL;
        EvalType best_move_value = -EVAL_INF;
        for (Move move: moves) {
            board.do_move(move);
            EvalType val = -alpha_beta(board, depth - 1, search_container, -beta, -alpha);
            if (val >= EVAL_MAX) {
                --val;
            }
            board.undo_last_move();
            if (val > best_move_value) {
                best_move_value = val;
                best_move = move;
            }
            alpha = std::max(alpha, val);
            if (alpha >= beta) {
                search_container->transposition_table.update(TranspositionTable::Element(hash_val, depth, best_move_value, best_move, false));
                return alpha;
            }
        }
        search_container->transposition_table.update(TranspositionTable::Element(hash_val, depth, alpha, best_move, true));
        return alpha;
    }

    std::pair<Move, EvalType> find_best_move(Board board, size_t depth, Search* search_container) {
//        std::cout << "find_best_move depth=" << depth << std::endl;
        size_t thread_count = 2 * std::thread::hardware_concurrency();
        MoveList moves = move_ordering::get_ordered_legal_moves(board);
        if (moves.empty()) return {MOVE_NULL, EVAL_ZERO};
        std::vector<Board> boards;
        for (Move move: moves) {
//            std::cout << "move=" << get_string_move(move) << std::endl;
            board.do_move(move);
            boards.push_back(board);
            board.undo_last_move();
        }
        std::vector<std::thread> threads;
        std::vector<int> values(moves.size());
        for (size_t i = 0; i < thread_count; ++i) {
            threads.emplace_back([&boards, i, &values, depth, thread_count, search_container]() {
                for (size_t j = i; j < boards.size(); j += thread_count) {
                    values[j] = -alpha_beta(boards[j], depth - 1, search_container);
                }
            });
        }
        for (auto &thread: threads) {
            thread.join();
        }
        int best_index = 0;
        for (int i = 0; i < moves.size(); ++i) {
            if (values[i] > values[best_index]) {
                best_index = i;
            }
        }
        if (search_container->stop_flag.load()) {
            return {MOVE_NULL, EVAL_ZERO};
        }
        return {moves[best_index], values[best_index]};
    }

    void iterative_deeping(Board board, Search* search_container) {
        search_container->transposition_table.clear();
        search_container->capture_only_table.clear();
        if (board.get_legal_moves().empty()) {
            search_container->best_move = MOVE_NULL;
            search_container->eval = evaluate::evaluate(board);
            return;
        }
        search_container->best_move.store(board.get_legal_moves()[0]);
        search_container->eval.store(EVAL_ZERO);
        size_t depth = 1;
        while (true) {
            std::pair<Move, EvalType> result = find_best_move(board, depth, search_container);
            if (search_container->stop_flag.load()) {
                break;
            } else {
                search_container->best_move.store(result.first);
                search_container->eval.store(result.second);
                if (result.second == EVAL_ZERO || result.second >= EVAL_MAX || result.second <= -EVAL_MAX) {
                    break;
                }
            }
            ++depth;
        }
    }

    Move Search::get_move() const {
        return best_move.load();
    }

    EvalType Search::get_eval() const {
        return eval.load();
    }

    bool Search::is_working() const {
        return is_working_;
    }

    void Search::stop() {
        if (!is_working_) return;
        stop_flag.store(true);
        auto status = future_.wait_for(std::chrono::milliseconds(0));
        while (status != std::future_status::ready) {
//            std::cout << "bad status" << std::endl;
            status = future_.wait_for(std::chrono::milliseconds(1));
        }
        is_working_ = false;
    }

    void Search::set_board(const Board & new_board) {
        stop();
        stop_flag.store(false);
        is_working_ = true;
        board = new_board;
        transposition_table.clear();
        capture_only_table.clear();
        std::packaged_task<void()> task([&] {
            iterative_deeping(board, this);
        });
        future_ = task.get_future();

        std::thread t(std::move(task));
        t.detach();
    }

    Search::Search() = default;

    Search::~Search() {
        stop();
    }
}