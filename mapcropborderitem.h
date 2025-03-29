#ifndef MAPCROPBORDERITEM_H
#define MAPCROPBORDERITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

class MapCropBorderItem :public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit MapCropBorderItem(QObject *parent = nullptr);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

signals:
};

#endif // MAPCROPBORDERITEM_H
