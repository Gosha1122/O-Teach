#ifndef MAPCONTROLPOINT_H
#define MAPCONTROLPOINT_H

#include <QGraphicsItem>
#include <QObject>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QDebug>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>


class MapScene;
class MapLineKP;
class QGraphicsSimpleTextItem;

class MapControlPoint :public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    enum Shape{
      Start,
      KP,
      Finish
    };
    explicit MapControlPoint(QObject* parent = nullptr);
    void setShape(Shape newShape);
    void setSettings(QColor textColor, QColor penColor, int textSize, int KPSize, int penWidth, int textStyle,
                     int StartSize, int  StartWidth, QColor StartColor, int LineWidth, QColor LineColor);

    MapControlPoint *getPrevPoint() const;
    void setPrevPoint(MapControlPoint *newPrevPoint);

    void setParentScene(MapScene *newParentScene);
    void setPreviousPosition(QPointF pos);

    MapLineKP *getStartLine() const;
    void setStartLine(MapLineKP *newStartLine);

    MapLineKP *getFinishLine() const;
    void setFinishLine(MapLineKP *newFinishLine);

    void setColorPoint(const QColor &newColorPoint);

    QColor getColorPoint() const;

    void setColorAlphaFlag(bool newColorAlphaFlag);

    void setNumberKP(int num);

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
     void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
     void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
signals:
    void removeMapPoint(MapControlPoint* point);
    void moveMapPoint(QPointF oldPos, QPointF newPos);
    void movePointSignal(MapControlPoint* mp);
private:
    Shape shape;

    QColor colorPoint;
    bool   colorAlphaFlag = false;

    QColor textColor;
    QColor penColor ;
    int    textSize ;
    int    KPSize   ;
    int    penWidth  = 3;
    int    textStyle;

    QColor StartColor;
    QColor LineColor;
    int    StartSize;
    int    StartWidth;
    int    LineWidth;

    MapControlPoint* prevPoint;
    MapScene* parentScene;
    QPointF previousPosition;
    bool leftButtonPresed = false;

    MapLineKP* startLine = nullptr;
    MapLineKP* finishLine = nullptr;

    QPoint minPoint(QPoint p1, QPoint p2, QPoint p3);

    QGraphicsSimpleTextItem* number;

};



#endif // MAPCONTROLPOINT_H
