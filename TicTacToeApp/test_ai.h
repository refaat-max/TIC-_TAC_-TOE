#ifndef TESTAI_H
#define TESTAI_H

#include <QObject>
#include "gamelogic.h"

class TestAI : public QObject {
    Q_OBJECT
private:
    GameLogic gameLogic;
private slots:
    void initTestCase();
    void testGetBestMove();
    void testMinimaxWin();
    void testMinimaxBlock();
    void testMinimaxDraw();
    void cleanupTestCase();
};

#endif // TESTAI_H
