#ifndef MAPICONBUTTON_H
#define MAPICONBUTTON_H

#include <QFrame>
#include <QMouseEvent>
#include "logger.h"

namespace Ui {
class MapIconButton;
}

class MapIconButton : public QFrame
{
    Q_OBJECT

public:
    explicit MapIconButton(QWidget *parent = nullptr);
    ~MapIconButton();
    void setData(QString title, QString description, QString originPath, QString prevPath, int size, int pixSize);
    bool isNormal();

    QString getTitle() const;

    QString getOrigingPath() const;

    int getSz() const;

    int getPixSize() const;

    void setTitle(const QString &newTitle);

    void setDescription(const QString &newDescription);

    void setOrigingPath(const QString &newOrigingPath);

    void setPrevPath(const QString &newPrevPath);

    void setSz(int newSz);

    void setPixSize(int newPixSize);

    QString getDescription() const;

    QString getPrevPath() const;

    void setIndex(int newIndex);

    void setLogger(Logger *newLogger);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

signals:
    void openMap();
    void deleteMap(int index);

private:
    Ui::MapIconButton *ui;
    QString title = "";
    QString description = "";
    QString origingPath = "";
    QString prevPath = "";
    int sz = 0;
    int pixSize = 0;

    int index = -1;

    Logger* logger;

};

#endif // MAPICONBUTTON_H
