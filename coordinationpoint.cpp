#include "coordinationpoint.h"
#include <QPainter>
#include <QPointF>
#include "coordinationscene.h"

CoordinationPoint::CoordinationPoint(QObject *parent)
    : QObject{parent}
{
    setAcceptHoverEvents(true);

    setObjectName("CoordinationPoint");
}

QRectF CoordinationPoint::boundingRect() const
{
    return (!(isHover || isActive)? QRectF(-5 - 1, -18 - 1, 10 + 1 * 2, 18 + 1 * 2): QRectF(-8 - 1, -23 - 1, 16 + 1 * 2, 23 + 1 * 2));
}

void CoordinationPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(!(isHover || isActive)){
        QPen pen;
        pen.setColor(QColor(Qt::red));
        pen.setWidth(1);
        pen.setBrush(QColor(Qt::red));
        painter->setPen(pen);
        painter->setBrush(QColor(Qt::red));
        painter->drawEllipse(QPoint(0, -13), 5, 5);
        painter->drawPolygon({QPointF(0, 0), QPointF(-4.615, -11.076), QPointF(4.615, -11.076)});
    }else{
        QPen pen;
        pen.setColor(QColor(Qt::red));
        pen.setWidth(1);
        pen.setBrush(QColor(Qt::red));
        painter->setPen(pen);
        painter->setBrush(QColor(Qt::red));
        painter->drawEllipse(QPoint(0, -13), 7, 7);
        painter->drawPolygon({QPointF(0, 0), QPointF(-7.058, -13.235), QPointF(7.058, -13.235)});
    }
}

void CoordinationPoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        isActive = !isActive;
        parent->touchPoint = true;
        qDebug() << "Point Pressed";
        emit pressCoordPoint(this, isActive);

    }
    QGraphicsItem::mousePressEvent(event);
}

void CoordinationPoint::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    isHover = true;

    QGraphicsItem::hoverEnterEvent(event);
}

void CoordinationPoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    isHover = false;

    QGraphicsItem::hoverLeaveEvent(event);
}

bool CoordinationPoint::eventFilter(QObject *object, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress){
        return true;
    }
    return false;
}

qreal CoordinationPoint::getY_0() const
{
    return y_0;
}

qreal CoordinationPoint::getX_0() const
{
    return x_0;
}

void CoordinationPoint::setParent(CoordinationScene *newParent)
{
    parent = newParent;
}

void CoordinationPoint::initCoordination(qreal x, qreal y)
{
    x_0 = x;
    y_0 = y;
    setToolTip("Longetude: " + QString::number(x) + ", Latitude: " + QString::number(y));
}
