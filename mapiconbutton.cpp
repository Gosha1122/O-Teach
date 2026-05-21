#include "mapiconbutton.h"
#include "ui_mapiconbutton.h"
#include <QDebug>
#include <QMenu>
#include <QFile>

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

void MapIconButton::setData(QString title, QString description, QString originPath, QString prevPath, int size, int pixSize, qreal longetude, qreal latitude, QString srtmPath)
{
    this->title = title;
    this->description = description;
    this->origingPath = originPath;
    this->prevPath = prevPath;
    this->sz = size;
    this->pixSize = pixSize;
    ui->imageLabel->setPixmap(QPixmap(prevPath));
    ui->headerLabel->setText(title);
    this->longetude_lt = longetude;
    this->latitude_lt  = latitude;
    this->srtmPath = srtmPath;
}

bool MapIconButton::isNormal()
{
    return !(title == "" && description == "" && origingPath == "" && prevPath == "" && sz == 0 && pixSize == 0 && longetude_lt == 0 && latitude_lt == 0);
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
        QFile::remove(this->origingPath);
        QFile::remove(this->prevPath);
        this->deleteLater();
        emit deleteMap(index);
    }else if(selectedAction == readyAction ) {
        emit openMap();
    }
}

void MapIconButton::setSrtmPath(const QString &newSrtmPath)
{
    srtmPath = newSrtmPath;
}

void MapIconButton::setLatitude_lt(qreal newLatitude_lt)
{
    latitude_lt = newLatitude_lt;
}

void MapIconButton::setLongetude_lt(qreal newLongetude_lt)
{
    longetude_lt = newLongetude_lt;
}

qreal MapIconButton::getLatitude_lt() const
{
    return latitude_lt;
}

qreal MapIconButton::getLongetude_lt() const
{
    return longetude_lt;
}

QString MapIconButton::getSrtmPath() const
{
    return srtmPath;
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
