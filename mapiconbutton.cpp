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

bool MapIconButton::isNormal()
{
    return !(title == "" && description == "" && origingPath == "" && prevPath == "" && sz == 0 && pixSize == 0);
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
        emit deleteMap(index);
    }else if(selectedAction == readyAction ) {
        emit openMap();
    }
}

void MapIconButton::setIndex(int newIndex)
{
    index = newIndex;
}

QString MapIconButton::getPrevPath() const
{
    return prevPath;
}

QString MapIconButton::getDescription() const
{
    return description;
}

void MapIconButton::setPixSize(int newPixSize)
{
    pixSize = newPixSize;
}

void MapIconButton::setSz(int newSz)
{
    sz = newSz;
}

void MapIconButton::setPrevPath(const QString &newPrevPath)
{
    prevPath = newPrevPath;
    ui->imageLabel->setPixmap(QPixmap(prevPath));
}

void MapIconButton::setOrigingPath(const QString &newOrigingPath)
{
    origingPath = newOrigingPath;
}

void MapIconButton::setDescription(const QString &newDescription)
{
    description = newDescription;
    setToolTip(description);
}

void MapIconButton::setTitle(const QString &newTitle)
{
    title = newTitle;
    ui->headerLabel->setText(title);
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
