#ifndef LOGGER_H
#define LOGGER_H


#include "defines.h"
#include <QFile>

class Logger
{

public:
    Logger(const Logger&) = delete;
    Logger& operator= (const Logger&) = delete;

    QString getLogPath() const;

    void message(QString msg, Fatal::Message type);

    static Logger& getInstance(QString path = "");

signals:

private:
    QString logPath;
    QTextStream* logTextStream;

    QFile logFile;

    Logger(QString path);
    ~Logger();

};

#endif // LOGGER_H
