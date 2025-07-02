#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QMutex>
#include <QDir>

class Logger : public QObject
{
    Q_OBJECT

public:
    enum class LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        CRITICAL
    };

    static Logger* getInstance();

    // Logging methods
    void debug(const QString& message, const QString& component = "General");
    void info(const QString& message, const QString& component = "General");
    void warning(const QString& message, const QString& component = "General");
    void error(const QString& message, const QString& component = "General");
    void critical(const QString& message, const QString& component = "General");

    // Configuration
    void setLogLevel(LogLevel level);
    void setLogFile(const QString& filePath);
    void enableConsoleOutput(bool enable);
    void setMaxLogSize(int sizeInMB);
    void setMaxLogFiles(int count);

    // Utility methods
    QString getLogLevelString(LogLevel level) const;
    void rotateLogs();

signals:
    void logMessage(LogLevel level, const QString& message, const QString& component);
    void errorOccurred(const QString& error, const QString& component);

private:
    explicit Logger(QObject *parent = nullptr);
    ~Logger();
    static Logger* instance;

    void log(LogLevel level, const QString& message, const QString& component);
    void writeToFile(const QString& message);
    void checkLogRotation();
    QString formatLogMessage(LogLevel level, const QString& message, const QString& component);

    LogLevel currentLogLevel;
    QString logFilePath;
    QFile logFile;
    QTextStream logStream;
    bool consoleOutput;
    int maxLogSize;
    int maxLogFiles;
    QMutex mutex;
};

#endif // LOGGER_H 