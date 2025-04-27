QT += core gui sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# Main application target
TEMPLATE = app
TARGET = AdvancedTicTacToe

SOURCES += \
    main.cpp \
    gamelogic.cpp \
    gamewindow.cpp \
    userauth.cpp

HEADERS += \
    gamelogic.h \
    gamewindow.h \
    userauth.h

# Test target
CONFIG(test) {
    TEMPLATE = app
    TARGET = TicTacToeTests
    QT += testlib
    INCLUDEPATH += .
    SOURCES = \
        test_gamelogic.cpp \
        test_ai.cpp \
        test_userauth.cpp \
        gamelogic.cpp \
        userauth.cpp
    HEADERS = \
        test_gamelogic.h \
        test_ai.h \
        test_userauth.h
}
