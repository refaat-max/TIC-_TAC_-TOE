#ifndef USERAUTH_H
#define USERAUTH_H

#include <QString>
#include <QStringList>
#include "gamelogic.h"
#include <QSqlDatabase>
#include <QSqlQuery>

class UserAuth
{
public:
    UserAuth();
    ~UserAuth();
    bool registerUser(const QString &username, const QString &password);
    bool login(const QString &username, const QString &password);
    void saveGameResult(const QString &username, GameResult result, bool vsAI);
    QStringList getGameHistory(const QString &username);

private:
    QSqlDatabase db;
    void initializeDatabase();
};

#endif // USERAUTH_H
