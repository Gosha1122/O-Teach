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
    Q_UNUSED(event);
    qDebug() << "Pressed";
    emit openMap();
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
