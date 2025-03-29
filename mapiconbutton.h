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

protected:
    void mousePressEvent(QMouseEvent* event) override;

signals:
    void openMap();

private:
    Ui::MapIconButton *ui;
};

#endif // MAPICONBUTTON_H
