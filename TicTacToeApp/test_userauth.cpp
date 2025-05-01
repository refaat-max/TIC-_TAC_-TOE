#include <QtTest/QTest>
#include "test_userauth.h"
#include "userauth.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>

void TestUserAuth::initTestCase() {
    // Initialize test database
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "testConnection");
        db.setDatabaseName("users.db");
        QVERIFY(db.open());

        QSqlQuery query(db);
        query.exec("DROP TABLE IF EXISTS game_history");
        query.exec("DROP TABLE IF EXISTS users");
        query.exec("CREATE TABLE users (username TEXT PRIMARY KEY, password TEXT NOT NULL)");
        query.exec("CREATE TABLE game_history ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "username TEXT, "
                   "datetime TEXT, "
                   "result TEXT, "
                   "opponent TEXT, "
                   "FOREIGN KEY (username) REFERENCES users(username))");
    }
}

void TestUserAuth::testRegisterUser() {
    QVERIFY(userAuth.registerUser("testuser", "password"));
    QVERIFY(!userAuth.registerUser("testuser", "password")); // Should fail (duplicate user)
}

void TestUserAuth::testLogin() {
    userAuth.registerUser("testuser", "password");
    QVERIFY(userAuth.login("testuser", "password"));
    QVERIFY(!userAuth.login("testuser", "wrongpassword"));
}

void TestUserAuth::testSaveGameResult() {
    userAuth.registerUser("testuser", "password");
    userAuth.saveGameResult("testuser", PLAYER_X_WINS, true);
    userAuth.saveGameResult("testuser", GAME_DRAW, false);

    QStringList history = userAuth.getGameHistory("testuser");
    QVERIFY(history.size() == 2);
    QVERIFY(history[0].contains("X won") && history[0].contains("vs AI"));
    QVERIFY(history[1].contains("Draw") && history[1].contains("vs Human"));
}

void TestUserAuth::testGetGameHistory() {
    // Reset game_history table to start fresh
    {
        QSqlDatabase db = QSqlDatabase::database("testConnection");
        QSqlQuery query(db);
        query.exec("DELETE FROM game_history");
    }

    userAuth.registerUser("testuser", "password");
    userAuth.saveGameResult("testuser", PLAYER_O_WINS, true);

    QStringList history = userAuth.getGameHistory("testuser");
    QVERIFY(history.size() == 1);
    QVERIFY(history[0].contains("O won") && history[0].contains("vs AI"));

    // Test with non-existent user
    QStringList emptyHistory = userAuth.getGameHistory("nonexistent");
    QVERIFY(emptyHistory.isEmpty());
}

void TestUserAuth::cleanupTestCase() {
    // Close the userAuth database connection (will be done automatically by destructor)
    // Remove the explicit destructor call to avoid double destruction
    if (QSqlDatabase::contains("userauth_connection")) {
        QSqlDatabase::database("userauth_connection").close();
        QSqlDatabase::removeDatabase("userauth_connection");
    }

    // Close and remove the test connection
    if (QSqlDatabase::contains("testConnection")) {
        QSqlDatabase::database("testConnection").close();
        QSqlDatabase::removeDatabase("testConnection");
    }

    QFile::remove("users.db");
}
