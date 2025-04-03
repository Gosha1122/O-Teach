#ifndef MAPICONBUTTON_H
#define MAPICONBUTTON_H

#include <QFrame>
#include <QMouseEvent>

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

    QString getTitle() const;

    QString getOrigingPath() const;

    int getSz() const;

    int getPixSize() const;

protected:
    void mousePressEvent(QMouseEvent* event) override;

signals:
    void openMap();

private:
    Ui::MapIconButton *ui;
    QString title;
    QString description;
    QString origingPath;
    QString prevPath;
    int sz;
    int pixSize;

};

#endif // MAPICONBUTTON_H
