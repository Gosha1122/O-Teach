#include "maptext.h"
#include <QPen>
#include <QColor>

MapText::MapText(QObject *parent)
    : QObject{parent}
{}

MapText::MapText(const QString &text, QGraphicsItem *item)
{
    QGraphicsSimpleTextItem(text, item);
}

QRectF MapText::boundingRect() const
{
    return QGraphicsSimpleTextItem::boundingRect();
}

void MapText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(!originColor.isValid()){
        originColor = this->pen().color();
    }
    if(originColorFlag){
        QPen pen = this->pen();
        pen.setColor(originColor);
        this->setPen(pen);
    }
    QGraphicsSimpleTextItem::paint(painter, option, widget);
}

void MapText::setOriginColorFlag(bool newOriginColorFlag)
{
    originColorFlag = newOriginColorFlag;
}
