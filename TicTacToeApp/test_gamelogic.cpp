#include <QtTest/QTest>
#include "test_gamelogic.h"
#include "test_ai.h"
#include "test_userauth.h"

void TestGameLogic::initTestCase() {
    // Initialize the game logic for testing
    gameLogic.resetGame();
}

void TestGameLogic::testResetGame() {
    // Test that the game board is reset correctly
    gameLogic.makeMove(0);
    gameLogic.resetGame();
    for (int i = 0; i < 9; ++i) {
        QCOMPARE(gameLogic.getCellState(i), CELL_EMPTY);
    }
    QCOMPARE(gameLogic.getCurrentPlayer(), PLAYER_X);
    QCOMPARE(gameLogic.checkGameStatus(), GAME_ONGOING);
}

void TestGameLogic::testMakeMove() {
    // Test making a move
    gameLogic.resetGame();
    QVERIFY(gameLogic.makeMove(0));
    QCOMPARE(gameLogic.getCellState(0), CELL_X);
    QCOMPARE(gameLogic.getCurrentPlayer(), PLAYER_O);
    QVERIFY(!gameLogic.makeMove(0)); // Already occupied
}

void TestGameLogic::testCheckGameStatusHorizontal() {
    // Test horizontal win
    gameLogic.resetGame();
    gameLogic.makeMove(0); // X
    gameLogic.makeMove(3); // O
    gameLogic.makeMove(1); // X
    gameLogic.makeMove(4); // O
    gameLogic.makeMove(2); // X
    QCOMPARE(gameLogic.checkGameStatus(), PLAYER_X_WINS);
}

void TestGameLogic::testCheckGameStatusVertical() {
    // Test vertical win
    gameLogic.resetGame();
    gameLogic.makeMove(0); // X
    gameLogic.makeMove(1); // O
    gameLogic.makeMove(3); // X
    gameLogic.makeMove(4); // O
    gameLogic.makeMove(6); // X
    QCOMPARE(gameLogic.checkGameStatus(), PLAYER_X_WINS);
}

void TestGameLogic::testCheckGameStatusDiagonal() {
    // Test diagonal win
    gameLogic.resetGame();
    gameLogic.makeMove(0); // X
    gameLogic.makeMove(1); // O
    gameLogic.makeMove(4); // X
    gameLogic.makeMove(2); // O
    gameLogic.makeMove(8); // X
    QCOMPARE(gameLogic.checkGameStatus(), PLAYER_X_WINS);
}

void TestGameLogic::testCheckGameStatusDraw() {
    // Test draw condition
    gameLogic.resetGame();
    gameLogic.makeMove(0); // X
    gameLogic.makeMove(4); // O
    gameLogic.makeMove(8); // X
    gameLogic.makeMove(1); // O
    gameLogic.makeMove(7); // X
    gameLogic.makeMove(6); // O
    gameLogic.makeMove(2); // X
    gameLogic.makeMove(5); // O
    gameLogic.makeMove(3); // X
    QCOMPARE(gameLogic.checkGameStatus(), GAME_DRAW);
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    int status = 0;

    // Run TestGameLogic
    TestGameLogic testGameLogic;
    status |= QTest::qExec(&testGameLogic, argc, argv);

    // Run TestAI
    TestAI testAI;
    status |= QTest::qExec(&testAI, argc, argv);

    // Run TestUserAuth
    TestUserAuth testUserAuth;
    status |= QTest::qExec(&testUserAuth, argc, argv);

    return status;
}
