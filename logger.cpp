#include "logger.h"
#include <QTextStream>
#include <QDateTime>

QString Logger::getLogPath() const
{
    return logPath;
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



Logger &Logger::getInstance(QString path)
{
    static Logger instance(path);
    return instance;
}

Logger::Logger(QString path):logPath(path), logFile(path){
    logTextStream = new QTextStream(&logFile);
}

Logger::~Logger(){
    logFile.close();
}
