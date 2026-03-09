#include "mapiconbutton.h"
#include "ui_mapiconbutton.h"
#include <QDebug>
#include <QMenu>

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

void MapIconButton::setData(QString title, QString description, QString originPath, QString prevPath, int size, int pixSize)
{
    this->title = title;
    this->description = description;
    this->origingPath = originPath;
    this->prevPath = prevPath;
    this->sz = size;
    this->pixSize = pixSize;
    ui->imageLabel->setPixmap(QPixmap(prevPath));
    ui->headerLabel->setText(title);
}

void MapIconButton::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "Pressed";
    if(event->button() == Qt::LeftButton){
        Q_UNUSED(event);

        emit openMap();
    }else if(event->button() == Qt::RightButton){

    }
}

void MapIconButton::contextMenuEvent(QContextMenuEvent *event)
{
    QFrame::contextMenuEvent(event);

    QMenu menu;
    QAction* deleteAction = menu.addAction("Удалить");
    QAction* readyAction = menu.addAction("Выбрать");

    QAction *selectedAction = menu.exec(event->globalPos());
    if(selectedAction == deleteAction){
        this->deleteLater();
    }else if(selectedAction == readyAction ) {
        emit openMap();
    }
}

int MapIconButton::getPixSize() const
{
    return pixSize;
}

int MapIconButton::getSz() const
{
    return sz;
}

QString MapIconButton::getOrigingPath() const
{
    return origingPath;
}

QString MapIconButton::getTitle() const
{
    return title;
}
