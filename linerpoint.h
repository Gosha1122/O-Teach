#ifndef LINERPOINT_H
#define LINERPOINT_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>

class LinerPoint :public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit LinerPoint(QObject *parent = nullptr);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

signals:
    void updatePositionSignal();
private:
    QPointF previousPosition;
    bool leftMouseClick = false;

    void setPreviousPosition(const QPointF newPos);
};

#endif // LINERPOINT_H
