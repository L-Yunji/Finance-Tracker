#ifndef USERDBMANAGER_H
#define USERDBMANAGER_H

#include <QString>
#include <QSqlDatabase>

class UserDBManager {
public:
    static bool initDB();
    static bool registerUser(const QString &username, const QString &password);
    static bool loginUser(const QString &username, const QString &password);
};

#endif // USERDBMANAGER_H
