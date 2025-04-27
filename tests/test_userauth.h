#ifndef TESTUSERAUTH_H
#define TESTUSERAUTH_H

#include <QObject>
#include "userauth.h"

class TestUserAuth : public QObject {
    Q_OBJECT
private:
    UserAuth userAuth;
private slots:
    void initTestCase();
    void testRegisterUser();
    void testLogin();
    void testSaveGameResult();
    void testGetGameHistory();
    void cleanupTestCase();
};

#endif // TESTUSERAUTH_H
