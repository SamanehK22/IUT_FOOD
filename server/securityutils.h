#ifndef SECURITYUTILS_H
#define SECURITYUTILS_H

#include <QString>
#include <QCryptographicHash>
#include <QDateTime>
#include <QUuid>

class SecurityUtils
{
public:
    static QString hashPassword(const QString& password);
    static bool verifyPassword(const QString& password, const QString& hashedPassword);
    static QString generateSessionToken();
    static bool isTokenExpired(const QString& token);
    static QString generateSalt();
    static QString hashWithSalt(const QString& password, const QString& salt);

private:
    static const int SALT_LENGTH = 16;
    static const int TOKEN_EXPIRY_HOURS = 24;
};

#endif // SECURITYUTILS_H 