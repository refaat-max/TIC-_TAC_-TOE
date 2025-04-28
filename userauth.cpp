#include "userauth.h"
#include <QSqlError>
#include <QDateTime>
#include <QDebug>

UserAuth::UserAuth()
{
    initializeDatabase();
}

UserAuth::~UserAuth()
{
    if (db.isOpen()) {
        db.close();
    }
}

void UserAuth::initializeDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("users.db");

    if (!db.open()) {
        qDebug() << "Error: Could not open database:" << db.lastError().text();
        return;
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "username TEXT PRIMARY KEY, "
               "password TEXT NOT NULL)");

    query.exec("CREATE TABLE IF NOT EXISTS game_history ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "username TEXT, "
               "datetime TEXT, "
               "result TEXT, "
               "opponent TEXT, "
               "FOREIGN KEY (username) REFERENCES users(username))");

    if (query.lastError().isValid()) {
        qDebug() << "Error creating tables:" << query.lastError().text();
    }
}

bool UserAuth::registerUser(const QString &username, const QString &password)
{
    if (!db.isOpen()) {
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT username FROM users WHERE username = :username");
    query.bindValue(":username", username);
    if (query.exec() && query.next()) {
        return false;
    }

    query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    if (!query.exec()) {
        qDebug() << "Error registering user:" << query.lastError().text();
        return false;
    }
    return true;
}

bool UserAuth::login(const QString &username, const QString &password)
{
    if (!db.isOpen()) {
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = :username");
    query.bindValue(":username", username);
    if (!query.exec() || !query.next()) {
        return false;
    }

    QString storedPassword = query.value(0).toString();
    return storedPassword == password;
}

void UserAuth::saveGameResult(const QString &username, GameResult result, bool vsAI)
{
    if (!db.isOpen()) {
        return;
    }

    QString resultStr;
    switch (result) {
    case PLAYER_X_WINS:
        resultStr = "X won";
        break;
    case PLAYER_O_WINS:
        resultStr = "O won";
        break;
    case GAME_DRAW:
        resultStr = "Draw";
        break;
    default:
        return;
    }

    QString opponent = vsAI ? "AI" : "Human";
    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    QSqlQuery query;
    query.prepare("INSERT INTO game_history (username, datetime, result, opponent) "
                  "VALUES (:username, :datetime, :result, :opponent)");
    query.bindValue(":username", username);
    query.bindValue(":datetime", datetime);
    query.bindValue(":result", resultStr);
    query.bindValue(":opponent", opponent);

    if (!query.exec()) {
        qDebug() << "Error saving game result:" << query.lastError().text();
    }
}

QStringList UserAuth::getGameHistory(const QString &username)
{
    QStringList history;
    if (!db.isOpen()) {
        return history;
    }

    QSqlQuery query;
    query.prepare("SELECT datetime, result, opponent FROM game_history WHERE username = :username");
    query.bindValue(":username", username);
    if (!query.exec()) {
        qDebug() << "Error retrieving game history:" << query.lastError().text();
        return history;
    }

    while (query.next()) {
        QString datetime = query.value("datetime").toString();
        QString result = query.value("result").toString();
        QString opponent = query.value("opponent").toString();
        QString entry = QString("[%1] %2 (vs %3)").arg(datetime, result, opponent);
        history << entry;
    }

    return history;
}
