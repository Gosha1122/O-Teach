#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include "defines.h"
#include <QFile>

class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = nullptr);

    QString getLogPath() const;
    void setLogPath(const QString &newLogPath);

    void message(QString msg, Fatal::Message type);

    void closeFile();

signals:

private:
    QString logPath;
    QTextStream* logTextStream;

    QFile logFile;

};

#endif // LOGGER_H
