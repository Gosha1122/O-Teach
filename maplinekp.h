#ifndef MAPLINEKP_H
#define MAPLINEKP_H

#include <QObject>
#include <QGraphicsLineItem>

class MapLineKP : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    enum StartPoint{
        noStartPoint,
        beginStartPoint,
        endStartPoint,
        beginAndEndStartPoint
    };

    explicit MapLineKP(QObject *parent = nullptr);
    void setKPLine(qreal x1, qreal y1, qreal x2, qreal y2);
    void updateKP();

    void setRKP(qreal newRKP);

    QPointF getStartPoint() const;

    QPointF getFinishPoint() const;

    void setRStart(qreal newRStart);

    void setFlagStartPoint(StartPoint newFlagStartPoint);

protected:

private:
    qreal rKP;
    qreal rStart;
    QPointF startPoint;
    QPointF finishPoint;
    StartPoint flagStartPoint;

signals:
};

#endif // MAPLINEKP_H
