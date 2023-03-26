#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <string>
#include <vector>
#include <array>

#include "types.h"
#include "move_list.h"
#include "repetition_history.h"
#include "misc.h"
#include "constant.h"
#include "zobrist_hash.h"
#include "clear_array.h"

#include "rook.h"
#include "bishop.h"
#include "pawn.h"
#include "king.h"
#include "knight.h"
#include "queen.h"

class Board {
public:
    enum State : uint8_t {
        WhiteWin, BlackWin, Draw, None
    };

    explicit Board(std::string fen_string=fen_start);

    std::string fen() const;
    Figure get_figure(Square pos) const;
    Bitboard figure_bitboard(Figure figure) const;
    Bitboard color_bitboard(uint8_t color) const;
    bool turn() const;
    size_t step_count() const;
    State state() const;
    bool is_repetition_rule() const;
    bool is_50rule() const;
    Move last_move() const;
    Hash hash() const;
    MoveList get_legal_moves() const;

    bool is_legal(Move move) const;

    void do_move(Move move);
    void undo_last_move();

    std::string get_string() const;



private:
    struct Changes {
        Changes() = default;
        Square old_passant_flag = SQUARE_NULL;
        uint8_t old_castling_flag = 15;
        uint8_t old_quiet_count = 0;
        Square pos = SQUARE_NULL;
        Figure figure = FIGURE_NULL;
    };

    mutable std::array<Figure, 64> board_;
    mutable bool legal_moves_calced = false;
    mutable MoveList legal_moves_;
    mutable std::array<Bitboard, MAX_FIGURE> figure_bitboard_{};
    mutable std::array<Bitboard, 2> color_bitboard_{};

    std::vector<std::pair<Move, Changes>> moves_;
    uint8_t turn_ = WHITE;
    Square passant_flag_ = SQUARE_NULL;
    uint8_t castling_flag_ = 15; //KQKq 3210
    size_t step_count_ = 0;
    uint8_t quiet_count_ = 0;
    Hash hash_ = 0;
    RepetitionHistory repetition_history_;

    void change_pos_(Square pos, Figure figure);
    void fast_change_pos_(Square pos, Figure figure) const;

    bool is_attacked_(Square pos, uint8_t color) const;
    Square king_pos_() const;

    Changes fast_move_(Move move) const;
    void fast_undo_(Move move, Changes changes) const;


    void add_to_legal_(const std::vector<uint16_t> & moves) const;
    void add_to_legal_(uint16_t move) const;

    bool short_castling_() const;
    bool long_castling_() const;
    void calc_legal_moves_() const;
};

#include "board.cpp"

#endif //CHESS_BOARD_H
