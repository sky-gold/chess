# Chess
* c++ chess engine based on [alpha–beta pruning](https://en.wikipedia.org/wiki/Alpha–beta_pruning)
* Interface written using the SFML library
## How to use
* Clone this repo
* Download [SFML 2.5.1](https://www.sfml-dev.org/download/sfml/2.5.1/) to `src/lib/`
## Used techniques
### Move Generation features
* Using bitboard to store a chessboard
* Preliminary calculation of possible moves for the pieces
* Multiply-right-shift perfect hashing algorithm called [Magic Bitboards](https://www.chessprogramming.org/Magic_Bitboards)
### Search features
* [Alpha–beta pruning](https://en.wikipedia.org/wiki/Alpha–beta_pruning)
* [Quiescence Search technique](https://www.chessprogramming.org/Quiescence_Search) used to avoid the [horizon effect](https://en.wikipedia.org/wiki/Horizon_effect)
* Using a [Move ordering](https://www.chessprogramming.org/Move_Ordering)  to speed up alpha-beta search
* Multithreading
* [Zobrist hashing](https://en.wikipedia.org/wiki/Zobrist_hashing)
* [Transposition table](https://en.wikipedia.org/wiki/Transposition_table) based on Shared Hash Table
