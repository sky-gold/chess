#ifndef CHESS_SEARCH_H
#define CHESS_SEARCH_H

#include <thread>
#include <unordered_map>
#include <mutex>

#include "board.h"
#include "evaluate.h"
#include "move_ordering.h"
#include "transposition_table.h"

namespace search {

    constexpr int MAX_DEPTH = 12;

//std::unordered_map<Hash, int> search_value;
    TranspositionTable transposition_table;
    TranspositionTable capture_only_table;

    clock_t MAX_TIME = 5000;

    clock_t start_time;
    bool time_ended = false;

    int search_capture_only(Board & board, int alpha, int beta) {
        if (time_ended || clock() > MAX_TIME + start_time) {
            time_ended = true;
            return 0;
        }
        int eval = evaluate(board);
        if (eval >= beta) {
            return beta;
        }
        Hash hash_val = board.hash();
        if (transposition_table.count(hash_val)) {
            return transposition_table.get(hash_val).first;
        }
        alpha = std::max(alpha, eval);
        MoveList moves = get_ordered_legal_moves(board);
        for (Move move: moves) {
            if (board.get_figure((move >> 6) & ((1ULL << 6) - 1)) == FIGURE_NULL) continue;
            board.do_move(move);
            eval = -search_capture_only(board, -beta, -alpha);
            board.undo_last_move();
            if (eval >= beta) {
                return beta;
            }
            alpha = std::max(alpha, eval);
        }
        transposition_table.update(hash_val, 1, alpha);
        return alpha;
    }
//std::mutex mtx;

    int alpha_beta(Board &board, int depth, int alpha = -INF, int beta = INF) {
        if (time_ended || clock() > MAX_TIME + start_time) {
            time_ended = true;
            return 0;
        }
        if (depth == 0) {
            return search_capture_only(board, alpha, beta);//evaluate(board);
        }
        Hash hash_val = board.hash();
        int result;
//    std::cout << "! " <<  transposition_table.count(hash_val) << std::endl;
        if (transposition_table.count(hash_val) >= depth) {
//        std::cout << "OK" << std::endl;
            result = transposition_table.get(hash_val).first;
            return result;
        }
//    std::cout << "KOK" << std::endl;
//    if (depth == 0) {
//        result = evaluate(board);
//        return result;
//    }
        MoveList moves = get_ordered_legal_moves(board);
        if (moves.empty()) {
            result = evaluate(board);
            return result;
        }
        Move probably_best_move = MOVE_NULL;
        if (transposition_table.count(hash_val) != 0) {
            probably_best_move = transposition_table.get(hash_val).second;
        }
        if (probably_best_move != MOVE_NULL) {
            for (int i = 0; i < moves.size(); ++i) {
                if (moves[i] == probably_best_move) {
                    std::swap(moves[0], moves[i]);
                    break;
                }
            }
        }
        Move best_move = MOVE_NULL;
        int best_move_value = -INF;
        for (Move move: moves) {
            board.do_move(move);
            int val = -alpha_beta(board, depth - 1, -beta, -alpha);
            board.undo_last_move();
            if (val > best_move_value) {
                best_move_value = val;
                best_move = move;
            }
            alpha = std::max(alpha, val);
            if (alpha >= beta) {
                return alpha;
            }
        }
        transposition_table.update(hash_val, depth, alpha, best_move);
        return alpha;
    }

    Move find_best_move(Board board, int depth = 6) {
//    size_t thread_count = std::thread::hardware_concurrency();
//    best_move.clear();
//    search_value.clear();
//        std::cout << "depth=6" << std::endl;
        transposition_table.clear();
        MoveList moves = board.get_legal_moves();
        if (moves.empty()) {
            return MOVE_NULL;
        }
        if (moves.size() == 1) {
            return moves[0];
        }
        std::vector<Board> boards;
        for (Move move: moves) {
            board.do_move(move);
            boards.push_back(board);
            board.undo_last_move();
        }
        std::vector<std::thread> threads;
        std::vector<int> values(moves.size());
        for (int i = 0; i < moves.size(); ++i) {
            threads.emplace_back([&boards, i, &values, depth]() {
                values[i] = -alpha_beta(boards[i], depth - 1, -INF, INF);
            });
        }
        for (auto &thread: threads) {
            thread.join();
        }
        int best_index = 0;
//        std::cout << std::endl;
        for (int i = 0; i < moves.size(); ++i) {
//            std::cout << get_string_move(moves[i]) << " " << values[i] << std::endl;
            if (values[i] > values[best_index]) {
                best_index = i;
            }
        }
        if (time_ended) return MOVE_NULL;
//        std::cout << "! " << get_string_move(moves[best_index]) << std::endl;
        return moves[best_index];
    }

    Move iterative_deepening(Board board, clock_t max_time=5000) {
//        std::cout << "iterative_deepening" << std::endl;
        std::cout << std::endl;
        if (board.get_legal_moves().empty()) {
            return MOVE_NULL;
        }
        MAX_TIME = max_time;
        transposition_table.clear();
        start_time = clock();
        time_ended = false;
        Move result = board.get_legal_moves()[0];
        for (int depth = 2; depth <= MAX_DEPTH; depth += 2) {
            Move move = find_best_move(board, depth);
            if (time_ended) {
                std::cout << "depth=" << depth - 2 << std::endl;
//                std::cout << "result=" << get_string_move(result) << std::endl;
                return result;
            } else {
                result = move;
            }
        }
        return result;
    }
}

#endif //CHESS_SEARCH_H
