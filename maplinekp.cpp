#include "maplinekp.h"

MapLineKP::MapLineKP(QObject *parent)
    : QObject{parent}
{}

void MapLineKP::setKPLine(qreal x1, qreal y1, qreal x2, qreal y2)
{
    qreal x1_new = x1;
    qreal y1_new = y1;
    qreal x2_new = x2;
    qreal y2_new = y2;
    auto dx = qAbs(x2 - x1);
    auto dy = qAbs(y2 - y1);
    qreal cos_a = dx / (qSqrt(dx * dx + dy * dy));
    qreal sin_a = dy / (qSqrt(dx * dx + dy * dy));
    if(x2 > x1){
        x1_new += (flagStartPoint == beginStartPoint || flagStartPoint == beginAndEndStartPoint? rStart: rKP) * cos_a;
        x2_new -= (flagStartPoint == endStartPoint || flagStartPoint == beginAndEndStartPoint? rStart: rKP) * cos_a;
    }else if(x2 < x1){
        x1_new -= (flagStartPoint == beginStartPoint || flagStartPoint == beginAndEndStartPoint? rStart: rKP) * cos_a;
        x2_new += (flagStartPoint == endStartPoint || flagStartPoint == beginAndEndStartPoint? rStart: rKP) * cos_a;
    }
    if(y2 > y1){
        y1_new += (flagStartPoint == beginStartPoint || flagStartPoint == beginAndEndStartPoint? rStart: rKP) * sin_a;
        y2_new -= (flagStartPoint == endStartPoint || flagStartPoint == beginAndEndStartPoint? rStart: rKP) * sin_a;
    }else if(y2 < y1){
        y1_new -= (flagStartPoint == beginStartPoint || flagStartPoint == beginAndEndStartPoint? rStart: rKP) * sin_a;
        y2_new += (flagStartPoint == endStartPoint || flagStartPoint == beginAndEndStartPoint? rStart: rKP) * sin_a;
    }
    startPoint = QPointF(x1, y1);
    finishPoint = QPointF(x2, y2);
    this->setLine(x1_new, y1_new, x2_new, y2_new);
}

void MapLineKP::updateKP()
{
    setKPLine(this->startPoint.x(), this->startPoint.y(), this->finishPoint.x(), this->finishPoint.y());
    this->update();
}

void MapLineKP::setRKP(qreal newRKP)
{
    rKP = newRKP;
}

QPointF MapLineKP::getStartPoint() const
{
    return startPoint;
}

QPointF MapLineKP::getFinishPoint() const
{
    return finishPoint;
}

void MapLineKP::setRStart(qreal newRStart)
{
    rStart = newRStart;
}

void MapLineKP::setFlagStartPoint(StartPoint newFlagStartPoint)
{
    flagStartPoint = newFlagStartPoint;
}
