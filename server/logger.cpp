#include "logger.h"
#include <QDebug>
#include <QFileInfo>
#include <QDateTime>

Logger* Logger::instance = nullptr;

Logger* Logger::getInstance() {
    if (!instance) {
        instance = new Logger();
    }
    return instance;
}

Logger::Logger(QObject *parent)
    : QObject(parent)
    , currentLogLevel(LogLevel::INFO)
    , consoleOutput(true)
    , maxLogSize(10) // 10MB default
    , maxLogFiles(5) // Keep 5 log files by default
{
    // Set default log file path
    QString logDir = QDir::currentPath() + "/logs";
    QDir().mkpath(logDir);
    logFilePath = logDir + "/iutfood_" + QDateTime::currentDateTime().toString("yyyy-MM-dd") + ".log";
    
    logFile.setFileName(logFilePath);
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Failed to open log file:" << logFilePath;
    }
    logStream.setDevice(&logFile);
}

Logger::~Logger() {
    if (logFile.isOpen()) {
        logFile.close();
    }
}

void Logger::debug(const QString& message, const QString& component) {
    log(LogLevel::DEBUG, message, component);
}

void Logger::info(const QString& message, const QString& component) {
    log(LogLevel::INFO, message, component);
}

void Logger::warning(const QString& message, const QString& component) {
    log(LogLevel::WARNING, message, component);
}

void Logger::error(const QString& message, const QString& component) {
    log(LogLevel::ERROR, message, component);
    emit errorOccurred(message, component);
}

void Logger::critical(const QString& message, const QString& component) {
    log(LogLevel::CRITICAL, message, component);
    emit errorOccurred(message, component);
}

void Logger::setLogLevel(LogLevel level) {
    currentLogLevel = level;
}

void Logger::setLogFile(const QString& filePath) {
    QMutexLocker locker(&mutex);
    
    if (logFile.isOpen()) {
        logFile.close();
    }
    
    logFilePath = filePath;
    logFile.setFileName(logFilePath);
    
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Failed to open log file:" << logFilePath;
    }
    logStream.setDevice(&logFile);
}

void Logger::enableConsoleOutput(bool enable) {
    consoleOutput = enable;
}

void Logger::setMaxLogSize(int sizeInMB) {
    maxLogSize = sizeInMB;
}

void Logger::setMaxLogFiles(int count) {
    maxLogFiles = count;
}

QString Logger::getLogLevelString(LogLevel level) const {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

void Logger::log(LogLevel level, const QString& message, const QString& component) {
    if (level < currentLogLevel) {
        return;
    }

    QMutexLocker locker(&mutex);
    
    QString formattedMessage = formatLogMessage(level, message, component);
    
    if (consoleOutput) {
        qDebug() << formattedMessage;
    }
    
    writeToFile(formattedMessage);
    emit logMessage(level, message, component);
    
    checkLogRotation();
}

void Logger::writeToFile(const QString& message) {
    if (logFile.isOpen()) {
        logStream << message << Qt::endl;
        logStream.flush();
    }
}

void Logger::checkLogRotation() {
    if (!logFile.isOpen()) {
        return;
    }

    QFileInfo fileInfo(logFilePath);
    if (fileInfo.size() >= maxLogSize * 1024 * 1024) { // Convert MB to bytes
        rotateLogs();
    }
}

void Logger::rotateLogs() {
    QMutexLocker locker(&mutex);
    
    // Close current log file
    logFile.close();
    
    // Get the directory and base name
    QFileInfo fileInfo(logFilePath);
    QString dir = fileInfo.absolutePath();
    QString baseName = fileInfo.baseName();
    
    // Remove oldest log file if we've reached the maximum
    QString oldestLog = dir + "/" + baseName + "_" + QString::number(maxLogFiles) + ".log";
    QFile::remove(oldestLog);
    
    // Rotate existing log files
    for (int i = maxLogFiles - 1; i > 0; --i) {
        QString oldName = dir + "/" + baseName + "_" + QString::number(i) + ".log";
        QString newName = dir + "/" + baseName + "_" + QString::number(i + 1) + ".log";
        QFile::rename(oldName, newName);
    }
    
    // Rename current log file
    QString newName = dir + "/" + baseName + "_1.log";
    QFile::rename(logFilePath, newName);
    
    // Create new log file
    logFile.setFileName(logFilePath);
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Failed to open new log file:" << logFilePath;
    }
    logStream.setDevice(&logFile);
}

QString Logger::formatLogMessage(LogLevel level, const QString& message, const QString& component) {
    return QString("[%1] [%2] [%3] %4")
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
        .arg(getLogLevelString(level))
        .arg(component)
        .arg(message);
} 