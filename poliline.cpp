#include "poliline.h"
#include <QtMath>

PoliLine::PoliLine(QObject *parent)
    : QObject{parent}
{}

void PoliLine::setPath(QPainterPath &path)
{
    QGraphicsPathItem::setPath(path);
}

MapControlPoint *PoliLine::getStartPoint()
{
    return startPoint;
}

void PoliLine::setStartPoint(MapControlPoint *newStartPoint)
{
    startPoint = newStartPoint;
}

MapControlPoint *PoliLine::getFinishPoint()
{
    return finishPoint;
}

void PoliLine::setFinishPoint(MapControlPoint *newFinishPoint)
{
    finishPoint = newFinishPoint;
}

QPointF PoliLine::getEndPoint() const
{
    return endPoint;
}

void PoliLine::setEndPoint(QPointF newEndPoint)
{
    endPoint = newEndPoint;
}

qreal PoliLine::calculateDistance()
{
    QPointF oldPoint = QPointF(this->path().elementAt(0).x, this->path().elementAt(0).y);
    qreal dist = 0;
    for(int i = 1; i < this->path().elementCount(); i++){
        QPointF newPoint = QPointF(this->path().elementAt(i).x, this->path().elementAt(i).y);
        //dist += qSqrt(qPow(oldPoint.x() - newPoint.x(), 2) + qPow(oldPoint.y() - newPoint.y(), 2));
        dist += srtm->calculateDistance(oldPoint.x(), oldPoint.y(), newPoint.x(), newPoint.y(), scaleMap, longetude_lt_currentMap, latitude_lt_currentMap);
        oldPoint = newPoint;
    }
    dist *= scaleMap;
    distance = dist;
    return dist;
}

QGraphicsSimpleTextItem *PoliLine::getText()
{
    return text;
}

void PoliLine::setText(QGraphicsSimpleTextItem *newText)
{
    text = newText;
}

void PoliLine::setSrtm(DistanseSrtm *newSrtm)
{
    srtm = newSrtm;
}

void PoliLine::setLongetude_lt_currentMap(qreal newLongetude_lt_currentMap)
{
    longetude_lt_currentMap = newLongetude_lt_currentMap;
}

void PoliLine::setLatitude_lt_currentMap(qreal newLatitude_lt_currentMap)
{
    latitude_lt_currentMap = newLatitude_lt_currentMap;
}
