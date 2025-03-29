#ifndef MAPTEXT_H
#define MAPTEXT_H

#include <QObject>
#include <QGraphicsSimpleTextItem>


class QColor;

class MapText : public QObject, public QGraphicsSimpleTextItem
{
    Q_OBJECT
public:
    explicit MapText(QObject *parent = nullptr);
    explicit MapText(const QString &text, QGraphicsItem* item = nullptr);

    void setOriginColorFlag(bool newOriginColorFlag);

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

signals:

private:
    QColor originColor;

    bool originColorFlag;
};

#endif // MAPTEXT_H
