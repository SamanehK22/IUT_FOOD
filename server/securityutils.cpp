#include "securityutils.h"
#include <QDebug>

QString SecurityUtils::hashPassword(const QString& password)
{
    // Generate a random salt
    QString salt = generateSalt();
    
    // Hash the password with the salt
    QString hashedPassword = hashWithSalt(password, salt);
    
    // Store the salt with the hash (salt:hash format)
    return salt + ":" + hashedPassword;
}

bool SecurityUtils::verifyPassword(const QString& password, const QString& storedHash)
{
    // Split the stored hash into salt and hash
    QStringList parts = storedHash.split(":");
    if (parts.size() != 2) {
        qDebug() << "Invalid stored hash format";
        return false;
    }

    QString salt = parts[0];
    QString storedHashedPassword = parts[1];

    // Hash the input password with the stored salt
    QString hashedInput = hashWithSalt(password, salt);

    // Compare the hashes
    return hashedInput == storedHashedPassword;
}

QString SecurityUtils::generateSessionToken()
{
    // Generate a UUID-based token with timestamp
    QString uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    QString timestamp = QString::number(QDateTime::currentDateTime().toSecsSinceEpoch());
    
    // Combine UUID and timestamp
    QString token = uuid + ":" + timestamp;
    
    // Hash the combined string
    return QString(QCryptographicHash::hash(token.toUtf8(), QCryptographicHash::Sha256).toHex());
}

bool SecurityUtils::isTokenExpired(const QString& token)
{
    // Extract timestamp from token
    QStringList parts = token.split(":");
    if (parts.size() != 2) {
        return true;
    }

    qint64 tokenTime = parts[1].toLongLong();
    qint64 currentTime = QDateTime::currentDateTime().toSecsSinceEpoch();
    
    // Check if token is older than TOKEN_EXPIRY_HOURS
    return (currentTime - tokenTime) > (TOKEN_EXPIRY_HOURS * 3600);
}

QString SecurityUtils::generateSalt()
{
    // Generate a random salt using QUuid
    return QUuid::createUuid().toString(QUuid::WithoutBraces).left(SALT_LENGTH);
}

QString SecurityUtils::hashWithSalt(const QString& password, const QString& salt)
{
    // Combine password and salt
    QString combined = password + salt;
    
    // Hash the combined string using SHA-256
    return QString(QCryptographicHash::hash(combined.toUtf8(), QCryptographicHash::Sha256).toHex());
} 