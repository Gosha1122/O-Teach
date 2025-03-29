#include "mapcropborderitem.h"

MapCropBorderItem::MapCropBorderItem(QObject *parent)
    : QObject{parent}
{}

QRectF MapCropBorderItem::boundingRect() const
{
    return QRectF(0,0,600,600);
}

void MapCropBorderItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(QColor(0,0,0,200));
    painter->setPen(Qt::NoPen);
    painter->drawRect(0,0,600,50);
    painter->drawRect(0,250,600,200);
    painter->drawRect(0,50,120,200);
    painter->drawRect(420,50,200,200);
    painter->setPen(QColor(60,100,200));
    QPen pen = painter->pen();
    pen.setWidth(2);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(120,50,300,200);

}
