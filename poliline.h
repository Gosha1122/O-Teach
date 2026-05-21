#ifndef POLILINE_H
#define POLILINE_H

#include <QObject>
#include <QGraphicsPathItem>
#include "mapcontrolpoint.h"
#include "distansesrtm.h"

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

    void setSrtm(DistanseSrtm *newSrtm);

    void setLongetude_lt_currentMap(qreal newLongetude_lt_currentMap);

    void setLatitude_lt_currentMap(qreal newLatitude_lt_currentMap);

signals:

private:
    MapControlPoint* startPoint;
    MapControlPoint* finishPoint;
    QPointF endPoint;
    qreal distance = -1;
    qreal scaleMap = 1;

    qreal longetude_lt_currentMap = 0;
    qreal latitude_lt_currentMap  = 0;

    QGraphicsSimpleTextItem* text;

    DistanseSrtm* srtm;
};

#endif // POLILINE_H
