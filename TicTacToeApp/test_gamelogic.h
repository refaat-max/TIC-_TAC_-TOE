#ifndef TESTGAMELOGIC_H
#define TESTGAMELOGIC_H

#include <QObject>
#include "gamelogic.h"

class TestGameLogic : public QObject {
    Q_OBJECT
private slots:
    void initTestCase();
    void testResetGame();
    void testMakeMove();
    void testCheckGameStatusHorizontal();
    void testCheckGameStatusVertical();
    void testCheckGameStatusDiagonal();
    void testCheckGameStatusDraw();
private:
    GameLogic gameLogic;
};

#endif // TESTGAMELOGIC_H
