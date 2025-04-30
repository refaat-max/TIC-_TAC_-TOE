QT += core gui sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# Main application target
TEMPLATE = app
TARGET = AdvancedTicTacToe

SOURCES += \
    src/main.cpp \
   GUI/gamelogic.cpp \
    GUI/gamewindow.cpp \
        sql codes/userauth.cpp

HEADERS += \
    src/gamelogic.h \
    GUI/gamewindow.h \
    sql codes/userauth.h

# Test target
CONFIG(test) {
    TEMPLATE = app
    TARGET = TicTacToeTests
    QT += testlib
    INCLUDEPATH += .
    SOURCES = \
        tests/test_gamelogic.cpp \
        tests/test_ai.cpp \
        tests/test_userauth.cpp \
        src/gamelogic.cpp \
        sql codes/userauth.cpp
    HEADERS = \
        tests/test_gamelogic.h \
        tests/test_ai.h \
        tests/test_userauth.h
}
