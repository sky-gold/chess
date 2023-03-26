#ifndef CHESS_MOVE_GEN_TEST_H
#define CHESS_MOVE_GEN_TEST_H

#include "board.h"

namespace move_gen_test {
    size_t perft(Board &board, size_t deep) {
        if (deep == 0) return 1;
        size_t result = 0;
        MoveList moves = board.get_legal_moves();
        for (auto move: moves) {
            //        std::cout << "deep=" << deep << std::endl;
            //        std::cout << board.get_string() << std::endl;
            //        std::cout << get_string_move(move) << std::endl;
            board.do_move(move);
            //        std::cout << board.get_string() << std::endl;
            //        std::cout << "{" << std::endl;
            result += perft(board, deep - 1);
            //        std::cout << "undo" << std::endl;
            board.undo_last_move();
            //        std::cout << "}" << std::endl;
        }
        return result;
    }

    void speed_test() {
        std::vector<std::string> fens = {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
                                         "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"};
        for (auto fen: fens) {
            Board board(fen);
            for (int i = 1; i <= 6; ++i) {
                clock_t start = clock();
                size_t result = perft(board, i);
                clock_t finish = clock();
                std::cout << "deep=" << i << " nodes=" << result << " time=" << (finish - start) << "ms speed="
                          << ((float) result / (float) (finish - start)) << " nodes/ms" << std::endl;
            }
        }
    }

    struct Test {
        Test(std::string fen, size_t deep, size_t expected_result) : fen(fen), deep(deep),
                                                                     expected_result(expected_result) {};

        bool check() {
            Board board(fen);
            size_t real_result = perft(board, deep);
            return (real_result == expected_result);
        }

        std::string fen = "";
        size_t deep = 0;
        size_t expected_result = 0;
        size_t real_result;
    };


    void test() {
        std::cout << "MOVE GEN TEST" << std::endl;
        std::vector<Test> tests;
        tests.emplace_back("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 4, 197281);
        tests.emplace_back("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 4, 4085603);
        tests.emplace_back("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 5, 674624);
        tests.emplace_back("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 4, 422333);
        tests.emplace_back("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ", 4, 2103487);
        tests.emplace_back("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 ", 4, 3894594);
        size_t k = 0;
        for (auto test: tests) {
            std::cout << "TESTED " << k << "/" << tests.size() << std::endl;
            if (!test.check()) {
                std::cout << "Wrong answer on move gen test" << std::endl;
                std::cout << "fen=" << test.fen << std::endl;
                std::cout << "deep=" << test.deep << std::endl;
                std::cout << "expected " << test.expected_result << " but gоt " << test.real_result << std::endl;
                exit(3);
            }
            ++k;
        }
        std::cout << "TESTED " << k << "/" << tests.size() << std::endl;
    }

}

#endif //CHESS_MOVE_GEN_TEST_H
