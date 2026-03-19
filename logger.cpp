#include "logger.h"
#include <QTextStream>
#include <QDateTime>

Logger::Logger(QObject *parent)
    : QObject{parent}
{}

QString Logger::getLogPath() const
{
    return logPath;
}

void Logger::setLogPath(const QString &newLogPath)
{
    logPath = newLogPath;
    logFile.setFileName(logPath);
    logFile.open(QIODevice::WriteOnly | QIODevice::Append);
    logTextStream = new QTextStream(&logFile);
    qDebug() << newLogPath;

}

void Logger::message(QString msg, Fatal::Message type)
{
    QString dt = QDateTime::currentDateTime().toString("yyyy_MM_dd-hh:mm:ss");
    QString txt = QString("[%1] ").arg(dt);

    switch(type){
        case Fatal::Message::Debug:    txt += QString("{debug} \t\t %1").arg(msg);  break;
        case Fatal::Message::Warning:  txt += QString("{Warning} \t %1").arg(msg);  break;
        case Fatal::Message::Critical: txt += QString("{Critical} \t %1").arg(msg); break;
        case Fatal::Message::Fatal:    txt += QString("{Fatal} \t\t %1").arg(msg);  break;

    }
    (*logTextStream) << txt << "\n";
}

void Logger::closeFile()
{
    logFile.close();
}
