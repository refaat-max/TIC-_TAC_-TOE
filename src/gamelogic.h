// gamelogic.h
#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <vector>

enum Cell { CELL_EMPTY, CELL_X, CELL_O };
enum Player { PLAYER_X, PLAYER_O };
enum GameResult { GAME_ONGOING, PLAYER_X_WINS, PLAYER_O_WINS, GAME_DRAW };

class GameLogic
{
public:
    GameLogic();
    void resetGame();
    bool makeMove(int cellIndex);
    bool isCellEmpty(int cellIndex) const;
    Cell getCellState(int cellIndex) const;
    Player getCurrentPlayer() const;
    GameResult checkGameStatus() const;
    int getBestMove(); // AI move using minimax

private:
    std::vector<Cell> board;
    Player currentPlayer;

    int minimax(std::vector<Cell> &board, int depth, bool isMaximizing, int alpha, int beta);
    GameResult evaluateBoard(const std::vector<Cell> &board) const;
    std::vector<int> getAvailableMoves(const std::vector<Cell> &board) const;
};

#endif // GAMELOGIC_H
