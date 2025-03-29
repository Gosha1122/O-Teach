#ifndef POLILINE_H
#define POLILINE_H

#include <QObject>
#include <QGraphicsPathItem>
#include "mapcontrolpoint.h"

class PoliLine : public QObject, public QGraphicsPathItem
{
    Q_OBJECT
public:
    explicit PoliLine(QObject *parent = nullptr);
    void setPath(QPainterPath& path);

    MapControlPoint *getStartPoint() const;
    void setStartPoint(MapControlPoint *newStartPoint);

    MapControlPoint *getFinishPoint() const;
    void setFinishPoint(MapControlPoint *newFinishPoint);

    QPointF getEndPoint() const;
    void setEndPoint(QPointF newEndPoint);

    qreal calculateDistance();
signals:

private:
    MapControlPoint* startPoint;
    MapControlPoint* finishPoint;
    QPointF endPoint;
    qreal distance = -1;
    qreal scaleMap = 1;
};

#endif // POLILINE_H
