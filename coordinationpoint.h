#ifndef COORDINATIONPOINT_H
#define COORDINATIONPOINT_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>

class CoordinationScene;
class CoordinationPoint : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit CoordinationPoint(QObject *parent = nullptr);

    void initCoordination(qreal x, qreal y);

    void setParent(CoordinationScene *newParent);

    qreal getX_0() const;

    qreal getY_0() const;

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    bool eventFilter(QObject* object, QEvent* event) override;

signals:
    void pressCoordPoint(CoordinationPoint* point, bool is_newActive);

private:
    bool isActive = false;
    bool isHover  = false;

    CoordinationScene* parent;

    qreal x_0;
    qreal y_0;
};

#endif // COORDINATIONPOINT_H
