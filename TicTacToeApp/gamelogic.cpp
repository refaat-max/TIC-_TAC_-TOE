// gamelogic.cpp
#include "gamelogic.h"
#include <algorithm>

GameLogic::GameLogic()
{
    resetGame();
}

void GameLogic::resetGame()
{
    board.clear();
    board.resize(9, CELL_EMPTY);
    currentPlayer = PLAYER_X;
}

bool GameLogic::makeMove(int cellIndex)
{
    if (cellIndex < 0 || cellIndex >= 9 || board[cellIndex] != CELL_EMPTY) {
        return false;
    }

    board[cellIndex] = (currentPlayer == PLAYER_X) ? CELL_X : CELL_O;
    currentPlayer = (currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
    return true;
}

bool GameLogic::isCellEmpty(int cellIndex) const
{
    if (cellIndex < 0 || cellIndex >= 9) {
        return false;
    }
    return board[cellIndex] == CELL_EMPTY;
}

Cell GameLogic::getCellState(int cellIndex) const
{
    if (cellIndex < 0 || cellIndex >= 9) {
        return CELL_EMPTY;
    }
    return board[cellIndex];
}

Player GameLogic::getCurrentPlayer() const
{
    return currentPlayer;
}

GameResult GameLogic::checkGameStatus() const
{
    return evaluateBoard(board);
}

GameResult GameLogic::evaluateBoard(const std::vector<Cell> &board) const
{
    // Check rows
    for (int i = 0; i < 3; i++) {
        if (board[i*3] != CELL_EMPTY &&
            board[i*3] == board[i*3+1] &&
            board[i*3] == board[i*3+2]) {
            return (board[i*3] == CELL_X) ? PLAYER_X_WINS : PLAYER_O_WINS;
        }
    }

    // Check columns
    for (int i = 0; i < 3; i++) {
        if (board[i] != CELL_EMPTY &&
            board[i] == board[i+3] &&
            board[i] == board[i+6]) {
            return (board[i] == CELL_X) ? PLAYER_X_WINS : PLAYER_O_WINS;
        }
    }

    // Check diagonals
    if (board[0] != CELL_EMPTY &&
        board[0] == board[4] &&
        board[0] == board[8]) {
        return (board[0] == CELL_X) ? PLAYER_X_WINS : PLAYER_O_WINS;
    }

    if (board[2] != CELL_EMPTY &&
        board[2] == board[4] &&
        board[2] == board[6]) {
        return (board[2] == CELL_X) ? PLAYER_X_WINS : PLAYER_O_WINS;
    }

    // Check for draw
    bool hasEmptyCell = false;
    for (Cell cell : board) {
        if (cell == CELL_EMPTY) {
            hasEmptyCell = true;
            break;
        }
    }

    return hasEmptyCell ? GAME_ONGOING : GAME_DRAW;
}

std::vector<int> GameLogic::getAvailableMoves(const std::vector<Cell> &board) const
{
    std::vector<int> moves;
    for (size_t i = 0; i < board.size(); i++) {  // Change 'int' to 'size_t'
        if (board[i] == CELL_EMPTY) {
            moves.push_back(i);
        }
    }
    return moves;
}

int GameLogic::getBestMove()
{
    int bestScore = -1000;
    int bestMove = -1;

    std::vector<int> availableMoves = getAvailableMoves(board);

    for (int move : availableMoves) {
        std::vector<Cell> boardCopy = board;
        boardCopy[move] = CELL_O; // AI is always O

        int score = minimax(boardCopy, 0, false, -1000, 1000);

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }

    return bestMove;
}

int GameLogic::minimax(std::vector<Cell> &board, int depth, bool isMaximizing, int alpha, int beta)
{
    GameResult result = evaluateBoard(board);

    // Terminal states
    if (result == PLAYER_X_WINS) return -10 + depth;
    if (result == PLAYER_O_WINS) return 10 - depth;
    if (result == GAME_DRAW) return 0;

    std::vector<int> availableMoves = getAvailableMoves(board);

    if (isMaximizing) {
        int bestScore = -1000;
        for (int move : availableMoves) {
            board[move] = CELL_O;
            int score = minimax(board, depth + 1, false, alpha, beta);
            board[move] = CELL_EMPTY;

            bestScore = std::max(bestScore, score);
            alpha = std::max(alpha, bestScore);
            if (beta <= alpha) break;  // Alpha-beta pruning
        }
        return bestScore;
    } else {
        int bestScore = 1000;
        for (int move : availableMoves) {
            board[move] = CELL_X;
            int score = minimax(board, depth + 1, true, alpha, beta);
            board[move] = CELL_EMPTY;

            bestScore = std::min(bestScore, score);
            beta = std::min(beta, bestScore);
            if (beta <= alpha) break;  // Alpha-beta pruning
        }
        return bestScore;
    }
}
