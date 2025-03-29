#include "mapiconbutton.h"
#include "ui_mapiconbutton.h"
#include <QDebug>

MapIconButton::MapIconButton(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::MapIconButton)
{
    ui->setupUi(this);
    setMouseTracking(true);
}

MapIconButton::~MapIconButton()
{
    delete ui;
}

void MapIconButton::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    qDebug() << "Pressed";
    emit openMap();
}
