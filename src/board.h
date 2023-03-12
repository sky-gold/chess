#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <string>
#include <vector>
#include <array>

#include "types.h"
#include "move_list.h"
#include "repetition_history.h"

class Board {
public:
    enum State : uint8_t {
        WhiteWin, BlackWin, Draw, Continues
    };
    explicit Board(std::string fen);

    std::string fen() const;
    Figure get_figure(Square pos) const;
    Bitboard figure_bitboard(Figure figure) const;
    bool turn() const;
    size_t step_count() const;
    State state() const;
    Move last_move() const;
    uint64_t hash() const;
    std::vector<Move> get_legal_moves() const;

    void do_move(Move move);
    void undo_last_move();

private:
    struct Changes {
        Changes() = default;
        Square old_passant_flag = SQUARE_NULL;
        uint8_t old_castling_flag = 15;
    };

    std::array<std::vector<Figure>, 64> board_;
    std::vector<MoveList> legal_move_list;
    size_t version_;
    std::array<Bitboard, MAX_FIGURE> figure_bitboard_{};
    std::array<Bitboard, 2> color_bitboard_{};
    std::vector<std::pair<Move, Changes>> moves_;
    uint8_t turn_ = WHITE;
    Square passant_flag_ = SQUARE_NULL;
    uint8_t castling_flag_ = 15; //KQKq 3210
    size_t version_delta_ = 0;
    Hash hash_ = 0;
    RepetitionHistory repetition_history_;

    void change_pos_(Square pos, Figure figure);
    void force_move_(Move move);
    bool is_attacked_(Square pos);
    bool short_castling_();
    bool long_castling_();
    void calc_legal_moves();
};


#endif //CHESS_BOARD_H
