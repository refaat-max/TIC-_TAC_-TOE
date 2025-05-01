#include <QtTest/QTest>
#include "test_ai.h"

void TestAI::initTestCase() {
    // Reset the game board for tests
    gameLogic.resetGame();
}

void TestAI::testGetBestMove() {
    // Test AI's ability to make a move
    gameLogic.resetGame();
    int move = gameLogic.getBestMove();
    QVERIFY(move >= 0 && move < 9); // Ensure move is valid
    QVERIFY(gameLogic.isCellEmpty(move)); // Ensure the move is to an empty cell
}

void TestAI::testMinimaxWin() {
    // Test AI's minimax algorithm for a winning move (AI is O)
    gameLogic.resetGame();
    gameLogic.makeMove(1); // X
    gameLogic.makeMove(0); // O
    gameLogic.makeMove(2); // X
    gameLogic.makeMove(3); // O
    gameLogic.makeMove(5); // X
    gameLogic.makeMove(4); // O
    gameLogic.makeMove(7); // X
    gameLogic.makeMove(8); // O
    // Board: O X X | O O X | _ X O
    // After the above moves, currentPlayer is PLAYER_X
    // Only position 6 is empty, and placing O there wins (vertical 0-3-6)
    int move = gameLogic.getBestMove();
    QCOMPARE(move, 6); // AI (O) should win by placing at position 6 (vertical 0-3-6)
    gameLogic.makeMove(move); // Places CELL_O at position 6
    QCOMPARE(gameLogic.checkGameStatus(), PLAYER_O_WINS);
}

void TestAI::testMinimaxBlock() {
    // Test AI's ability to block opponent's win (AI is O)
    gameLogic.resetGame();
    gameLogic.makeMove(0); // X
    gameLogic.makeMove(1); // O
    gameLogic.makeMove(3); // X
    // After the above moves, currentPlayer is PLAYER_X
    int move = gameLogic.getBestMove();
    QCOMPARE(move, 6); // AI (O) should block X's win by placing at position 6 (vertical 0-3-6)
    gameLogic.makeMove(move); // Places CELL_O at position 6
    QCOMPARE(gameLogic.checkGameStatus(), GAME_ONGOING);
}

void TestAI::testMinimaxDraw() {
    // Test AI's handling of a draw scenario (AI is O)
    gameLogic.resetGame();
    gameLogic.makeMove(4); // X
    gameLogic.makeMove(0); // O
    gameLogic.makeMove(8); // X
    gameLogic.makeMove(1); // O
    gameLogic.makeMove(7); // X
    gameLogic.makeMove(6); // O
    gameLogic.makeMove(2); // X
    gameLogic.makeMove(5); // O
    // After the above moves, currentPlayer is PLAYER_X
    int move = gameLogic.getBestMove();
    QCOMPARE(move, 3); // Last move to draw
    gameLogic.makeMove(move); // Places CELL_O at position 3
    QCOMPARE(gameLogic.checkGameStatus(), GAME_DRAW);
}

void TestAI::cleanupTestCase() {
    // No specific cleanup needed for now
}
