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

    MapControlPoint *getStartPoint();
    void setStartPoint(MapControlPoint *newStartPoint);

    MapControlPoint *getFinishPoint();
    void setFinishPoint(MapControlPoint *newFinishPoint);

    QPointF getEndPoint() const;
    void setEndPoint(QPointF newEndPoint);

    qreal calculateDistance();
    QGraphicsSimpleTextItem *getText();
    void setText(QGraphicsSimpleTextItem *newText);

signals:

private:
    MapControlPoint* startPoint;
    MapControlPoint* finishPoint;
    QPointF endPoint;
    qreal distance = -1;
    qreal scaleMap = 1;

    QGraphicsSimpleTextItem* text;
};

#endif // POLILINE_H
